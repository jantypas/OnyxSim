//
// Created by jantypas on 1/3/23.
//

#include "Onyx1L.h"
#include <endian.h>
#include "../../../Memory/MemoryConstants.h"

bool Onyx1L::Onyx1LEventService() {
    return true;
}

/**
 * Get the next "chunk of bytes" for an instruction.
 * From the PC, fetch 16-bits and decode them to determine the number of 16-bits to follow.
 *
 */
bool Onyx1L::Onyx1LFetch() {
    uint8_t value1, value2;
    uint16_t opcode;
    bool result;

    if (memory->ReadAddress(PC++, &value1)) {
        if (memory->ReadAddress(PC++, &value2)) {
            opcode = (value1 << 8) + value2;
            decoded.opCode = (opcode & 0xFFE) >> 5;
            decoded.worDSize = (opcode & 0x) >> 2;
            decoded.numRegisters = (opcode & 0x3);
            for (uint8_t ix = 0; ix < decoded.numRegisters; ix++) {
                result = memory->ReadAddress(PC++, &value1);
                result = memory->ReadAddress(PC++, &value2);
                decoded.Register[ix].name = value1;
                decoded.Register[ix].mode = value2;
            }
        } else {
            uint16_t h = htobe32()
        }
    } else {

    }
    return true;
}

bool Onyx1L::Onyx1LDecode() {
    for (uint8_t ix = 0; ix < decoded.numRegisters; i++) {
        switch(decoded.Register[ix].mode) {
            case REGMODE_DIRECT : {
                if (IS_DATA_REGISTER(decoded.Register[ix].name)) {
                    decoded.tempRegisters[ix] = dataRegisters[ix];
                }
                if (IS_ADDRESS_REGISTER(decoded.Register[ix].name)) {
                    uint64_t newAddress = addressRegisters[decoded.Register[ix].name+0x20];
                    memory->ReadAddress(decoded.tempRegisters[ix]
                }
            }
            case REGMODE_DIRECTX :
            case REGMODE_DIRECTY :
            case REGMODE_POSTINC :
            case REGMODE_POSTDEC :
            case REGMODE_PREINC :
            case REGMODE_PREDEC :
            default :
        }
    }
    return true;
}

bool Onyx1L::Onyx1LExecute() {
    return true;
}

bool Onyx1L::Onyx1LStore() {
    return true;
}

bool Onyx1L::Read8(uint64_t addr, uint8_t *value) {
    return memory->ReadAddress(addr, value);
}

bool Onyx1L::Read16(uint64_t addr, uint16_t *value) {
    union {
        uint16_t    a;
        uint8_t     b[2];
    } temp;
    bool error = false;
    error = memory->ReadAddress(addr, &temp.b[0]);
    if (error != MEMORY_ERROR_NONE) { return error; };
    error = memory->ReadAddress(addr+1, &temp.b[1]);
    if (error != MEMORY_ERROR_NONE) { return error; };
    *value = b16toh(temp.a);
    return true;
}

bool Onyx1L::Read32(uint64_t addr, uint32_t *value) {
    union {
        uint32_t    a;
        uint8_t     b[4];
    } temp;
    bool error = false;
    error = memory->ReadAddress(addr, &temp.b[0]);
    if (error != MEMORY_ERROR_NONE) { return error; };
    error = memory->ReadAddress(addr+1, &temp.b[1]);
    if (error != MEMORY_ERROR_NONE) { return error; };
    error = memory->ReadAddress(addr+2, &temp.b[2]);
    if (error != MEMORY_ERROR_NONE) { return error; };
    error = memory->ReadAddress(addr+3, &temp.b[3]);
    if (error != MEMORY_ERROR_NONE) { return error; };
    *value = b32toh(temp.a);
    return true;
}

bool Onyx1L::Read64(uint64_t addr, uint64_t *value) {
    union {
        uint64_t    a;
        uint8_t     b[8];
    } temp;
    bool error = false;
    error = memory->ReadAddress(addr, &temp.b[0]);
    if (error != MEMORY_ERROR_NONE) { return error; };
    error = memory->ReadAddress(addr+1, &temp.b[1]);
    if (error != MEMORY_ERROR_NONE) { return error; };
    error = memory->ReadAddress(addr+2, &temp.b[2]);
    if (error != MEMORY_ERROR_NONE) { return error; };
    error = memory->ReadAddress(addr+3, &temp.b[3]);
    if (error != MEMORY_ERROR_NONE) { return error; };
    bool error = false;
    error = memory->ReadAddress(addr+4, &temp.b[4]);
    if (error != MEMORY_ERROR_NONE) { return error; };
    error = memory->ReadAddress(addr+5, &temp.b[5]);
    if (error != MEMORY_ERROR_NONE) { return error; };
    error = memory->ReadAddress(addr+6, &temp.b[6]);
    if (error != MEMORY_ERROR_NONE) { return error; };
    error = memory->ReadAddress(addr+7, &temp.b[7]);
    if (error != MEMORY_ERROR_NONE) { return error; };
    *value = b64toh(temp.a);
    return true;}

bool Onyx1L::Write8(uint64_t addr, uint8_t value) {
    return memory->WriteAddress(addr, value);
}

bool Onyx1L::Write16(uint64_t addr, uint16_t value) {
    union {
        uint16_t    a;
        uint8_t     b[2];
    } temp;
    bool error = false;
    temp.a = htobe16(value);
    error = memory->WriteAddress(addr, temp.b[0]);
    if (error != MEMORY_ERROR_NONE) { return error; };
    error = memory->WriteAddress(addr+1, temp.b[1]);
    if (error != MEMORY_ERROR_NONE) { return error; };
    return true;
}

bool Onyx1L::Write32(uint64_t addr, uint32_t value) {
    union {
        uint32_t    a;
        uint8_t     b[4];
    } temp;
    bool error = false;
    temp.a = htobe32(value);
    error = memory->WriteAddress(addr, temp.b[0]);
    if (error != MEMORY_ERROR_NONE) { return error; };
    error = memory->WriteAddress(addr+1, temp.b[1]);
    if (error != MEMORY_ERROR_NONE) { return error; };
    error = memory->WriteAddress(addr+2, temp.b[2]);
    if (error != MEMORY_ERROR_NONE) { return error; };
    error = memory->WriteAddress(addr+3, temp.b[3]);
    if (error != MEMORY_ERROR_NONE) { return error; };
    return true;
}

bool Onyx1L::Write64(uint64_t addr, uint64_t value) {
    union {
        uint64_t    a;
        uint8_t     b[8];
    } temp;
    bool error = false;
    temp.a = htobe64(value);
    error = memory->WriteAddress(addr, temp.b[0]);
    if (error != MEMORY_ERROR_NONE) { return error; };
    error = memory->WriteAddress(addr+1, temp.b[1]);
    if (error != MEMORY_ERROR_NONE) { return error; };
    error = memory->WriteAddress(addr+2, temp.b[2]);
    if (error != MEMORY_ERROR_NONE) { return error; };
    error = memory->WriteAddress(addr+3, temp.b[3]);
    if (error != MEMORY_ERROR_NONE) { return error; };
    error = memory->WriteAddress(addr+4, temp.b[4]);
    if (error != MEMORY_ERROR_NONE) { return error; };
    error = memory->WriteAddress(addr+5, temp.b[5]);
    if (error != MEMORY_ERROR_NONE) { return error; };
    error = memory->WriteAddress(addr+6, temp.b[6]);
    if (error != MEMORY_ERROR_NONE) { return error; };
    error = memory->WriteAddress(addr+7, temp.b[7]);
    if (error != MEMORY_ERROR_NONE) { return error; };
    return true;
}
