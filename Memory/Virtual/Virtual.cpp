//
// Created by jantypas on 12/31/22.
//

#include "Virtual.h"
#include <stdio.h>

bool Virtual::InitLinear(uint32_t pNumPages) {
    LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_NOT_IMPLEMENTED];
    return false;
}

bool Virtual::InitBanked(uint32_t pNumMainBanks, uint32_t pBankSize, uint32_t pNumAlternateBanks) {
    LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_NOT_IMPLEMENTED];
    return false;
}

bool Virtual::InitVirtual(uint32_t pNumVirtualPages, uint32_t pNumPhysicalPages, std::string swapFileName) {
    uint32_t ix;

    numFreeVirtualPages = pNumVirtualPages;
    numFreePhysicalPages = pNumPhysicalPages;
    numUsedVirtualPages = 0;
    numUsedPhysicalPages = 0;
    minPhysicalPages = 0;
    minVirtualPages = 0;
    physicalStorage = new uint8_t [MEM_PAGE_SIZE*pNumPhysicalPages];
    virtualPageTable.clear();
    for (ix = 0; ix < numFreePhysicalPages; ix++) {
        physicalPageBitmap[ix] = false;
    };
    for (ix = 0; ix < numFreeVirtualPages; ix++) {
        virtualPageBitmap[ix] = false;
    };
    swapFile = fopen(swapName, "rw");
    isActive = true;
    return true;
}

bool Virtual::Exit() {
    if (!isActive) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_NOT_INITIALIZED];
        return false;
    }
    fclose(swapFile);
    delete physicalStorage;
    return true;
}

bool Virtual::ReadAddress(uint64_t addr, uint8_t *value) {
    if (!isActive) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_NOT_INITIALIZED];
        return false;
    }
    uint32_t reqPage = addr / MEM_PAGE_SIZE;
    if (!virtualPageBitmap[reqPage]) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_FREE_PAGE_ACCESS];
        return false;
    }
    if (!virtualPageTable[reqPage].isInUse) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_FREE_PAGE_ACCESS];
        return false;
    }
    if (virtualPageTable[reqPage].isOnDisk) {
        if (!SwapIn(reqPage)) {
            LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_CANT_SWAP_IN_PAGE];
            return false;
        }
    }
    uint32_t physPag = virtualPageTable[reqPage].physicalPage;
    *value = physicalStorage[physPag*MEM_PAGE_SIZE+(addr % MEM_PAGE_SIZE)];
    virtualPageTable[reqPage].lastUsed = true;
    return true;
}

bool Virtual::WriteAddress(uint64_t addr, uint8_t value) {
    if (!isActive) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_NOT_INITIALIZED];
        return false;
    }
    uint32_t reqPage = addr / MEM_PAGE_SIZE;
    if (!virtualPageBitmap[reqPage]) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_FREE_PAGE_ACCESS];
        return false;
    }
    if (!virtualPageTable[reqPage].isInUse) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_FREE_PAGE_ACCESS];
        return false;
    }
    if (virtualPageTable[reqPage].isOnDisk) {
        if (!SwapIn(reqPage)) {
            LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_CANT_SWAP_IN_PAGE];
            return false;
        }
    }
    uint32_t physPag = virtualPageTable[reqPage].physicalPage;
    physicalStorage[physPag*MEM_PAGE_SIZE+(addr % MEM_PAGE_SIZE)] = value;
    virtualPageTable[reqPage].isDirty = true;
    virtualPageTable[reqPage].lastUsed = true;
    return true;
}

bool Virtual::LoadPage(uint32_t page, uint8_t *buffer) {
    return true;
}

bool Virtual::SavePage(uint32_t page, uint8_t *buffer) {
    return true;
}

bool Virtual::SetBank(uint8_t bank) {
    LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_NOT_IMPLEMENTED];
    return false;
}

bool Virtual::GetBank(uint8_t *bank) {
    LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_NOT_IMPLEMENTED];
    return false;
}

bool Virtual::AllocateNPages(uint32_t pPages, uint32_t *pPagelist) {
    return true;
}

bool Virtual::SwapIn(uint32_t page) {
    return true;
}

bool Virtual::Swapout(uint32_t page) {
    return true;
}