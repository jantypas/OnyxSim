//
// Created by jantypas on 1/3/23.
//
#include <stdint-gcc.h>
#include <map>
#ifndef ONYXSIM_ONYX1_H
#define ONYXSIM_ONYX1_H
#include "../../../Processes/Process.h"
#include "../../../IO/IOInterface.h"
#include "../../../BIOS/BIOSInterface.h"
#include "../../../Monitor/MonitorInterface.h"
#include "../../../Memory/Local/LinearMemory/LinearMemory.h"
#include "../../../CPU/CPU.h"

#define REGMODE_DIRECT      0x00
#define REGMODE_DIRECTX     0x01
#define REGMODE_DIRECTY     0x02
#define REGMODE_POSTINC     0x03
#define REGMODE_POSTDEC     0x04
#define REGMODE_PREINC      0x05
#define REGMODE_PREDEC      0x06

#define MAX_DATA_REGISTERS      0x20
#define MAX_ADDRESS_REGISTERS   0x20
#define MAX_CONTROL_REGISTERS   0x20
#define MAX_REGISTERS           0x60
#define MAX_PROCESSES           1


#define IS_DATA_REGISTER(x) (x < 0x20)
#define IS_ADDRESS_REGISTER(x) ( x > 0x20 && x < 0x40)
#define IS_CONTROL_REGISTER(x) (x >= 0x40 && x < MAX_REGISTERS)

enum CPUEventType {
    NO_EVENT,
    IO_EVENT,
    TRQP_EVENT,
    MONITOR_EVENT
};

union CPUEventInfo {
    struct { } ioevent;
    struct { } trapevent;
    struct { } monitorevent;
};

struct CPUEvent {
    CPUEventType    eventType;
    CPUEventInfo    data;
};

struct Onyx1DecodedInstruction {
public :
    uint64_t    tempRegisters[8];
    uint16_t    opCode;
    uint8_t     numRegisters;
    struct {
            bool    store;
            uint8_t name;
            uint8_t mode;
    } Register[8];
};

class CPUContextObject {
public :
    Onyx1DecodedInstruction decoded;
    CPUEvent            events;
    int64_t             dataRegisters[MAX_DATA_REGISTERS];
    uint64_t            addressRegisters[MAX_ADDRESS_REGISTERS];
    uint64_t            controlRegisters[MAX_CONTROL_REGISTERS];
    uint64_t            instructionCount;
    uint64_t            PC;                // Program counter
    uint64_t            DSP;               // Data Stack pointer
    uint64_t            CSP;               // Code stack pointer
    uint64_t            SSP;               // Shadow stack of code stack pointer
};

class Onyx1L : OnyxCPU {
private :
    bool Read(uint64_t *addr, int sz, uint64_t *value);
    bool Write(uint64_t *addr, int sz, uint64_t value);
public :
    Onyx1DecodedInstruction                 decodedInstruction;
    CPUContextObject                        context;
    uint64_t                                cpuModel;
    uint64_t                                featureSetA;
    uint64_t                                featureSetB;
    LinearMemory                            *memory;
    IOInterface                             *io;
    BIOSInterface                           *bios;
    MonitorInterface                        *monitor;
    void Tick() override {

    }
    Onyx1L();
    bool Onyx1LEventService();
    bool Onyx1LFetch(Onyx1DecodedInstruction *instr);
    bool Onyx1LDecode(Onyx1DecodedInstruction *instr);
    bool Onyx1LExecute();
    bool Onyx1LStore();
};


#endif //ONYXSIM_ONYX1_H
