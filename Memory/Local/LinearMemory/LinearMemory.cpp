//
// Created by jantypas on 12/31/22.
//

#include "LinearMemory.h"
#include <memory.h>
#include "../../../Logger/Syslog.h"
#include <cstdio>
#include <syslog.h>
#include <string>
#include "../../../Configuration/ConfigParameters.h"
#include "../../MemoryConstants.h"

LinearMemoryError   LinearMemoryErrorTable[] = {
        {   MEMORY_ERROR_NONE,              false,  "No error"                      },
        {   MEMORY_ERROR_ADDRESS_ERROR,     false,  "Memory adddress violation"     },
        {   MEMORY_ERROR_NOT_INITIALIZED,   false,  "Memory system not initialized" }
};

bool LinearMemory::Init(ConfigParameters *conf, uint32_t pNumPages) {
    logger.Message(LOG_DEBUG, "LinearMemory::InitLinear: started");
    numPages = pNumPages;
    storage = new uint8_t [pNumPages*LOCAL_MEM_PAGE_SIZE];
    logger.Message(LOG_DEBUG, "LinearMemory::InitLinear: Allocating " + std::to_string(pNumPages)+ " pages");
    isActive = true;
    logger.Message(LOG_DEBUG, "LinearMemory::InitLinear: Code returns success(true)");
    return true;
};

bool LinearMemory::Exit() {
    if (!isActive) {
        LastMemoryError = &LinearMemoryErrorTable[MEMORY_ERROR_NOT_INITIALIZED];
        return false;
    } else {
        delete []storage;
        return true;
    }
}

bool LinearMemory::ReadAddress(uint64_t addr, uint8_t *value) {
    if (!isActive) {
        LastMemoryError = &LinearMemoryErrorTable[MEMORY_ERROR_NOT_INITIALIZED];
        return false;
    };
    if (addr / LOCAL_MEM_PAGE_SIZE > numPages) {
        LastMemoryError = &LinearMemoryErrorTable[MEMORY_ERROR_ADDRESS_ERROR];
        return false;
    };
    *value = storage[addr];
    return true;
}

bool LinearMemory::WriteAddress(uint64_t addr, uint8_t value) {
    if (!isActive) {
        LastMemoryError = &LinearMemoryErrorTable[MEMORY_ERROR_NOT_INITIALIZED];
        return false;
    };
    if (addr / LOCAL_MEM_PAGE_SIZE > numPages) {
        LastMemoryError = &LinearMemoryErrorTable[MEMORY_ERROR_ADDRESS_ERROR];
        return false;
    };
    storage[addr] = value;
    return true;
}

bool LinearMemory::LoadPage(uint32_t page, uint8_t *buffer) {
    if (!isActive) {
        LastMemoryError = &LinearMemoryErrorTable[MEMORY_ERROR_NOT_INITIALIZED];
        return false;
    };
    if (page > numPages) {
        LastMemoryError = &LinearMemoryErrorTable[MEMORY_ERROR_ADDRESS_ERROR];
        return false;
    };
    memcpy(buffer, &storage[page*LOCAL_MEM_PAGE_SIZE], LOCAL_MEM_PAGE_SIZE);
    return true;
}

bool LinearMemory::SavePage(uint32_t page, uint8_t *buffer) {
    if (!isActive) {
        LastMemoryError = &LinearMemoryErrorTable[MEMORY_ERROR_NOT_INITIALIZED];
        return false;
    };
    if (page > numPages) {
        LastMemoryError = &LinearMemoryErrorTable[MEMORY_ERROR_ADDRESS_ERROR];
        return false;
    };
    memcpy(&storage[page*LOCAL_MEM_PAGE_SIZE], buffer, LOCAL_MEM_PAGE_SIZE);
    return true;
}

LinearMemInfo *LinearMemory::GetInfo() {
    info.numPages       = numPages;
    info.memPageSize    = LOCAL_MEM_PAGE_SIZE;
    return &info;
}

LinearMemoryError *LinearMemory::GetLastError() {
    return LastMemoryError;
}