
set(COMPONENTS
    spi_flash 
    esp_wifi
    esp_event
    nvs_flash
    esp_netif
    shared-types
)

set(SOURCE_FILES 
    anchor_main.c
)

idf_component_register(
    SRCS ${SOURCE_FILES} 
    PRIV_REQUIRES ${COMPONENTS}
    INCLUDE_DIRS ""
)

