//
// Created by jantypas on 1/3/23.
//
#include <stdint-gcc.h>
#ifndef ONYXSIM_ONYX1_H
#define ONYXSIM_ONYX1_H
#include "../../CPU/Microcode/CPUMicrocode.h"
#include "../../Memory/MemInterface.h"
#include "../../Processes/Process.h"
#include "../../IO/IOInterface.h"
#include "../../BIOS/BIOSInterface.h"
#include "../../Monitor/MonitorInterface.h"

class Onyx1 {
public :
    uint64_t            *dataRegisters;
    uint64_t            *addressRegisters;
    uint64_t            *controlRegisters;
    uint64_t             instructionCount;
    uint64_t             cpuModel;
    uint64_t             featureSetA;
    uint64_t             featureSetB;
    uint64_t             PC;            // Program counter
    uint64_t             SP;            // Stack pointer
    uint64_t             SSP;           // Shadow stack pointer
    Process             *processID;     // Current process;
    uint8_t              privLevel;     // Processor privilege level
    MemInterface        *memory;
    IOInterface         *io;
    BIOSInterface       *bios;
    MonitorInterface    *monitor;
};


#endif //ONYXSIM_ONYX1_H
