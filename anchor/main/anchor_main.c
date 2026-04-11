#include "esp_event.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"
#include "nvs.h"
#include "nvs_flash.h"
#include <stdint.h>
#include <string.h>

#define WIFI_CONNECTED_BIT BIT0

static const char *TAG = "ANCHOR";
static EventGroupHandle_t wifi_events;

struct AppConfig
{
    uint8_t last_octet;
    char wifi_ssid[32];
    size_t wifi_ssid_len;
    char wifi_password[64];
    size_t wifi_password_len;
};

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

    ESP_LOGI(TAG, "Loading App Config");

    esp_err_t err = nvs_open("net_config", NVS_READONLY, &handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error loading app config!");
        nvs_close(handle);
        return app_cfg;
    }

    nvs_get_u8(handle, "last_octet", &app_cfg.last_octet);

    size_t ssid_len = sizeof(app_cfg.wifi_ssid);
    nvs_get_str(handle, "wifi_ssid", app_cfg.wifi_ssid, &ssid_len);
    app_cfg.wifi_ssid_len = ssid_len;

    size_t pass_len = sizeof(app_cfg.wifi_password);
    nvs_get_str(handle, "wifi_pass", app_cfg.wifi_password, &pass_len);
    app_cfg.wifi_password_len = pass_len;

    nvs_close(handle);

    ESP_LOGI(TAG, "== App Config ==");
    ESP_LOGI(TAG, "IP   : 10.10.0.%d", app_cfg.last_octet);
    ESP_LOGI(TAG, "SSID : '%s' len=%d", app_cfg.wifi_ssid, app_cfg.wifi_ssid_len);
    ESP_LOGI(TAG, "PASS : '%s' len=%d", app_cfg.wifi_password, app_cfg.wifi_password_len);
    ESP_LOGI(TAG, "================");

    return app_cfg;
}

static void configure_wifi(const struct AppConfig *app_cfg)
{
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_t *netif = esp_netif_create_default_wifi_sta();

    wifi_events = xEventGroupCreate();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);

    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, event_handler, NULL);
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, event_handler, NULL);


    wifi_config_t wifi_config = {0};
    strlcpy((char *) wifi_config.sta.ssid, app_cfg->wifi_ssid, sizeof(wifi_config.sta.ssid));
    strlcpy((char *) wifi_config.sta.password, app_cfg->wifi_password, sizeof(wifi_config.sta.password));
    wifi_config.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;

    ESP_LOGI(TAG, "Wifi Config: SSID : '%s'", wifi_config.sta.ssid);
    ESP_LOGI(TAG, "Wifi Config: PW   : '%s'", wifi_config.sta.password);

    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
    esp_netif_dhcpc_stop(netif); // disable DHCP client

    esp_netif_ip_info_t ip_info = {
        .ip.addr = ESP_IP4TOADDR(10, 10, 0, app_cfg->last_octet),
        .netmask.addr = ESP_IP4TOADDR(255, 255, 255, 0),
        .gw.addr = ESP_IP4TOADDR(10, 10, 0, 1),
    };

    esp_netif_set_ip_info(netif, &ip_info);
    esp_wifi_start();
}

void app_main(void)
{
    nvs_flash_init();

    struct AppConfig app_cfg = load_app_config();

    configure_wifi(&app_cfg);

    // Wait until connected and got IP
    xEventGroupWaitBits(wifi_events, WIFI_CONNECTED_BIT, false, true, portMAX_DELAY);
    ESP_LOGI(TAG, "Connected to WiFi");

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
