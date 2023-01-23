//
// Created by jantypas on 1/3/23.
//
#include <stdint-gcc.h>
#ifndef ONYXSIM_ONYX1_H
#define ONYXSIM_ONYX1_H
#include "../../../Processes/Process.h"
#include "../../../IO/IOInterface.h"
#include "../../../BIOS/BIOSInterface.h"
#include "../../../Monitor/MonitorInterface.h"
#include "../../../Memory/Local/LinearMemory/LinearMemory.h"

#define REGMODE_DIRECT      0x00
#define REGMODE_DIRECTX     0x01
#define REGMODE_DIRECTY     0x02
#define REGMODE_POSTINC     0x03
#define REGMODE_POSTDEC     0x04
#define REGMODE_PREINC      0x05
#define REGMODE_PREDEC      0x06

#define MAX_REGISTERS   0x80

#define IS_DATA_REGISTER(x) (x < 0x20)
#define IS_ADDRESS_REGISTER(x) ( x > 0x20 && x < 0x40)
#define IS_CONTROL_REGISTER(x) (x >= 0x40 && x < MAX_REGISTERS)

enum CPUState {
};

struct CPUEvent {
    CPUState    state;
};

struct Onyx1DecodedInstruction {
public :
    uint64_t    tempRegisters[8];
    uint16_t    opCode;
    uint8_t     worDSize;
    uint8_t     numRegisters;
    struct {
            bool    STORE;
            uint8_t name;
            uint8_t mode;
    } Register[8];
};

class Onyx1L {
public :
    Onyx1DecodedInstruction decoded;
    CPUEvent            events;
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
    Onyx1L();
    bool Onyx1LEventService();
    bool Onyx1LFetch();
    bool Onyx1LDecode();
    bool Onyx1LExecute();
    bool Onyx1LStore();
};


#endif //ONYXSIM_ONYX1_H
