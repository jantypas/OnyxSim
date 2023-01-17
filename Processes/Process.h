//
// Created by jantypas on 12/31/22.
//

#ifndef ONYXSIM_PROCESS_H
#define ONYXSIM_PROCESS_H
#include <string>
#include <vector>
#include "../IO/IOInterface.h"

#define SEGMENT_STATE_IN_USE        0x0001
#define SEGMENT_STATE_IS_LOCKED     0x0002
#define SEGMENT_STATE_IS_ON_DISK    0x0004

#define PRIV_LEVEL_NORMAL   0x00
#define PRIV_LEVEL_SERVICE  0x01
#define PRIV_LEVEL_DRIVER   0x02
#define PRIV_LEVEL_KERNEL   0x03

class ProcessSegment {
public :
    uint16_t        processID;
    std::string     name;
    uint32_t        numVirtualPages;
    uint32_t        *virtualPages;
    uint32_t        processState;
};

class Process {
public :
    uint32_t                userid;
    uint32_t                groupid;
    std::string             commandline;
    uint8_t                 numSegments;
    ProcessSegment         *segments;
    uint64_t               *dataRegisters;
    uint64_t               *addressRegisters;
    uint8_t                 privLevel;
    IOHandle                handleStdin;
    IOHandle                handleStdout;
    IOHandle                handleStdError;
    std::vector<IOHandle>   handleFiles;
};


#endif //ONYXSIM_PROCESS_H
