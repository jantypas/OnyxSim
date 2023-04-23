//
// Created by jantypas on 12/31/22.
//

#include "HighMemory.h"
#include <new>
#include <memory.h>
#include <cstdio>
#include <syslog.h>
#include <string>
#include "../../../Configuration/ConfigParameters.h"
#include "../../MemoryConstants.h"
#include <boost/log/trivial.hpp>

HighMemoryError   LinearMemoryErrorTable[] = {
        {   MEMORY_ERROR_NONE,              false,  "No error"                      },
        {   MEMORY_ERROR_ADDRESS_ERROR,     false,  "Memory address violation"     },
        {   MEMORY_ERROR_NOT_INITIALIZED,   false,  "Memory system not initialized" },
        {   MEMORY_ERROR_NO_STORAGE,        true,   "No backing storage available"}
};

bool HighMemory::Init(ConfigParameters *conf, uint32_t pNumPages) {
    BOOST_LOG_TRIVIAL(debug) << "LinearMemory:Init: started";
    numPages = pNumPages;
    try {
        storage = new uint8_t [pNumPages*LOCAL_MEM_PAGE_SIZE];
    } catch (std::bad_alloc &e) {
        ReportError("Init", MEMORY_ERROR_NO_STORAGE);
        return false
    }
    BOOST_LOG_TRIVIAL(debug) << "LinearMemory:Init: Allocating " + std::to_string(pNumPages)+ " pages";
    isActive = true;
    ReportError("Init", MEMORY_ERROR_NONE);
    return true;
};

bool HighMemory::Exit() {
    if (!isActive) {
        ReportError("Exit", MEMORY_ERROR_NOT_INITIALIZED);
        return false;
    } else {
        delete []storage;
        ReportError("Exit", MEMORY_ERROR_NONE);
        return true;
    }
}

bool HighMemory::ReadAddress(uint64_t addr, uint8_t *value) {
    BOOST_LOG_TRIVIAL(debug) << "LinearMemory:ReadAddress: Read("+std::to_string(addr)+")";
    if (!isActive) {
        ReportError("ReadAddress", MEMORY_ERROR_NOT_INITIALIZED);
        return false;
    };
    if (addr / LOCAL_MEM_PAGE_SIZE > numPages) {
        ReportError("ReadAddress", MEMORY_ERROR_ADDRESS_ERROR);
        return false;
    };
    *value = storage[addr];
    BOOST_LOG_TRIVIAL(debug) << "LinearMemory:ReadAddress: Read value "+std::to_string(*value);
    ReportError("ReadAddress", MEMORY_ERROR_NONE);
    return true;
}

bool HighMemory::WriteAddress(uint64_t addr, uint8_t value) {
    BOOST_LOG_TRIVIAL(debug) << "LinearMemory:WriteAddress: Write("+
                                std::to_string(addr)+","+std::to_string(value)+
                                ")";
    if (!isActive) {
        ReportError("WriteAddress", MEMORY_ERROR_NOT_INITIALIZED);
        return false;
    };
    if (addr / LOCAL_MEM_PAGE_SIZE > numPages) {
        ReportError("WriteAddress", MEMORY_ERROR_ADDRESS_ERROR);
        return false;
    };
    storage[addr] = value;
    ReportError("WriteAddress", MEMORY_ERROR_NONE);
    return true;
}

bool HighMemory::LoadPage(uint32_t page, uint8_t *buffer) {
    if (!isActive) {
        ReportError("LoadPage", MEMORY_ERROR_NOT_INITIALIZED);
        return false;
    };
    if (page > numPages) {
        ReportError("LoadPage", MEMORY_ERROR_ADDRESS_ERROR);
        return false;
    };
    memcpy(buffer, &storage[page*LOCAL_MEM_PAGE_SIZE], LOCAL_MEM_PAGE_SIZE);
    ReportError("LoadPage", MEMORY_ERROR_NONE);
    return true;
}

bool HighMemory::SavePage(uint32_t page, uint8_t *buffer) {
    BOOST_LOG_TRIVIAL(debug) << "LinearMemoryh:SavePage: Page: "+std::to_string(page);
    if (!isActive) {
        ReportError("SavePage", MEMORY_ERROR_NOT_INITIALIZED);
        return false;
    };
    if (page > numPages) {
        ReportError("SavePage", MEMORY_ERROR_ADDRESS_ERROR);
        return false;
    };
    memcpy(&storage[page*LOCAL_MEM_PAGE_SIZE], buffer, LOCAL_MEM_PAGE_SIZE);
    ReportError("SavePage", MEMORY_ERROR_NONE);
    return true;
}

HighMemInfo *LinearMemory::GetInfo() {
    ReportError("GetInfo", MEMORY_ERROR_NONE);
    info.numPages       = numPages;
    info.memPageSize    = LOCAL_MEM_PAGE_SIZE;
    return &info;
}

HighMemoryError *LinearMemory::GetLastError() {
    return LastMemoryError;
}

void HighMemory::ReportError(std::string func, uint32_t errornumber) {
    LastMemoryError = &LinearMemoryErrorTable[errornumber];
    if (errornumber == MEMORY_ERROR_NONE) {
        BOOST_LOG_TRIVIAL(debug)
            << "LinearMemory:" + func + ": " + LastMemoryError->msg;
    } else {
        BOOST_LOG_TRIVIAL(error)
            << "LinearMemoryL" + func +": "+ LastMemoryError->msg;
    }
}
