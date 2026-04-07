#!/bin/bash

OCTET = $1
PORT = $2

cat > nvs_config.csv << EOF
key,type,encoding,value
net_config,namespace,,
last_octet,data,u8,$OCTET
wifi_ssid,data,string,DirtyBirdyEstate
wifi_pass,data,string,huskycartoon113
EOF

python $IDF_PATH/components/nvs_flash/nvs_partition_generator/nvs_partition_gen.py \
    generate nvs_config.csv nvs_config.bin 0x6000

parttool.py - -port $PORT write_partition - -partition-name nvs - -input nvs_config.bin
