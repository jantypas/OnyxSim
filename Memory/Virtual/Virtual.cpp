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
#define CLR_FLAG(x, y) x &= ~y;
#define SET_FLAG(x, y) x &= y;
#define IS_FLAG_CLEAR(x, y) (x & y) != y
#define IS_FLAG_SET(x, y) (x & y) == y

/**
 * Same here -- this template allows sus to remove all of a given value
 * from a vector.
 */
template <typename T>
    void removeAllFromVector(std::vector <T> &v, const T &target) {
        v.erase(std::remove(v.begin(), v.end(), target), v.end());
    };


template <typename T>
    void insertAtHeadofVector(std::vector <T> &v, const T &target) {
        v.insert(v.begin(), target);
    };

/**
 * MarkPhysicalPageAsFree - Given a page idea, mark it as a free page
 *
 * @param page  - The page we want to free
 * @return      - Did we succeed
 */
bool Virtual::MarkPhysicalPageAsFree(uint32_t page) {\
    // Check the page range
    if (page >= numPhysicalPages) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_ADDRESS_ERROR];
        return false;
    }

    // Make sure that page isn't already free
    if (IS_FLAG_CLEAR(physicalPageTable[page].pageState, PAGE_STATE_IN_USE)) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_FREE_PAGE_ACCESS];
        return false;
    }
    CLR_FLAG(physicalPageTable[page].pageState, PAGE_STATE_IN_USE);
    insertAtHeadofVector(physicalFreePagesList, page);
    removeAllFromVector(physicalUsedPagesList, page);
    return true;
}

/**
 * MarkPhysicalPageAsUsed - Mark a given physical page as in use
 * @param page - The page to use
 * @return - Succcess/failure
 */
bool Virtual::MarkPhysicalPageAsUsed(uint32_t page) {
    if (page >= numPhysicalPages) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_ADDRESS_ERROR];
        return false;
    }
    if (IS_FLAG_SET(physicalPageTable[page].pageState, PAGE_STATE_IN_USE)) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_USED_PAGE_ACCESS];
        return false;
    }
    SET_FLAG(physicalPageTable[page].pageState, PAGE_STATE_IN_USE);
    removeAllFromVector(physicalFreePagesList, page);
    insertAtHeadofVector(physicalUsedPagesList, page);
    return true;
}

/**
 * MarkVirtualPageAsFree - Mark a virtual page as free
 *
 * @param page - The page we want to mark
 * @return - Success/failure
 */
bool Virtual::MarkVirtualPageAsFree(uint32_t page) {
    // Check the page limits
    if (page >= numVirtualPages) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_ADDRESS_ERROR];
        return true;
    }
    // Make sure the page isn't locked or on disk
    uint32_t state = virtualPageTable[page].pageState;
    if (IS_FLAG_SET(state, PAGE_STATE_IS_LOCKED) || IS_FLAG_SET(state, PAGE_STATE_IS_ON_DISK)) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_PAGE_IS_LOCKED];
        return false;
    }
    // Make sure page isn't free
    if (IS_FLAG_CLEAR(state, PAGE_STATE_IN_USE)) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_FREE_PAGE_ACCESS];
        return false;
    }
    // Set the free state
    CLR_FLAG(virtualPageTable[page].pageState, PAGE_STATE_IN_USE);
    // Remove the page from the used list and the lruCache
    removeAllFromVector(virtualUsedPagesList, page);
    removeAllFromVector(lruCache, page);
    // Add to the free list
    insertAtHeadofVector(virtualFreePagesList, page);
    // Free the physical page associated with it
    MarkPhysicalPageAsFree(page);
    // We're done
    return true;
}

/**
 * MarkVirtualPageAsUsed -- Mark a page as used
 *
 * @param page - Page to mark
 * @return - Succecss/failure
 */
bool Virtual::MarkVirtualPageAsUsed(uint32_t page) {
    // Check the page limits
    if (page >= numVirtualPages) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_ADDRESS_ERROR];
        return true;
    }
    // Make sure page isn't in use already
    if (IS_FLAG_SET(virtualPageTable[page].pageState, PAGE_STATE_IN_USE)) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_USED_PAGE_ACCESS];
        return false;
    }
    // Set the free state
    SET_FLAG(virtualPageTable[page].pageState, PAGE_STATE_IN_USE);
    // Remove the page from the free list and move to the used list
    removeAllFromVector(virtualFreePagesList, page);
    insertAtHeadofVector(virtualUsedPagesList, page);
    // Try to get a new physical page
    uint32_t newPage;
    if (AllocateNewPhysicalPage(&newPage)) {
        virtualPageTable[page].physicalPage = newPage;
        return true;
    } else {
        // We can't get a page -- try a swap out
        if (SwapAndAllocatePhysicalPage(&newPage)) {
            virtualPageTable[page].physicalPage = newPage;
            return true;
        } else {
            // Oh well, we trie -- time to die
            LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_NO_PHYSICAL_PAGES];
            return false;
        };
    }
}

/**
 * WritePage to swap
 *
 * @param page to write
 * @return success/failure
 */
bool Virtual::WritePageToSwap(uint32_t page) {
    uint8_t *buffer = &physicalStorage[virtualPageTable[page].physicalPage*MEM_PAGE_SIZE];
    fseek(swapFile, MEM_PAGE_SIZE*page, SEEK_SET);
    fwrite(buffer, MEM_PAGE_SIZE, 1, swapFile);
    return true;
}

/**
 *ReadPage
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