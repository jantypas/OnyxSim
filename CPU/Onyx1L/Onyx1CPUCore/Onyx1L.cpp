//
// Created by jantypas on 1/3/23.
//

#include "Onyx1L.h"

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