//
// Created by jantypas on 1/3/23.
//
#include <stdint-gcc.h>
#ifndef ONYXSIM_ONYX1_H
#define ONYXSIM_ONYX1_H
#include "../../Onyx1L//Onyx1LMicrocode/Onyx1LMicrocode.h"
#include "../../../Processes/Process.h"
#include "../../../IO/IOInterface.h"
#include "../../../BIOS/BIOSInterface.h"
#include "../../../Monitor/MonitorInterface.h"
#include "../../../Memory/Local/LinearMemory/LinearMemory.h"

enum CPUState {
};

struct CPUEvent {
    CPUState    state;
};

class Onyx1L {
public :
    CPUEvent            events;
    uint16_t            ccs, dsp, cssp, dssp, esp, xsp; // Segment pointers
    uint64_t            *dataRegisters;
    uint64_t            *addressRegisters;
    uint64_t            *controlRegisters;
    uint64_t             instructionCount;
    uint64_t             cpuModel;
    uint64_t             featureSetA;
    uint64_t             featureSetB;
    uint64_t             PC;                // Program counter
    uint64_t             DSP;               // Data Stack pointer
    uint64_t             CSP;               // Code stack pointer
    uint64_t             SSP;               // Shadow stack of code stack pointer
    Process             *processID;         // Current process;
    uint8_t              privLevel;         // Processor privilege level
    LinearMemory        *memory;
    IOInterface         *io;
    BIOSInterface       *bios;
    MonitorInterface    *monitor;
    Onyx1LMicrocode     *microcode;
};


#endif //ONYXSIM_ONYX1_H
