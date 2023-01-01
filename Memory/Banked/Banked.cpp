//
// Created by jantypas on 12/31/22.
//

#include "Banked.h"
#include <memory.h>

bool Banked::InitLinear(uint32_t pNumPages) {
    LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_NOT_IMPLEMENTED];
    return false;
}

bool Banked::InitVirtual(uint32_t pNumVirtualPages, uint32_t pNumPhysicalPages, std::string swapFileName) {
    LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_NOT_IMPLEMENTED];
    return false;
}

bool Banked::InitBanked(uint32_t pNumMainBanks, uint32_t pBankSize, uint32_t pNumAlternateBanks) {
    uint8_t ix;
    *mainStorage = new uint8_t[pNumMainBanks];
    for (ix = 0; ix < pNumMainBanks; ix++) {
        if (ix != pNumMainBanks-2) {
            mainStorage[ix] = new uint8_t[pBankSize];
        };
    };
    *alternateStorage = new uint8_t[pNumMainBanks];
    for (ix = 0; ix < pNumMainBanks; ix++) {
        alternateStorage[ix] = new uint8_t[pBankSize];
    };
    mainStorage[pNumMainBanks-2] = alternateStorage[0];
    activeBank = 0;
    numMainBanks = pNumMainBanks;
    numAlternateBanks = pNumAlternateBanks;
    bankSize = pBankSize;
    return true;
}

bool Banked::Exit() {
    uint8_t ix;
    if (!isActive) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_NOT_INITIALIZED];
        return false;
    };
    for (ix = 0; ix < numMainBanks; ix++) {
        if (ix != numMainBanks-2) {
            delete []mainStorage[ix];
        };
    };
    delete []mainStorage;
    for (ix = 0; ix < numAlternateBanks; ix++) {
        if (ix != numAlternateBanks) {
            delete [] alternateStorage[ix];
        };
    };
    delete []alternateStorage;
    isActive = false;
    return true;
}

bool Banked::ReadAddress(uint64_t addr, uint8_t *value) {
    if (!isActive) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_NOT_INITIALIZED];
        return false;
    };
    if (addr / MEM_PAGE_SIZE > numMainBanks*bankSize) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_ADDRESS_ERROR];
        return false;
    };
    *value = mainStorage[addr/MEM_PAGE_SIZE][addr % MEM_PAGE_SIZE];
    return true;
}

bool Banked::WriteAddress(uint64_t addr, uint8_t value) {
    if (!isActive) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_NOT_INITIALIZED];
        return false;
    };
    if (addr / MEM_PAGE_SIZE > numMainBanks*bankSize) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_ADDRESS_ERROR];
        return false;
    };
    mainStorage[addr/MEM_PAGE_SIZE][addr % MEM_PAGE_SIZE] = value;
    return true;
}

bool Banked::LoadPage(uint32_t page, uint8_t *buffer) {
    if (!isActive) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_NOT_INITIALIZED];
        return false;
    };
    if (page > numMainBanks*bankSize) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_ADDRESS_ERROR];
        return false;
    }
    memcpy(mainStorage[page], buffer, MEM_PAGE_SIZE);
    return true;
}

bool Banked::SavePage(uint32_t page, uint8_t *buffer) {
    if (!isActive) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_NOT_INITIALIZED];
        return false;
    };
    if (page > numMainBanks*bankSize) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_ADDRESS_ERROR];
        return false;
    }
    memcpy(buffer, mainStorage[page], MEM_PAGE_SIZE);
    return true;
}

bool Banked::SetBank(uint8_t bank) {
    if (bank >= numAlternateBanks) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_ADDRESS_ERROR];
        return false;
    }
    mainStorage[numMainBanks-2] = alternateStorage[bank];
    activeBank = bank;
    return true;
}

bool Banked::GetBank(uint8_t *bank) {
    *bank - activeBank;
    return true;
}

bool Banked::AllocateNPages(uint32_t pPages, uint32_t *pPagelist) {
    LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_NOT_IMPLEMENTED];
    return false;
}

bool Banked::Swapout(uint32_t page) {
    LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_NOT_IMPLEMENTED];
    return false;
}

bool Banked::SwapIn(uint32_t page) {
    LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_NOT_IMPLEMENTED];
    return false;
}