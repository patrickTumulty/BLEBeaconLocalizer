
#ifndef MESSAGE_TYPES_H
#define MESSAGE_TYPES_H

#include <cstdint>

typedef enum
{
    CMD_ID_UNKNOWN = 0,
    CMD_ID_PING = 1,
    CMD_ID_REGISTER_BEACON = 2,
    CMD_ID_REPORT_BEACON_STATS = 3,
} AnchorCommandID;

typedef struct __attribute__((packed))
{
    uint8_t magic;
    uint8_t cmd_id;
    uint8_t pkt_size;
    uint8_t anchor_id;
    uint32_t pkt_id;
} AnchorPacketHeader;

#define BEACON_ACTION_ADD (1)
#define BEACON_ACTION_REMOVE (2)

typedef struct __attribute__((packed))
{
    bool beacon_action;
    uint8_t mac[6];
} RegisteBeaconCommand;

typedef struct __attribute__((packed))
{
    uint8_t mac[6];
    float db;
} ReportBeaconStatusCommand;

typedef struct __attribute__((packed))
{
    AnchorPacketHeader header;
    union
    {
        RegisteBeaconCommand register_beacon;
        ReportBeaconStatusCommand report_beacon;
    };
} AnchorPacket;


#endif // MESSAGE_TYPES_H
