//
// Created by jantypas on 12/31/22.
//

#include "LinearMemory.h"
#include <memory.h>

bool LinearMemory::InitLinear(ConfigParameters *conf, uint32_t pNumPages) {
    numPages = pNumPages;
    storage = new uint8_t [pNumPages*MEM_PAGE_SIZE];
    isActive = true;
    return true;
};

bool LinearMemory::InitBanked(ConfigParameters *conf, uint32_t pNumMainBanks, uint32_t pBankSize, uint32_t pNumAlternateBanks) {
    LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_NOT_IMPLEMENTED];
    return false;
}

bool LinearMemory::InitVirtual(ConfigParameters *conf, uint32_t pNumVirtualPages, uint32_t pNumPhysicalPages, std::string swapFileName) {
    LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_NOT_IMPLEMENTED];
    return false;
}

bool LinearMemory::GetBank(uint8_t *bank) {
    LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_NOT_IMPLEMENTED];
    return false;
}

bool LinearMemory::SetBank(uint8_t bank) {
    LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_NOT_IMPLEMENTED];
    return false;
}

bool LinearMemory::Exit() {
    if (!isActive) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_NOT_INITIALIZED];
        return false;
    } else {
        delete []storage;
        return true;
    }
}

bool LinearMemory::ReadAddress(uint64_t addr, uint8_t *value) {
    if (!isActive) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_NOT_IMPLEMENTED];
        return false;
    };
    if (addr / MEM_PAGE_SIZE > numPages) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_ADDRESS_ERROR];
        return false;
    };
    *value = storage[addr];
    return true;
}

bool LinearMemory::WriteAddress(uint64_t addr, uint8_t value) {
    if (!isActive) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_NOT_IMPLEMENTED];
        return false;
    };
    if (addr / MEM_PAGE_SIZE > numPages) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_ADDRESS_ERROR];
        return false;
    };
    storage[addr] = value;
    return true;
}

bool LinearMemory::LoadPage(uint32_t page, uint8_t *buffer) {
    if (!isActive) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_NOT_IMPLEMENTED];
        return false;
    };
    if (page > numPages) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_ADDRESS_ERROR];
        return false;
    };
    memcpy(buffer, &storage[page*MEM_PAGE_SIZE], MEM_PAGE_SIZE);
    return true;
}

bool LinearMemory::SavePage(uint32_t page, uint8_t *buffer) {
    if (!isActive) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_NOT_IMPLEMENTED];
        return false;
    };
    if (page > numPages) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_ADDRESS_ERROR];
        return false;
    };
    memcpy(&storage[page*MEM_PAGE_SIZE], buffer, MEM_PAGE_SIZE);
    return true;
}

bool LinearMemory::AllocateNPages(uint32_t pPages, std::vector<uint32_t> &pPagelist) {
    LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_NOT_IMPLEMENTED];
    return false;
}

bool LinearMemory::SwapInPage(uint32_t page) {
    LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_NOT_IMPLEMENTED];
    return false;
}

bool LinearMemory::SwapOutPage(uint32_t page) {
    LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_NOT_IMPLEMENTED];
    return false;
}

bool LinearMemory::FreeNPages(uint32_t pPages, uint32_t *pPageList) {
    LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_NOT_IMPLEMENTED];
    return false;
}

MemoryInfo LinearMemory::GetInfo() {
    MemoryInfo info;

    info.memoryMode = MEMORY_LINEAR;
    info.memoryTypeName = "linear";
    info.Info.linfo.numPages = numPages;
    return info;
}