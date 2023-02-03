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
#include <boost/log/trivial.hpp>

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
        BOOST_LOG_TRIVIAL(debug) << "Swapper: Started";
        info.swapFileName = std::move(pName);
        swapFileDesc = fopen(info.swapFileName.c_str(), "w+");
        if (swapFileDesc != nullptr) {
            BOOST_LOG_TRIVIAL(debug) << "Swapper: Opened swap file <"+info.swapFileName;
            return true;
        } else {
            BOOST_LOG_TRIVIAL(error) << "Swapper: Unable to open swap file";
            std::string err = strerror(errno);
            BOOST_LOG_TRIVIAL(error) << "Swapper: fopen returns "+err;
            return false;
        }
    };
    ~Swapper() {
        BOOST_LOG_TRIVIAL(debug) << "Swapper: Closed swap file";
        fclose(swapFileDesc);
    };
    bool ReadPageFromSwap(uint32_t page, uint8_t *buffer) {
        BOOST_LOG_TRIVIAL(debug) << "Swapper:ReadPage("+std::to_string(page)+")";
        fseek(swapFileDesc, LOCAL_MEM_PAGE_SIZE*page, SEEK_SET);
        fread(buffer, LOCAL_MEM_PAGE_SIZE*page, 1, swapFileDesc);
        return true;
    };
    bool WritePageToSwap(uint32_t page, uint8_t *buffer) {
        BOOST_LOG_TRIVIAL(debug) << "Swapper:WritePage("+std::to_string(page)+")";
        if (swapFileDesc == nullptr) {
            BOOST_LOG_TRIVIAL(error) << "Swapper:WritePage: Swapfile descriptor is NULL";
            return false;
        }
        fseek(swapFileDesc, LOCAL_MEM_PAGE_SIZE*page, SEEK_SET);
        fwrite(buffer, LOCAL_MEM_PAGE_SIZE*page, 1, swapFileDesc);
        return true;
    }
    SwapperInfo *GetInfo() {
        return &info;
    }
};


#endif //ONYXSIM_SWAPPER_H
