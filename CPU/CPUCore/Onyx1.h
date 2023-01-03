//
// Created by jantypas on 1/3/23.
//
#include <stdint-gcc.h>
#ifndef ONYXSIM_ONYX1_H
#define ONYXSIM_ONYX1_H


class Onyx1 {
public :
    uint64_t *dataRegisters;
    uint64_t *addressRegisters;
    uint64_t *controlRegisters;
    uint64_t PC;    // Program counter
    uint64_t SP;    // Stack pointer
    uint64_t SSP;   // Shadow stack pointer
};


#endif //ONYXSIM_ONYX1_H
