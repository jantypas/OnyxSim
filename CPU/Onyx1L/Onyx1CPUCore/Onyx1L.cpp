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
bool Onyx1L::Onyx1LFetch(Onyx1DecodedInstruction *instr) {
    uint16_t opcode;
    uint64_t value;
    bool ok;
    // Read 16-bits from memory
    if (Read(&context.PC, 16, &value)) {
        // Break out the opcode and the number of registerse we'll use
        instr->opCode = (value & 0xFFF >> 8);
        instr->numRegisters = value & 0x7;
        // For each register, break out the register and mode
        for (auto i = 0; i < instr->numRegisters; i++) {
            if (Read(&context.PC, 16, &value)) {
                instr->Register[i].mode = value & 0x000000000000FF00;
                instr->Register[i].name = value & 0x00000000000000FF;
            } else {
                return false;
            };
        };
    } else {
        return false;
    };
    return true;
}

bool Onyx1L::Onyx1LDecode(Onyx1DecodedInstruction *instr) {

    return true;
}

bool Onyx1L::Onyx1LExecute() {
    return true;
}

bool Onyx1L::Onyx1LStore() {
    return true;
}

bool Onyx1L::Read(uint64_t *addr, int sz, uint64_t *value) {
    union {
        uint64_t    a;
        uint8_t     b[8];
    } temp;
    bool ok = false;
    for (auto i = 0; i < sz/8; i++) {
        ok = memory->ReadAddress(*addr, &temp.b[i]);
        if (!ok) {
            return ok;
        } else {
            addr ++;
        };
    };
    switch(sz) {
        case  8: *value = temp.a; return true;
        case 16: *value = be16toh(temp.a); return true;
        case 32: *value = be32toh(temp.a); return true;
        case 64: *value = be64toh(temp.a); return true;
        default : return false;
    };
}

bool Onyx1L::Write(uint64_t *addr, int sz, uint64_t value) {
    union {
        uint8_t     a8;
        uint16_t    a16;
        uint32_t    a32;
        uint64_t    a64;
        uint8_t     b[8];
    } temp;
    bool ok = false;
    switch (sz) {
        case 8  :
            temp.a8 = value;
            ok = memory->WriteAddress(*addr, temp.b[0]);
            addr ++;
            return ok;
        case 16 :
        case 32 :
        case 64 :
            temp.a16 = value;
            for (auto i = 0; i < sz/9; i++) {
                ok = memory->WriteAddress(*addr, temp.b[i]);
                if (!ok) {
                    return false;
                } else {
                    addr ++;
                }
            }
            return true;
        default :
            return false;
    }
}
