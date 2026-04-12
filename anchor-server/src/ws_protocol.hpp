
#ifndef WS_PROTOCOL_HPP
#define WS_PROTOCOL_HPP

#include <cstdint>
#include <cstdio>

enum CommandID : uint8_t
{
    WS_CMD_ID_UNKNOWN = 0,
    WS_CMD_ID_REGISTER_BEACON = 1,
    WS_CMD_ID_REQUEST_BEACONS = 2,
};

struct __attribute__((packed)) PktHeader
{
    CommandID cmdId;
    uint8_t pktSize;
};

struct __attribute__((packed)) RegisterBeacon
{
    uint8_t mac[6];
};

struct __attribute__((packed)) RequestBeacons
{
    uint8_t count;
    uint8_t mac[5][6];
};

struct __attribute__((packed)) WsProtocolPacket
{
    PktHeader header;
    union
    {
        RegisterBeacon registerBeacon;
        RequestBeacons requestBeacons;
    };
};

#endif // WS_PROTOCOL_HPP
