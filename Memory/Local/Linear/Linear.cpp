//
// Created by jantypas on 12/31/22.
//

#include "Linear.h"
#include <memory.h>

bool Linear::InitLinear(uint32_t pNumPages) {
    numPages = pNumPages;
    storage = new uint8_t [pNumPages*MEM_PAGE_SIZE];
    isActive = true;
    return true;
};

bool Linear::InitBanked(uint32_t pNumMainBanks, uint32_t pBankSize, uint32_t pNumAlternateBanks) {
    LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_NOT_IMPLEMENTED];
    return false;
}

bool Linear::InitVirtual(uint32_t pNumVirtualPages, uint32_t pNumPhysicalPages, std::string swapFileName) {
    LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_NOT_IMPLEMENTED];
    return false;
}

bool Linear::GetBank(uint8_t *bank) {
    LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_NOT_IMPLEMENTED];
    return false;
}

bool Linear::SetBank(uint8_t bank) {
    LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_NOT_IMPLEMENTED];
    return false;
}

bool Linear::Exit() {
    if (!isActive) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_NOT_INITIALIZED];
        return false;
    } else {
        delete []storage;
        return true;
    }
}

bool Linear::ReadAddress(uint64_t addr, uint8_t *value) {
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

bool Linear::WriteAddress(uint64_t addr, uint8_t value) {
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

bool Linear::LoadPage(uint32_t page, uint8_t *buffer) {
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

bool Linear::SavePage(uint32_t page, uint8_t *buffer) {
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

bool Linear::AllocateNPages(uint32_t pPages, uint32_t *pPagelist) {
    LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_NOT_IMPLEMENTED];
    return false;
}

bool Linear::SwapInPage(uint32_t page) {
    LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_NOT_IMPLEMENTED];
    return false;
}

bool Linear::SwapOutPage(uint32_t page) {
    LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_NOT_IMPLEMENTED];
    return false;
}