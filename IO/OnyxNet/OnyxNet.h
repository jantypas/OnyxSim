//
// Created by jantypas on 1/3/23.
//

#include <stdint-gcc.h>
#ifndef ONYXSIM_ONYXNET_H
#define ONYXSIM_ONYXNET_H

#define ONYX_PACKET_SIZE                        1200
#define ONYXNET_PACKET_HEARTBEAT                0x0000'0000
#define ONYXNET_PACKET_HEARTBEAT_REQUEST        0x0000'0001
#define ONYXNET_PACKET_HEARTBEAT_ACK            0x0000'0002
#define ONYXNET_PACKET_CONFIG_REQUEST           0x0000'0003
#define ONYXNET_PACKET_CONFIG_ACK               0x0000'0004
#define ONYXNET_PACKET_CONFIG_NAK               0x0000'0005
#define ONYXNET_PACKET_ROUTE_ANNOUNCEMENT       0x0000'0006
#define ONYXNET_PACKET_DATA_PACKET              0x0000'0007
#define ONYXNET_PACKET_DATA_STREAM              0x0000'0008

#define ONYXNET_SPECIAL_DATA       0x0000'0000
#define ONYXNET_SPECIAL_CONTROL    0x0000'0001
#define ONYXNET_SPECIAL_PRIORITY   0x0000'0002

struct OnyxPacketAddress {
    // 8 bytes
    uint8_t     clusterNode;
    uint8_t     nodeID;
    uint16_t    groupID;
    uint16_t    areaID;
    uint16_t    regionID;
};

struct OnyxPacketHeader { // 64 bytes
    OnyxPacketAddress source;       // +0 bytes
    uint32_t          srcChannel;   // +8 bytes
    OnyxPacketAddress destination;  // +12 bytes
    uint32_t          destChannel;  // +20 bytes
    uint32_t          packetType;   // +24 bytes
    uint64_t          trafficClass; // +32 bytes
    uint32_t          flowID;       // +36 bytes
    uint32_t          windowID;     // +40 bytes
    uint32_t          specialBits;  // +44 bytes
    uint32_t          crc;          // +48 bytes
    uint16_t          ttl;          // +50 bytes
    uint16_t          length;       // +52 bytes
    uint64_t          timestamp;    // +60 bytes
    uint8_t           unused[12];   // +68 Future space
};

struct OnyxPacket {
    OnyxPacketHeader    header;
    uint8_t             data[ONYX_PACKET_SIZE];
};

class OnyxNet {

};


#endif //ONYXSIM_ONYXNET_H
