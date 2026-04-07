#include "esp_event.h"
#include "esp_http_client.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"
#include "nvs.h"
#include "nvs_flash.h"

#define WIFI_SSID "DirtyBirdyEstate"
#define WIFI_PASS "huskycartoon113"
#define WIFI_CONNECTED_BIT BIT0

static const char *TAG = "wifi_test";
static EventGroupHandle_t wifi_events;

struct AppConfig
{
    uint8_t last_octet;
    char wifi_name[64];
    size_t wifi_name_len;
    char wifi_password[64];
    size_t wifi_password_len;
};

uint8_t get_last_octet()
{
    nvs_handle_t handle;
    uint8_t octet = 100; // default fallback

    esp_err_t err = nvs_open("net_config", NVS_READONLY, &handle);
    if (err == ESP_OK)
    {
        nvs_get_u8(handle, "last_octet", &octet);
        nvs_close(handle);
    }
    return octet;
}

void set_last_octet(uint8_t octet)
{
    nvs_handle_t handle;
    nvs_open("net_config", NVS_READWRITE, &handle);
    nvs_set_u8(handle, "last_octet", octet);
    nvs_commit(handle);
    nvs_close(handle);
}

static void event_handler(void *arg, esp_event_base_t base,
                          int32_t id, void *data)
{
    if (base == WIFI_EVENT && id == WIFI_EVENT_STA_START)
        esp_wifi_connect();
    else if (base == IP_EVENT && id == IP_EVENT_STA_GOT_IP)
        xEventGroupSetBits(wifi_events, WIFI_CONNECTED_BIT);
    else if (base == WIFI_EVENT && id == WIFI_EVENT_STA_DISCONNECTED)
        esp_wifi_connect(); // retry on disconnect
}

static struct AppConfig load_app_config()
{
    struct AppConfig app_cfg = {0};
    nvs_handle_t handle;

    esp_err_t err = nvs_open("net_config", NVS_READONLY, &handle);
    if (err != ESP_OK)
    {
        nvs_close(handle);
        return app_cfg;
    }

    nvs_get_u8(handle, "last_octet", &app_cfg.last_octet);
    nvs_get_str(handle, "wifi_name", app_cfg.wifi_name, &app_cfg.wifi_name_len);
    nvs_get_str(handle, "wifi_pass", app_cfg.wifi_password, &app_cfg.wifi_password_len);
    nvs_close(handle);

    return app_cfg;
}

void app_main(void)
{
    nvs_flash_init();


    struct AppConfig app_cfg = {};

    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();

    wifi_events = xEventGroupCreate();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);

    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, event_handler, NULL);
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, event_handler, NULL);

    wifi_config_t wifi_config = {
        .sta = {
                .ssid = WIFI_SSID,
                .password = WIFI_PASS,
                },
    };
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(WIFI_IF_STA, &wifi_config);

    esp_netif_t *netif = esp_netif_create_default_wifi_sta();

    esp_netif_dhcpc_stop(netif); // disable DHCP client

    uint8_t last_octet = get_last_octet();

    esp_netif_ip_info_t ip_info = {
        .ip = ESP_IP4TOADDR(10, 10, 0, last_octet),
        .netmask = ESP_IP4TOADDR(255, 255, 255, 0),
        .gw = ESP_IP4TOADDR(10, 10, 0, 1),
    };
    esp_netif_set_ip_info(netif, &ip_info);

    esp_wifi_start();

    // Wait until connected and got IP
    xEventGroupWaitBits(wifi_events, WIFI_CONNECTED_BIT, false, true, portMAX_DELAY);
    ESP_LOGI(TAG, "Connected to WiFi");

    // Now safe to make HTTP request
    esp_http_client_config_t config = {
        .url = "http://clients3.google.com/generate_204",
        .timeout_ms = 3000,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_err_t err = esp_http_client_perform(client);
    int status = esp_http_client_get_status_code(client);
    esp_http_client_cleanup(client);

    if (err == ESP_OK && status == 204)
        ESP_LOGI(TAG, "Internet looks good!");
    else
        ESP_LOGE(TAG, "HTTP check failed, status: %d", status);
}
