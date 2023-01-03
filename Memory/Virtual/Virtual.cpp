//
// Created by jantypas on 12/31/22.
//

#include "Virtual.h"
#include <vector>
#include <algorithm>
#include <iterator>

/**
 * We need these macros to make our life easier
 */
#define CLR_FLAG(x, y) (x &= ~y )
#define SET_FLAG(x, y) (x &= y )
#define IS_FLAG_SET(x, y) ((x & y) == y)
#define IS_FLAG_CLEAR(x, y) ((x & y) != y)

/**
 * Same here -- this template allows sus to remove all of a given value
 * from a vector.
 */
template <typename T>
    void removeAllFromVecetor(std::vector <T> &v, const T &target) {
        v.erase(std::remove(v.begin(), v.end(), target), v.end());
    };

/**
 * MarkPhysicalPageAsFree - Given a page idea, mark it as a free page
 *
 * @param page  - The page we want to free
 * @return      - Did we succeed
 */
bool Virtual::MarkPhysicalPageAsFree(uint32_t page) {
    // Make sure the page isn't locked or not in use
    if (page >= numPhysicalPages || IS_FLAG_CLEAR(!physicalPageTable[page].pageState, PAGE_STATE_IN_USE)) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_FREE_PAGE_ACCESS];
        return false;
    };
    if (IS_FLAG_SET(physicalPageTable[page].pageState, PAGE_STATE_IS_LOCKED)) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_PAGE_IS_LOCKED];
        return false;
    };
    /**
     * Mark the page as free and put it on the free queue
     * then remove ALL of that page from the used queue
     */
    CLR_FLAG(physicalPageTable[page].pageState, PAGE_STATE_IN_USE);
    physicalFreePagesList.insert(physicalFreePagesList.begin(), page);
    removeAllFromVecetor(physicalUsedPagesList, page);
    return true;
}

/**
 * MarkPhysicalPageAsUsed - Mark a given physical page as in use
 * @param page - The page to use
 * @return - Succcess/failure
 */
bool Virtual::MarkPhysicalPageAsUsed(uint32_t page) {
    /**
     * Check to make sure the page is not locked or already used
     */
    if (page >= numPhysicalPages || IS_FLAG_SET(physicalPageTable[page].pageState, PAGE_STATE_IN_USE)) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_FREE_PAGE_ACCESS];
        return false;
    };
    if (IS_FLAG_SET(physicalPageTable[page].pageState, PAGE_STATE_IS_LOCKED)) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_PAGE_IS_LOCKED];
        return false;
    };
    /**
     * Clear the in use flag
     */
    CLR_FLAG(physicalPageTable[page].pageState, PAGE_STATE_IN_USE);
    physicalUsedPagesList.push_back(page);
    removeAllFromVecetor(physicalFreePagesList, page);
    return true;
}

/**
 * MarkVirtualPageAsFree - Mark a virtual page as free
 * @param page - The page we want to mark
 * @return - Success/failure
 */
bool Virtual::MarkVirtualPageAsFree(uint32_t page) {
    // Make sure the page is valid, is not already free
    if (page >= numVirtualPages || IS_FLAG_CLEAR(virtualPageTable[page].pageState, PAGE_STATE_IN_USE) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_FREE_PAGE_ACCESS];
        return false;
    };
    // Make sure the page isn't locked or on disk'
    if IS_FLAG_SET(virtualPageTable[page].pageState, PAGE_STATE_IS_LOCKED) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_PAGE_IS_LOCKED];
        return false;
    };
    if IS_FLAG_SET(virtualPageTable[page].pageState, PAGE_STATE_IS_ON_DISK) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_PAGE_IS_ON_DISK];
        return false;
    };
    CLR_FLAG(virtualPageTable[page].pageState, PAGE_STATE_IN_USE);
    MarkPhysicalPageAsFree(virtualPageTable[page].physicalPage);
    virtualFreePagesList.push_back(page);
    removeAllFromVecetor(virtualUsedPagesList, page);
    removeAllFromVecetor(lruCache, page);
    return true;
}

/**
 * MarkVirtualPageAsUsed -- Mark a page as used
 *
 * @param page - Page to mark
 * @return - Succecss/failure
 */
bool Virtual::MarkVirtualPageAsUsed(uint32_t page) {
    if IS_FLAG_SET(page >= numVirtualPages || !virtualPageTable[page].pageState, PAGE_STATE_IN_USE) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_FREE_PAGE_ACCESS];
        return false;
    };
    if IS_FLAG_SET(virtualPageTable[page].pageState, PAGE_STATE_IS_LOCKED) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_PAGE_IS_LOCKED];
        return false;
    };
    if IS_FLAG_SET(virtualPageTable[page].pageState, PAGE_STATE_IS_ON_DISK) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_PAGE_IS_ON_DISK];
        return false;
    };
    CLR_FLAG(virtualPageTable[page].pageState, PAGE_STATE_IN_USE);
    MarkPhysicalPageAsFree(virtualPageTable[page].physicalPage);
    virtualFreePagesList.push_back(page);
    removeAllFromVecetor(virtualUsedPagesList, page);
    removeAllFromVecetor(lruCache, page);
    return true;
}

/**
 *
 * @param page
 * @return
 */
bool Virtual::WritePageToSwap(uint32_t page) {
    uint8_t *buffer = &physicalStorage[virtualPageTable[page].physicalPage*MEM_PAGE_SIZE];
    fseek(swapFile, MEM_PAGE_SIZE*page, SEEK_SET);
    fwrite(buffer, MEM_PAGE_SIZE, 1, swapFile);
    return true;
}

/**
 *
 * @param page
 * @return
 */
bool Virtual::ReadPageFromSwap(uint32_t page) {
    uint8_t *buffer = &physicalStorage[virtualPageTable[page].physicalPage*MEM_PAGE_SIZE];
    fseek(swapFile, MEM_PAGE_SIZE*page, SEEK_SET);
    fread(buffer, MEM_PAGE_SIZE, 1, swapFile);
    return true;
}

bool Virtual::InitLinear(uint32_t pNumPages) {
    LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_NOT_IMPLEMENTED];
    return false;
}

bool Virtual::InitBanked(uint32_t pNumMainBanks, uint32_t pBankSize, uint32_t pNumAlternateBanks) {
    LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_NOT_IMPLEMENTED];
    return false;
}

bool Virtual::InitVirtual(uint32_t pNumVirtualPages, uint32_t pNumPhysicalPages, std::string swapFileName) {

    numPhysicalPages = pNumPhysicalPages;
    numVirtualPages = pNumVirtualPages;
    minPhysicalPages = 0;
    minVirtualPages = 0;
    physicalStorage = new uint8_t [MEM_PAGE_SIZE*pNumPhysicalPages];
    virtualPageTable.clear();
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
    if (!virtualPageQueue.PageInUse(reqPage)) {
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
    virtualPageQueue.PutInLRU(reqPage);
    physicalPageQueue.PutInLRU(virtualPageTable[reqPage].physicalPage);
    return true;
}

bool Virtual::WriteAddress(uint64_t addr, uint8_t value) {
    if (!isActive) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_NOT_INITIALIZED];
        return false;
    }
    uint32_t reqPage = addr / MEM_PAGE_SIZE;
    if (!virtualPageQueue.PageInUse(reqPage)) {
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

bool Virtual::SwapInPage(uint32_t page) {
    uint32_t freepage;

    if (physicalPageQueue.FreeSize() <- minPhysicalPages) {
        if (MakeFreePages(&freepage)) {
            virtualPageTable[page].physicalPage = freepage;
            virtualPageTable[page].isOnDisk = false;
            virtualPageTable[page].isDirty = false;
            fseek(swapFile, MEM_PAGE_SIZE*sizeof(uint8_t)*page, SEEK_SET);
            fread(&physicalStorage[MEM_PAGE_SIZE*freepage], MEM_PAGE_SIZE, 1, swapFile);
            physicalPageQueue.MakeUsed(page);

            return true;
        } else {
            LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_CANT_SWAP_OUT_PAGE];
            return false;
        };
    } else {
        physicalPageQueue.MakeUsed(page);
        virtualPageTable[page].physicalPage = freepage;
        virtualPageTable[page].isOnDisk = false;
        virtualPageTable[page].isDirty = false;
        fseek(swapFile, MEM_PAGE_SIZE*sizeof(uint8_t)*page, SEEK_SET);
        fread(&physicalStorage[MEM_PAGE_SIZE*freepage], MEM_PAGE_SIZE, 1, swapFile);
        return true;
    }\
}

bool Virtual::SwapoutPage(uint32_t page) {
    virtualPageTable[page].isOnDisk = true;
    virtualPageTable[page].isDirty = false;
    fseek(swapFile, MEM_PAGE_SIZE*sizeof(uint8_t)*page, SEEK_SET);
    fread(&physicalStorage[MEM_PAGE_SIZE*page], MEM_PAGE_SIZE, 1, swapFile);\
    physicalPageQueue.MakeFree(page);
    return true;
}

bool Virtual::MakeFreePages(uint32_t *page) {
    uint32_t possiblePage, ix;

    while (physicalStorage)

    return false;
}

bool Virtual::FreeNPages(uint32_t pPages, uint32_t *pPageList) {
    return true;
}