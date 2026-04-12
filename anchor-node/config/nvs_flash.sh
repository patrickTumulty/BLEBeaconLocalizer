#!/bin/bash

OCTET=$1
PORT=$2
WIFI_SSID=$3
WIFI_PASS=$4

# Simple argument check
if [ -z "$OCTET" ] || [ -z "$PORT" ] || [ -z "$WIFI_SSID" ] || [ -z "$WIFI_PASS" ]; then
    echo "Usage: $0 <octet> <port> <wifi_ssid> <wifi_pass>"
    echo "Example: $0 101 /dev/ttyACM0 MyNetwork MyPassword"
    exit 1
fi

# Echo provided arguments
echo "Octet: $OCTET"
echo "Port: $PORT"
echo "WiFi SSID: $WIFI_SSID"
echo "WiFi Password: $WIFI_PASS"

cat > nvs_config.csv << EOF
key,type,encoding,value
net_config,namespace,,
last_octet,data,u8,$OCTET
wifi_ssid,data,string,$WIFI_SSID
wifi_pass,data,string,$WIFI_PASS
EOF

NVS_GEN_PATH=$IDF_PATH/components/nvs_flash/nvs_partition_generator 
PARTTOOL_PATH=$IDF_PATH/components/partition_table

python $NVS_GEN_PATH/nvs_partition_gen.py generate nvs_config.csv nvs_config.bin 0x6000

python $PARTTOOL_PATH/parttool.py --port $PORT write_partition --partition-name nvs --input nvs_config.bin
