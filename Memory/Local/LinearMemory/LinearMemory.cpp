//
// Created by jantypas on 12/31/22.
//

#include "LinearMemory.h"
#include <memory.h>
#include "../../../Logger/Logger.h"
#include <cstdio>
#include <syslog.h>
#include <string>
#include "../../../Configuration/ConfigParameters.h"
#include "../../MemoryConstants.h"

LinearMemoryError   LinearMemoryErrorTable[] = {
        {   MEMORY_ERROR_NONE,              false,  "No error"                      },
        {   MEMORY_ERROR_ADDRESS_ERROR,     false,  "Memory address violation"     },
        {   MEMORY_ERROR_NOT_INITIALIZED,   false,  "Memory system not initialized" }
};

bool LinearMemory::Init(ConfigParameters *conf, uint32_t pNumPages) {
    logger->Message(LOG_DEBUG, "Init: started");
    numPages = pNumPages;
    storage = new uint8_t [pNumPages*LOCAL_MEM_PAGE_SIZE];
    logger->Message(LOG_DEBUG, "Init: Allocating " + std::to_string(pNumPages)+ " pages");
    isActive = true;
    logger->Message(LOG_DEBUG, "Init: Code returns success(true)");
    return true;
};

bool LinearMemory::Exit() {
    if (!isActive) {
        logger->Message(LOG_DEBUG, "Exit: Not initialized");
        LastMemoryError = &LinearMemoryErrorTable[MEMORY_ERROR_NOT_INITIALIZED];
        return false;
    } else {
        delete []storage;
        logger->Message(LOG_DEBUG, "Exit: Success");
        return true;
    }
}

bool LinearMemory::ReadAddress(uint64_t addr, uint8_t *value) {
    logger->Message(LOG_DEBUG, "ReadAddress: Read("+std::to_string(addr)+")");
    if (!isActive) {
        logger->Message(LOG_ERR, "ReadAddress: Memory not initialized");
        LastMemoryError = &LinearMemoryErrorTable[MEMORY_ERROR_NOT_INITIALIZED];
        return false;
    };
    if (addr / LOCAL_MEM_PAGE_SIZE > numPages) {
        logger->Message(LOG_ERR, "ReadAddress: address outside of "+std::to_string(numPages));
        LastMemoryError = &LinearMemoryErrorTable[MEMORY_ERROR_ADDRESS_ERROR];
        return false;
    };
    *value = storage[addr];
    logger->Message(LOG_DEBUG, "ReadAddress: Succeeded - Value "+std::to_string(*value));
    return true;
}

bool LinearMemory::WriteAddress(uint64_t addr, uint8_t value) {
    logger->Message(LOG_DEBUG, "WriteAddress: Write("+
                std::to_string(addr)+","+std::to_string(value)+
                ")");
    if (!isActive) {
        logger->Message(LOG_ERR, "WriteAddress: Memory system not active");
        LastMemoryError = &LinearMemoryErrorTable[MEMORY_ERROR_NOT_INITIALIZED];
        return false;
    };
    if (addr / LOCAL_MEM_PAGE_SIZE > numPages) {
        logger->Message(LOG_ERR, "WriteAddress: address outside of "+std::to_string(numPages));
        LastMemoryError = &LinearMemoryErrorTable[MEMORY_ERROR_ADDRESS_ERROR];
        return false;
    };
    storage[addr] = value;
    logger->Message(LOG_DEBUG, "WriteAddress: Success");
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
    logger->Message(LOG_DEBUG, "SavePage: Page: "+std::to_string(page));
    if (!isActive) {
        logger->Message(LOG_ERR, "SavePage: Memory system not active");
        LastMemoryError = &LinearMemoryErrorTable[MEMORY_ERROR_NOT_INITIALIZED];
        return false;
    };
    if (page > numPages) {
        logger->Message(LOG_ERR,
                        "SavePage: "+std::to_string(page)+" >= "+std::to_string(numPages));
        LastMemoryError = &LinearMemoryErrorTable[MEMORY_ERROR_ADDRESS_ERROR];
        return false;
    };
    memcpy(&storage[page*LOCAL_MEM_PAGE_SIZE], buffer, LOCAL_MEM_PAGE_SIZE);
    logger->Message(LOG_DEBUG,"SavePage: Success");
    return true;
}

LinearMemInfo *LinearMemory::GetInfo() {
    logger->Message(LOG_DEBUG, "GetInfo: Success");
    info.numPages       = numPages;
    info.memPageSize    = LOCAL_MEM_PAGE_SIZE;
    return &info;
}

LinearMemoryError *LinearMemory::GetLastError() {
    logger->Message(LOG_DEBUG,"GetError: Success");
    return LastMemoryError;
}