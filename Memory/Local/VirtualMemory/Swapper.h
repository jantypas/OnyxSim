//
// Created by jantypas on 1/5/23.
//

#ifndef ONYXSIM_SWAPPER_H
#define ONYXSIM_SWAPPER_H
#include <stdint-gcc.h>
#include <cstdio>
#include <string>
#include <utility>
#include "../../MemInterface.h"

class Swapper {
private :
    FILE            *swapFileDesc;
    std::string     name;
public :
    bool Init(std::string pName) {
        name = std::move(pName);
        swapFileDesc = fopen(name.c_str(), "rw");
        return true;
    };
    ~Swapper() {
        fclose(swapFileDesc);
    };
    bool ReadPageFromPage(uint32_t page, uint8_t *buffer) {
        fseek(swapFileDesc, MEM_PAGE_SIZE*page, SEEK_SET);
        fread(buffer, MEM_PAGE_SIZE*page, 1, swapFileDesc);
        return true;
    };
    bool WritePageToSwap(uint32_t page, uint8_t *buffer) {
        fseek(swapFileDesc, MEM_PAGE_SIZE*page, SEEK_SET);
        fwrite(buffer, MEM_PAGE_SIZE*page, 1, swapFileDesc);
        return true;
    }
};


#endif //ONYXSIM_SWAPPER_H
