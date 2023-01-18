//
// Created by jantypas on 1/5/23.
//

#ifndef ONYXSIM_SWAPPER_H
#define ONYXSIM_SWAPPER_H
#include <stdint-gcc.h>
#include <cstdio>
#include <string>
#include <utility>
#include "../../MemoryConstants.h"

struct SwapperInfo {
    std::string     swapFileName;
    uint32_t        swapIns;
    uint32_t        swapOuts;
};

class Swapper {
private :
    FILE            *swapFileDesc;
    SwapperInfo     info;
public :
    bool Init(std::string pName) {
        info.swapFileName = std::move(pName);
        swapFileDesc = fopen(info.swapFileName.c_str(), "rw");
        return true;
    };
    ~Swapper() {
        fclose(swapFileDesc);
    };
    bool ReadPageFromPage(uint32_t page, uint8_t *buffer) {
        fseek(swapFileDesc, LOCAL_MEM_PAGE_SIZE*page, SEEK_SET);
        fread(buffer, LOCAL_MEM_PAGE_SIZE*page, 1, swapFileDesc);
        return true;
    };
    bool WritePageToSwap(uint32_t page, uint8_t *buffer) {
        fseek(swapFileDesc, LOCAL_MEM_PAGE_SIZE*page, SEEK_SET);
        fwrite(buffer, LOCAL_MEM_PAGE_SIZE*page, 1, swapFileDesc);
        return true;
    }
    SwapperInfo *GetInfo() {
        return &info;
    }
};


#endif //ONYXSIM_SWAPPER_H
