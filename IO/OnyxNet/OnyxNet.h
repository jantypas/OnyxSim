//
// Created by jantypas on 1/3/23.
//

#include <stdint-gcc.h>
#ifndef ONYXSIM_ONYXNET_H
#define ONYXSIM_ONYXNET_H

#define ONYX_PACKET_SIZE    1200
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
    OnyxPacketAddress source;       // 8 bytes
    uint32_t          srcChannel;   // 4 bytes
    OnyxPacketAddress destination;  // 8 bytes
    uint32_t          destChannel;  // 4 bytes
    uint32_t          packetType;   // 4 bytes
    uint64_t          trafficClass; // 8 butes
    uint32_t          flowID;       // 4 bytes
    uint32_t          windowID;     // 4 bytes
    uint32_t          specialBits;  // 4 bytes
    uint32_t          crc;          // 4 bytes
    uint16_t          ttl;          // 2 bytes
    uint16_t          length        // 2 bytes
    uint8_t           unused[10]    // Future space
};

struct OnyxPacket {
    OnyxPacketHeader    header;
    uint8_t             data[ONYX_PACKET_SIZE];
};

class OnyxNet {

};


#endif //ONYXSIM_ONYXNET_H
