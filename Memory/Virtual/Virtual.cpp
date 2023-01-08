//
// Created by jantypas on 12/31/22.
//

#include "Virtual.h"
#include <vector>
#include <memory.h>
#include <algorithm>
#include <iterator>

/**
 * We need these macros to make our life easier
 * Some inline defines to manage flags rather than compiled functions and
 * a couple of templates for things we're going to do over and over again.
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

/**********************************************************************************************
 *
 * Internal routines
 *
 * These are routines our public methods will use
 *********************************************************************************************/
/**
 * findFreePagesFromTheLRU -- Look into the LRU Cache to see if we have any
 *                            candidates for swapping
 * @param pages - Return a list of possible pages
 * @return success/failure boolean
 *
 */
bool Virtual::findFreePagesFromTheLRU(std::vector<uint32_t> &pages) {
    // No candidates?
    if (lruCache.empty()) { return false; }
    /**
     * Get tne number of desired pages.
     * Make sure these pages are not locked or on disk
     */
     uint32_t pg = 0;
     uint32_t minimalPages;
     if (lruCache > MIN_SWAPPABLE_PAGES) {
         minimalPages = MIN_SWAPPABLE_PAGES;
     } else {
        minimalPages = 1;
     }
     auto  ix = lruCache.end();
     while ((pg < minimalPages) && (!lruCache.empty())) {
         uint32_t possiblePage = *ix;
         uint16_t state;
         state = virtualPageTable[possiblePage].pageState;
         if (IS_FLAG_CLEAR(state, PAGE_STATE_IS_LOCKED) && IS_FLAG_CLEAR(state, PAGE_STATE_IS_ON_DISK)) {
             lruCache.erase(ix);
             ix --;
             pages.push_back(possiblePage);
             minimalPages ++;
         }
     }
    return true;
}

/**
 * markPhysicalPagesAsFree -- Given a physical page, mark it as free
 * @param page  - The page to free
 * @return Success/failure
 */
bool Virtual::markPhysicalPageAsFree(uint32_t page) {
    // Make sure our page is within range
    if (page >= numPhysicalPages) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_ADDRESS_ERROR];
        return false;
    }
    // Make sure page isn't already free
    if (IS_FLAG_CLEAR(physicalPageTable[page].pageState, PAGE_STATE_IN_USE)) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_FREE_PAGE_ACCESS];
        return false;
    }
    // Clear the page
    CLR_FLAG(virtualPageTable[page].pageState, PAGE_STATE_IN_USE);
    physicalFreePagesList.push_back(page);
    removeAllFromVector(physicalUsedPagesList, page);
    removeAllFromVector(lruCache, page);
    return true;
}

/**
 * markPhysicalPagesAsUsed -- Given a physical page, mark it as used
 * @param page  - The page to use
 * @return Success/failure
 */
bool Virtual::markPhysicalPageAsUsed(uint32_t page) {
    // Make sure our page is in range
    if (page >= numPhysicalPages) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_ADDRESS_ERROR];
        return false;
    }
    // Make sure page isn't already in use
    if (IS_FLAG_SET(physicalPageTable[page].pageState, PAGE_STATE_IN_USE)) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_USED_PAGE_ACCESS];
        return true;
    }
    // Make the page as used
    SET_FLAG(physicalPageTable[page].pageState, PAGE_STATE_IN_USE);
    removeAllFromVector(physicalFreePagesList, page);
    physicalUsedPagesList.push_back(page);
    return true;
}

/**
 * markVirtualPageAsFree -- Mark a virtual page as free
 * @param page - page to free
 * @return Success/failure
 */
bool Virtual::markVirtualPageAsFree(uint32_t page) {
    // Make sure page is in range
    if (page >= numVirtualPages) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_ADDRESS_ERROR];
        return false;
    }
    // Make sure the page isn't already free
    if (IS_FLAG_CLEAR(virtualPageTable[page].pageState, PAGE_STATE_IN_USE)) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_FREE_PAGE_ACCESS];
        return false;
    }
    // Mark the page as free, but first get the physical page, we need to free that first
    uint32_t pg = virtualPageTable[page].physicalPage;
    markPhysicalPageAsFree(pg);
    CLR_FLAG(virtualPageTable[page].pageState, PAGE_STATE_IN_USE);
    removeAllFromVector(virtualUsedPagesList, page);
    removeAllFromVector(lruCache, page);
    // Put on the free list
    physicalFreePagesList.push_back(page);
    return true;
}

/**
 *
 * @param page
 * @return
 */
bool Virtual::markVirtualPageAsUsed(uint32_t page, uint32_t physPage) {
    // Make sure page is within range
    if (page >= numVirtualPages) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_ADDRESS_ERROR];
        return false;
    }
    // Make sure the page isn't already in use
    if (IS_FLAG_SET(virtualPageTable[page].pageState, PAGE_STATE_IN_USE)) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_USED_PAGE_ACCESS];
        return false;
    }
    // Mark the virtual page as used
    virtualPageTable[page].physicalPage = physPage;
    virtualUsedPagesList.push_back(page);
    removeAllFromVector(virtualFreePagesList, page);
    return true;
}

bool Virtual::SwapOutPageList(std::vector<uint32_t> &list) {
    std::vector<uint32_t>::iterator ix;

    for (ix = list.begin(); ix != list.end(); ix++) {
        SwapOutPage(*ix);
    }
    return true;
}

bool Virtual::SwapInPageList() {
    std::vector<uint32_t> list;
    if (findFreePagesFromTheLRU(list)) {
        if (SwapOutPageList(list)) {
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

/***********************************************************************************************************
 *
 * Main routines
 *
 ******************************************************************************************************/

/**
 *
 * @param pNumPages
 * @return
 */
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

    numVirtualPages         = pNumVirtualPages;
    numPhysicalPages        = pNumPhysicalPages;
    physicalStorage         = new uint8_t[numPhysicalPages * MEM_PAGE_SIZE];
    std::fill(physicalFreePagesList.begin(), physicalFreePagesList.end(), ix);
    std::fill(virtualFreePagesList.begin(), virtualFreePagesList.end(),ix);
    swapper.Init(Configuration.SwapFileName);
    return true;
}

bool Virtual::Exit() {
    if (!isActive) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_NOT_INITIALIZED];
        return false;
    }
    delete physicalStorage;
    return true;
}

bool Virtual::ReadAddress(uint64_t addr, uint8_t *value) {
    if (!isActive) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_NOT_INITIALIZED];
        return false;
    }
    uint32_t page = addr / MEM_PAGE_SIZE;
    if (page >= numVirtualPages) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_ADDRESS_ERROR];
        return false;
    }
    if (IS_FLAG_CLEAR(virtualPageTable[page].pageState, PAGE_STATE_IN_USE)) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_FREE_PAGE_ACCESS];
        return false;
    }
    if (IS_FLAG_SET(virtualPageTable[page].pageState, PAGE_STATE_IS_ON_DISK)) {
        if (!SwapInPageList()) {
            LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_CANT_SWAP_IN_PAGE];
            return false;
        } else {
            LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_CANT_SWAP_IN_PAGE];
            return false;
        }
    }
    uint32_t physPage = virtualPageTable[page].physicalPage;
    *value = physicalStorage[(physPage*MEM_PAGE_SIZE)+(addr/MEM_PAGE_SIZE)];
    return true;
}

bool Virtual::WriteAddress(uint64_t addr, uint8_t value) {
    if (!isActive) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_NOT_INITIALIZED];
        return false;
    }
    uint32_t page = addr / MEM_PAGE_SIZE;
    if (page >= numVirtualPages) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_ADDRESS_ERROR];
        return false;
    }
    if (IS_FLAG_CLEAR(virtualPageTable[page].pageState, PAGE_STATE_IN_USE)) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_FREE_PAGE_ACCESS];
        return false;
    }
    if (IS_FLAG_SET(virtualPageTable[page].pageState, PAGE_STATE_IS_ON_DISK)) {
        if (!SwapInPage(page)) {
            LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_CANT_SWAP_IN_PAGE];
            return false;
        }
    }
    uint32_t physPage = virtualPageTable[page].physicalPage;
    physicalStorage[(physPage*MEM_PAGE_SIZE)+(addr/MEM_PAGE_SIZE)] = value;
    return true;
}

bool Virtual::LoadPage(uint32_t page, uint8_t *buffer) {
    if (!isActive) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_NOT_INITIALIZED];
        return false;
    }
    if (page >= numVirtualPages) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_ADDRESS_ERROR];
        return false;
    }
    if (IS_FLAG_CLEAR(virtualPageTable[page].pageState, PAGE_STATE_IN_USE)) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_FREE_PAGE_ACCESS];
        return false;
    }
    if (IS_FLAG_SET(virtualPageTable[page].pageState, PAGE_STATE_IS_ON_DISK)) {
        if (!SwapInPage(page)) {
            LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_CANT_SWAP_IN_PAGE];
            return false;
        }
    }
    uint32_t physPage = virtualPageTable[page].physicalPage;
    memcpy(&physicalStorage[physPage*MEM_PAGE_SIZE], buffer, MEM_PAGE_SIZE);
    return true;
}

bool Virtual::SavePage(uint32_t page, uint8_t *buffer) {
    if (!isActive) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_NOT_INITIALIZED];
        return false;
    }
    if (page >= numVirtualPages) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_ADDRESS_ERROR];
        return false;
    }
    if (IS_FLAG_CLEAR(virtualPageTable[page].pageState, PAGE_STATE_IN_USE)) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_FREE_PAGE_ACCESS];
        return false;
    }
    if (IS_FLAG_SET(virtualPageTable[page].pageState, PAGE_STATE_IS_ON_DISK)) {
        if (!SwapInPage(page)) {
            LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_CANT_SWAP_IN_PAGE];
            return false;
        }
    }
    uint32_t physPage = virtualPageTable[page].physicalPage;
    memcpy(buffer, &physicalStorage[physPage*MEM_PAGE_SIZE], MEM_PAGE_SIZE);
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

bool Virtual::SwapOutUsedPages() {
    if (lruCache.empty()) {
        // This should neve rhappen
        return false;
    }
    if (lruCache.size() == 1) {
        uint32_t page = *lruCache.end();
        lruCache.pop_back();

    }
    if (lruCache.size() > 0) {

    }
}

bool Virtual::SwapInPage(uint32_t page) {
    // Make sure the page is within range
    if (page >= numVirtualPages) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_ADDRESS_ERROR];
        return false;
    }
    // Make sure the page is in use -- can't swap in a free page
    if (!IS_FLAG_SET(virtualPageTable[page].pageState, PAGE_STATE_IN_USE)) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_USED_PAGE_ACCESS];
        return false;
    }
    // Make sure the page was swapped out
    if (IS_FLAG_CLEAR(virtualPageTable[page].pageState, PAGE_STATE_IS_ON_DISK)) {
        LastMemoryError = &MemoryErrorTable[MEMORY_ERROR_CANT_SWAP_IN_PAGE];
        return true;
    }
    // Try to get a free page
    std::vector<uint32_t> list;
    if (findFreePagesFromTheLRU(list)) {
        SwapOutPageList(list);
    }
}

bool Virtual::SwapOutPage(uint32_t page) {
    return false;
}

bool Virtual::FreeNPages(uint32_t pPages, uint32_t *pPageList) {
    return true;
}