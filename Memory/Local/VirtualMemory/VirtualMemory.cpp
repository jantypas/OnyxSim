//
// Created by jantypas on 12/31/22.
//

#include "VirtualMemory.h"
#include <vector>
#include <memory.h>
#include <algorithm>
#include <iterator>
#include <memory.h>
#include <iostream>
#include <map>
#include "../../../Configuration/ConfigParameters.h"
#include <boost/log/trivial.hpp>

/**
 * We need these macros to make our life easier
 * Some inline defines to manage flags rather than compiled functions and
 * a couple of templates for things we're going to do over and over again.
 */
#define CLR_FLAG(x, y)          x &= ~y;
#define SET_FLAG(x, y)          x &= y;
#define IS_FLAG_CLEAR(x, y)     (x & y) != y
#define IS_FLAG_SET(x, y)       (x & y) == y

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

VirtualMemoryError  VirtualErrorTable[] {
        {   MEMORY_ERROR_NONE,                  false,  "No errors"                         },
        {   MEMORY_ERROR_ADDRESS_ERROR,         false,  "Address access error"              },
        {   MEMORY_ERROR_FREE_PAGE_ACCESS,      true,   "Attempt to access a free page"     },
        {   MEMORY_ERROR_USED_PAGE_ACCECSS,     true,   "Attempt to access a used page"     },
        {   MEMORY_ERROR_NOT_INITIALIZED,       false,  "Memory system not ready"           },
        {   MEMORY_ERROR_CANT_SWAP_IN_PAGE,     true,   "Can't swap in page"                },
        {   MEMORY_ERROR_CANT_SWAP_OUT_PAGE,    true,   "Can't swap out page"               },
        {   MEMORY_ERROR_PAGE_IS_LOCKED,        true,   "Page is locked in memory"          },
        {   MEMORY_ERROR_NO_VIRTUAL_PAGES,      true,   "No more virtual pages available"   },
        {   MEMORY_ERROR_NO_VIRTUAL_PAGES,      true,   "No more physical pages available"  },
        {   MEMORY_ERROR_UNKNOWN_MEMORY_ERROR,  true,   "Unknown fatal memory error"       },
        {   MEMORY_ERROR_NO_STORAGE,            true,   "No backing storage available"      }
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
 * @ret
 *
 */
bool VirtualMemory::findFreePagesFromTheLRU(std::vector<uint32_t> &pages) {
    BOOST_LOG_TRIVIAL(debug) <<  "VirtualMemory:findFreePages: Started";
    // No candidates?
    if (lruCache.empty()) {
        BOOST_LOG_TRIVIAL(debug) << "VirtualMemory:findFreePages: lruCache is empty";
        return false;
    }
    /**
     * Get tne number of desired pages.
     * Make sure these pages are not locked or on disk
     */
     uint32_t pg = 0;
     uint32_t minimalPages;
     if (lruCache.size() > MIN_SWAPPABLE_PAGES) {
         BOOST_LOG_TRIVIAL(debug) <<  "VirtualMemory:findFreePages: lruCache pages > minimum swappable pages";
         minimalPages = MIN_SWAPPABLE_PAGES;
     } else {
         BOOST_LOG_TRIVIAL(debug) << "VirtualMemory:findFreePages: lruCache pages = 1";
         minimalPages = 1;
     }
     auto  ix = lruCache.end();
     while ((pg < minimalPages) && (!lruCache.empty())) {
         uint32_t possiblePage = *ix;
         uint16_t state;
         state = virtualPageTable[possiblePage].pageState;
         if (IS_FLAG_CLEAR(state, PAGE_STATE_IS_LOCKED) && IS_FLAG_CLEAR(state, PAGE_STATE_IS_ON_DISK)) {
             BOOST_LOG_TRIVIAL(debug) << "VirtualMemory:findFreePages:Moving page "+std::to_string(*ix);
             lruCache.erase(ix);
             ix --;
             pages.push_back(possiblePage);
             minimalPages ++;
         }
     }
    BOOST_LOG_TRIVIAL(debug) << "VirtualMemory:findFreePages: Found "+std::to_string(minimalPages)+" pages";
     return true;
}

/**
 * markPhysicalPagesAsFree -- Given a physical page, mark it as free
 * @param page  - The page to free
 * @return Success/failure
 */
bool VirtualMemory::markPhysicalPageAsFree(uint32_t page) {
    BOOST_LOG_TRIVIAL(debug) << "VirtualMemory:markPhysicalPagesAsFree started - Page "+std::to_string(page);
    // Make sure our page is within range
    if (page >= numPhysicalPages) {
        BOOST_LOG_TRIVIAL(debug) << "VirtualMemory:markPhysicalPagesAsFree: Memory address error";
        LastMemoryError = &VirtualErrorTable[MEMORY_ERROR_ADDRESS_ERROR];
        return false;
    }
    // Make sure page isn't already free
    if (IS_FLAG_CLEAR(physicalPageTable[page].pageState, PAGE_STATE_IN_USE)) {
        BOOST_LOG_TRIVIAL(debug) << "VirtualMemory:markPhysicalPagesAsFree: Accessing a free page";
        LastMemoryError = &VirtualErrorTable[MEMORY_ERROR_FREE_PAGE_ACCESS];
        return false;
    }
    // Clear the page
    BOOST_LOG_TRIVIAL(debug) << "VirtualMemory:markPhysicalPagesAsFree: Page "+std::to_string(page)+ "set to used";
    CLR_FLAG(virtualPageTable[page].pageState, PAGE_STATE_IN_USE);
    physicalFreePagesList.push_back(page);
    removeAllFromVector(physicalUsedPagesList, page);
    removeAllFromVector(lruCache, page);
    BOOST_LOG_TRIVIAL(debug) << "VirtualMemory:markPhysicalPagesAsFree: Success";
    return true;
}

/**
 * markPhysicalPagesAsUsed -- Given a physical page, mark it as used
 * @param page  - The page to use
 * @return Success/failure
 */
bool VirtualMemory::markPhysicalPageAsUsed(uint32_t page) {
    BOOST_LOG_TRIVIAL(debug) << "VirtualMemory:markPhysicalPageAsUsed - Page "+std::to_string(page);
    // Make sure our page is in range
    if (page >= numPhysicalPages) {
        ReportError("markPhysicalPageAsUsed", MEMORY_ERROR_ADDRESS_ERROR);
        return false;
    }
    // Make sure page isn't already in use
    if (IS_FLAG_SET(physicalPageTable[page].pageState, PAGE_STATE_IN_USE)) {
        ReportError("markPhysicalPageAsUsed", MEMORY_ERROR_USED_PAGE_ACCECSS);
        return true;
    }
    // Make the page as used
    SET_FLAG(physicalPageTable[page].pageState, PAGE_STATE_IN_USE);
    removeAllFromVector(physicalFreePagesList, page);
    physicalUsedPagesList.push_back(page);
    ReportError("markPhysicalPageAsUsed", MEMORY_ERROR_NONE);
    return true;
}

/**
 * markVirtualPageAsFree -- Mark a virtual page as free
 * @param page - page to free
 * @return Success/failure
 */
bool VirtualMemory::markVirtualPageAsFree(uint32_t page) {
    BOOST_LOG_TRIVIAL(debug) << "VirtualMemory:markVirtualPageAsFree: Started for page "+std::to_string(page);
    // Make sure page is in range
    if (page >= numVirtualPages) {
        ReportError("markVirtualPageAsFree", MEMORY_ERROR_ADDRESS_ERROR);
        return false;
    }
    // Make sure the page isn't already free
    if (IS_FLAG_CLEAR(virtualPageTable[page].pageState, PAGE_STATE_IN_USE)) {
        ReportError("markVirtualPageAsFree", MEMORY_ERROR_FREE_PAGE_ACCESS);
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
    ReportError("markVirtualPageAsFree", MEMORY_ERROR_NONE);
    return true;
}

/**
 *
 * @param page
 * @return
 */
bool VirtualMemory::markVirtualPageAsUsed(uint32_t page, uint32_t physPage) {
    // Make sure page is within range
    BOOST_LOG_TRIVIAL(debug) << "VirtualMemory:markVirtualPageAsUsed: Started for page "+std::to_string(page);
    if (page >= numVirtualPages) {
        ReportError("markVirtualPageAsUsed", MEMORY_ERROR_ADDRESS_ERROR);
        return false;
    }
    // Make sure the page isn't already in use
    if (IS_FLAG_SET(virtualPageTable[page].pageState, PAGE_STATE_IN_USE)) {
        ReportError("markVirtualPageAsUsed", MEMORY_ERROR_USED_PAGE_ACCECSS);
        return false;
    }
    // Mark the virtual page as used
    virtualPageTable[page].physicalPage = physPage;
    virtualUsedPagesList.push_back(page);
    removeAllFromVector(virtualFreePagesList, page);
    ReportError("markVirtualPagesAsUsed", MEMORY_ERROR_NONE);
    return true;
}

bool VirtualMemory::markVirtualPageAsSwapped(uint32_t page) {
    BOOST_LOG_TRIVIAL(debug) << "VirtualMemory:markVirtualPageAsSwapped started.  Page = "+std::to_string(page);
    // Make sure page is within range
    if (page >= numVirtualPages) {
        ReportError("markVirtualPageAsSwapped", MEMORY_ERROR_ADDRESS_ERROR);
        return false;
    }
    // Make sure the page isn't already in use
    if (IS_FLAG_SET(virtualPageTable[page].pageState, PAGE_STATE_IN_USE)) {
        BOOST_LOG_TRIVIAL(debug) << "VirtualMemory:markVirtualPageAsSwapped: Page "+ std::to_string(page)+" in use";
        ReportError("markPageAsSwapped", MEMORY_ERROR_USED_PAGE_ACCECSS);
        return false;
    }
    // Mark the virtual page as used
    virtualPageTable[page].physicalPage = 0;
    SET_FLAG(virtualPageTable[page].pageState, PAGE_STATE_IS_ON_DISK);
    virtualUsedPagesList.push_back(page);
    removeAllFromVector(virtualFreePagesList, page);
    ReportError("markVirtualPageAsSwapped", MEMORY_ERROR_NONE);
    return true;
}

bool VirtualMemory::SwapOutPageList(std::vector<uint32_t> &list) {
    std::vector<uint32_t>::iterator ix;

    for (ix = list.begin(); ix != list.end(); ix++) {
        SwapOutPage(*ix);
    }
    return true;
}

bool VirtualMemory::SwapOutPageCandidates() {
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


bool VirtualMemory::Init(ConfigParameters *conf, uint32_t pNumVirtualPages, uint32_t pNumPhysicalPages, std::string swapFileName) {
    uint32_t ix;

    numVirtualPages         = pNumVirtualPages;
    numPhysicalPages        = pNumPhysicalPages;

    BOOST_LOG_TRIVIAL(debug) << "VirtualMemory:Init: Started";
    physicalStorage         = new uint8_t[numPhysicalPages * LOCAL_MEM_PAGE_SIZE];\
    if (physicalStorage == nullptr) {
        ReportError("Init", MEMORY_ERROR_NO_STORAGE);
    } else {
        BOOST_LOG_TRIVIAL(debug) << "VirtualMemory:Init: Allocated "+std::to_string(numPhysicalPages)+" Physical Pages";
    }
    /*
     * Fill our physical free pages list and virtual free pages list with values
     */
    for (uint32_t ix = 0; ix < numPhysicalPages; ix++) {
        physicalFreePagesList.push_back(ix);
    };
    BOOST_LOG_TRIVIAL(debug) << "VirtualMemory:Init: We now have "+std::to_string(physicalFreePagesList.size())+" free physical pages";
    for (uint32_t ix = 0; ix < numVirtualPages; ix++) {
        virtualFreePagesList.push_back(ix);
    };
    BOOST_LOG_TRIVIAL(debug) << "VirtualMemory:Init: We now have "+std::to_string(virtualFreePagesList.size())+" free virtual; pages";
    swapper.Init(conf->getStringValue("swapF[leNema"));
    isActive = true;
    ReportError("Init", MEMORY_ERROR_NONE);
    return true;
}

bool VirtualMemory::Exit() {
    BOOST_LOG_TRIVIAL(debug) << "VirtualMemory:Exit: Started";
    if (!isActive) {
        ReportError("Exit", MEMORY_ERROR_NOT_INITIALIZED);
        return false;
    }
    delete physicalStorage;
    ReportError("Exit", MEMORY_ERROR_NONE);
    isActive = false;
    return true;
}

bool VirtualMemory::ReadAddress(uint64_t addr, uint8_t *value) {
    BOOST_LOG_TRIVIAL(debug) << "VirtualMemory:ReadAddress: Started";
    if (!isActive) {
        ReportError("ReadAddress", MEMORY_ERROR_NOT_INITIALIZED);
        return false;
    }
    uint32_t page = addr / LOCAL_MEM_PAGE_SIZE;
    BOOST_LOG_TRIVIAL(debug) << "VirtualMemory:ReadAddress: Reading address "+std::to_string(addr)+" Page "+
        std::to_string(page)+" Offset: "+std::to_string(addr % LOCAL_MEM_PAGE_SIZE);
    if (page >= numVirtualPages) {
        ReportError("ReadAddress", MEMORY_ERROR_ADDRESS_ERROR);
        return false;
    }
    if (IS_FLAG_CLEAR(virtualPageTable[page].pageState, PAGE_STATE_IN_USE)) {
        ReportError("ReadAddress", MEMORY_ERROR_FREE_PAGE_ACCESS);
        BOOST_LOG_TRIVIAL(error) << "VirtualMemory:ReadAddress:       PageState = "+
            std::to_string(virtualPageTable[page].pageState);
        return false;
    }
    if (IS_FLAG_SET(virtualPageTable[page].pageState, PAGE_STATE_IS_ON_DISK)) {
        if (!SwapOutPageCandidates()) {
            ReportError("ReadAddress", MEMORY_ERROR_CANT_SWAP_OUT_PAGE);
            return false;
        } else {
            ReportError("ReadAddress", MEMORY_ERROR_CANT_SWAP_IN_PAGE);
            return false;
        }
    }
    uint32_t physPage = virtualPageTable[page].physicalPage;
    if (SwapInPage(page)) {
        ReportError("ReadAddress", MEMORY_ERROR_CANT_SWAP_IN_PAGE);
        return false;
    }
    *value = physicalStorage[(physPage*LOCAL_MEM_PAGE_SIZE)+(addr/LOCAL_MEM_PAGE_SIZE)];
    BOOST_LOG_TRIVIAL(debug) << "VirtualMemory:ReadAddress: Read value "+std::to_string(*value);
    ReportError("ReadAddress", MEMORY_ERROR_NONE);
    return true;
}

bool VirtualMemory::WriteAddress(uint64_t addr, uint8_t value) {
    if (!isActive) {
        LastMemoryError = &VirtualErrorTable[MEMORY_ERROR_NOT_INITIALIZED];
        return false;
    }
    uint32_t page = addr / LOCAL_MEM_PAGE_SIZE;
    if (page >= numVirtualPages) {
        LastMemoryError = &VirtualErrorTable[MEMORY_ERROR_ADDRESS_ERROR];
        return false;
    }
    if (IS_FLAG_CLEAR(virtualPageTable[page].pageState, PAGE_STATE_IN_USE)) {
        LastMemoryError = &VirtualErrorTable[MEMORY_ERROR_FREE_PAGE_ACCESS];
        return false;
    }
    if (IS_FLAG_SET(virtualPageTable[page].pageState, PAGE_STATE_IS_ON_DISK)) {
        if (!SwapOutPageCandidates()) {
            LastMemoryError = &VirtualErrorTable[MEMORY_ERROR_CANT_SWAP_IN_PAGE];
            return false;
        } else {
            LastMemoryError = &VirtualErrorTable[MEMORY_ERROR_CANT_SWAP_IN_PAGE];
            return false;
        }
    }
    uint32_t physPage = virtualPageTable[page].physicalPage;
    if (SwapInPage(page)) {
        LastMemoryError = &VirtualErrorTable[MEMORY_ERROR_CANT_SWAP_IN_PAGE];
        return false;
    }
    physicalStorage[(physPage*LOCAL_MEM_PAGE_SIZE)+(addr/LOCAL_MEM_PAGE_SIZE)] = value;
    return true;
}

bool VirtualMemory::LoadPage(uint32_t page, uint8_t *buffer) {
    if (!isActive) {
        LastMemoryError = &VirtualErrorTable[MEMORY_ERROR_NOT_INITIALIZED];
        return false;
    }
    if (page >= numVirtualPages) {
        LastMemoryError = &VirtualErrorTable[MEMORY_ERROR_ADDRESS_ERROR];
        return false;
    }
    if (IS_FLAG_CLEAR(virtualPageTable[page].pageState, PAGE_STATE_IN_USE)) {
        LastMemoryError = &VirtualErrorTable[MEMORY_ERROR_FREE_PAGE_ACCESS];
        return false;
    }
    if (IS_FLAG_SET(virtualPageTable[page].pageState, PAGE_STATE_IS_ON_DISK)) {
        if (!SwapOutPageCandidates()) {
            LastMemoryError = &VirtualErrorTable[MEMORY_ERROR_CANT_SWAP_IN_PAGE];
            return false;
        } else {
            LastMemoryError = &VirtualErrorTable[MEMORY_ERROR_CANT_SWAP_IN_PAGE];
            return false;
        }
    }
    uint32_t physPage = virtualPageTable[page].physicalPage;
    if (SwapInPage(page)) {
        LastMemoryError = &VirtualErrorTable[MEMORY_ERROR_CANT_SWAP_IN_PAGE];
        return false;
    }
    memcpy(&physicalStorage[(physPage*LOCAL_MEM_PAGE_SIZE)], buffer, LOCAL_MEM_PAGE_SIZE);
    return true;
}

bool VirtualMemory::SavePage(uint32_t page, uint8_t *buffer) {
    if (!isActive) {
        LastMemoryError = &VirtualErrorTable[MEMORY_ERROR_NOT_INITIALIZED];
        return false;
    }
    if (page >= numVirtualPages) {
        LastMemoryError = &VirtualErrorTable[MEMORY_ERROR_ADDRESS_ERROR];
        return false;
    }
    if (IS_FLAG_CLEAR(virtualPageTable[page].pageState, PAGE_STATE_IN_USE)) {
        LastMemoryError = &VirtualErrorTable[MEMORY_ERROR_FREE_PAGE_ACCESS];
        return false;
    }
    if (IS_FLAG_SET(virtualPageTable[page].pageState, PAGE_STATE_IS_ON_DISK)) {
        if (!SwapOutPageCandidates()) {
            LastMemoryError = &VirtualErrorTable[MEMORY_ERROR_CANT_SWAP_IN_PAGE];
            return false;
        } else {
            LastMemoryError = &VirtualErrorTable[MEMORY_ERROR_CANT_SWAP_IN_PAGE];
            return false;
        }
    }
    uint32_t physPage = virtualPageTable[page].physicalPage;
    if (SwapInPage(page)) {
        LastMemoryError = &VirtualErrorTable[MEMORY_ERROR_CANT_SWAP_IN_PAGE];
        return false;
    }
    memcpy(buffer, &physicalStorage[(physPage*LOCAL_MEM_PAGE_SIZE)], LOCAL_MEM_PAGE_SIZE);
    return true;
}

bool VirtualMemory::AllocateNPages(uint32_t pPages, std::vector<uint32_t> *pPagelist) {
    BOOST_LOG_TRIVIAL(debug) << "VirtualMemory:AllocateNPage started";
    // Get the next virtual page off the fre list
    if (virtualFreePagesList.empty()) {
        BOOST_LOG_TRIVIAL(error) << "VirtualMemory:AllocateNPagges: Check virtual free page list -- empty";
        ReportError("AllocateNPages", MEMORY_ERROR_NO_VIRTUAL_PAGES);
        return false;
    }

    // Allocate a physical page to this virtual page
    BOOST_LOG_TRIVIAL(error) << "VirtualMemory:AllocateNPages: Try to allocate a physical page";
    if (physicalFreePagesList.size() < MIN_SWAPPABLE_PAGES) {
        if (!SwapOutPageCandidates()) {
            ReportError("AllocateNPages", MEMORY_ERROR_NO_PHYSICAL_PAGES);
            return false;
        };
    }
    /*
     * See if we have enough virtual ad physical pages (combined) to do the load.  If so,
     * great, just load them in.
     * If we DON'T have enough virtual pages, that's an error.
     * If we have enough virtual pages, but not enough physical pages, we have to do a hack.
     * In this case, we create the required number of virtual pages, BUT, we do not allocate physical pages.
     * In this case, we lie and say all of these pages are swapped out/
     */
    if (virtualFreePagesList.size() <= pPages) {
        BOOST_LOG_TRIVIAL(error) << "VirtualMemory:AlllocateNPages: We were asked for "+std::to_string(pPages)+
            " We only had "+std::to_string(virtualFreePagesList.size());
        ReportError("AllocateNPages", MEMORY_ERROR_NO_VIRTUAL_PAGES);
        return false;
    }
    if ((virtualFreePagesList.size() > pPages) &&
            (physicalFreePagesList.size() >= pPages)) {
        BOOST_LOG_TRIVIAL(debug) << "VirtualMemory:AllocateNPages: Requesting more physical pages than we have";
        for (uint32_t ix = 0; ix < pPages; ix++) {
            uint32_t newPhys = physicalFreePagesList[physicalFreePagesList.back()];
            physicalFreePagesList.pop_back();
            uint32_t newVirt = *virtualFreePagesList.end();
            virtualFreePagesList.pop_back();
            markPhysicalPageAsUsed(newPhys);
            markVirtualPageAsUsed(newVirt, newPhys);
            pPagelist->push_back(newVirt);
        }
        ReportError("AllocateNPages", MEMORY_ERROR_NONE);
        return true;
    }
    if ((virtualFreePagesList.size() >= pPages) &&
            (physicalFreePagesList.size() <= pPages)) {
        BOOST_LOG_TRIVIAL(debug) << "VirtualMemory:AllocateNPages: We have enough physical pages";
        for (uint32_t ix = 0; ix < pPages; ix++) {
            uint32_t newVirt = *virtualFreePagesList.end();
            virtualFreePagesList.pop_back();
            markVirtualPageAsSwapped(newVirt);
            pPagelist->push_back(newVirt);
        }
        ReportError("AllocateNPagtes", MEMORY_ERROR_NONE);
        return true;
    }
    ReportError("AllocateNPages", MEMORY_ERROR_UNKNOWN_MEMORY_ERROR);
    return false;
}

bool VirtualMemory::SwapInPage(uint32_t page) {
    // Make sure the page is within range
    uint8_t buffer[LOCAL_MEM_PAGE_SIZE];

    if (page >= numVirtualPages) {
        LastMemoryError = &VirtualErrorTable[MEMORY_ERROR_ADDRESS_ERROR];
        return false;
    }
    // Make sure the page is in use -- can't swap in a free page
    if (!IS_FLAG_SET(virtualPageTable[page].pageState, PAGE_STATE_IN_USE)) {
        LastMemoryError = &VirtualErrorTable[MEMORY_ERROR_USED_PAGE_ACCECSS];
        return false;
    }
    // Make sure the page was swapped out
    if (IS_FLAG_CLEAR(virtualPageTable[page].pageState, PAGE_STATE_IS_ON_DISK)) {
        LastMemoryError = &VirtualErrorTable[MEMORY_ERROR_CANT_SWAP_IN_PAGE];
        return true;
    }
    // Try to get a free page
    std::vector<uint32_t> list;
    if (physicalFreePagesList.size() < MIN_SWAPPABLE_PAGES) {
        /* We don't have enough free pages, try swapping some out  */
        if (!findFreePagesFromTheLRU(list)) {
            if (!SwapOutPageCandidates()) {
                LastMemoryError = &VirtualErrorTable[MEMORY_ERROR_CANT_SWAP_OUT_PAGE];
                return false;
            }
        } else {
            LastMemoryError = &VirtualErrorTable[MEMORY_ERROR_CANT_SWAP_OUT_PAGE];
            return false;
        }
    }
    uint32_t newPhysicalPage = *physicalFreePagesList.end();
    markPhysicalPageAsUsed(newPhysicalPage);
    virtualPageTable[page].physicalPage = newPhysicalPage;
    CLR_FLAG(virtualPageTable[page].pageState, PAGE_STATE_IS_ON_DISK);
    swapper.ReadPageFromPage(page, buffer);
    memcpy(&physicalFreePagesList[page*LOCAL_MEM_PAGE_SIZE], buffer, LOCAL_MEM_PAGE_SIZE);
    return true;
}

bool VirtualMemory::SwapOutPage(uint32_t page) {
    uint16_t state;

    // Make sure the page is within range
    if (page >= numVirtualPages) {
        LastMemoryError = &VirtualErrorTable[MEMORY_ERROR_ADDRESS_ERROR];
        return false;
    }
    // Make sure the page is in use -- can't swap in a free page or a page on disk or locked one
    state = virtualPageTable[page].pageState;
    if (IS_FLAG_SET(state, PAGE_STATE_IS_LOCKED) ||
        IS_FLAG_SET(state, PAGE_STATE_IS_ON_DISK) ||
        IS_FLAG_CLEAR(state, PAGE_STATE_IN_USE)) {
        LastMemoryError = &VirtualErrorTable[MEMORY_ERROR_CANT_SWAP_OUT_PAGE];
        return false;
    }
    // Swap out the page to disk
    swapper.WritePageToSwap(page, &physicalStorage[page*LOCAL_MEM_PAGE_SIZE]);
    SET_FLAG(virtualPageTable[page].pageState, PAGE_STATE_IS_ON_DISK);
    // Free the physical page - we no longer need it
    markPhysicalPageAsUsed(virtualPageTable[page].physicalPage);
    return true;
}

bool VirtualMemory::FreeNPages(uint32_t pPages, uint32_t *pPageList) {
    if (pPages >= numVirtualPages) {
        LastMemoryError = &VirtualErrorTable[MEMORY_ERROR_ADDRESS_ERROR];
        return false;
    }
    uint32_t state = virtualPageTable[pPages].pageState;
    if (IS_FLAG_CLEAR(state, PAGE_STATE_IN_USE) ||
        IS_FLAG_SET(state, PAGE_STATE_IS_ON_DISK) ||
        IS_FLAG_SET(state, PAGE_STATE_IS_LOCKED)) {
        LastMemoryError = &VirtualErrorTable[MEMORY_ERROR_PAGE_IS_LOCKED];
        return false;
    }
    markPhysicalPageAsFree(virtualPageTable[pPages].physicalPage);
    markVirtualPageAsFree(pPages);
    return true;
}

VirtualMemoryInfo *VirtualMemory::GetInfo() {
    return &info;
}

VirtualMemoryError *VirtualMemory::GetError() {
    return LastMemoryError;
}

void VirtualMemory::ReportError(std::string func, uint32_t errornumber) const {
    if (errornumber == MEMORY_ERROR_NONE) {
        BOOST_LOG_TRIVIAL(debug) << "VirtualMemory:"+func+":"+VirtualErrorTable[errornumber].msg;
    } else {
        BOOST_LOG_TRIVIAL(error) << "VirtualMemory:"+func+": "+VirtualErrorTable[errornumber].msg;
    }
}

std::string decodePageBits(uint32_t x) {
    std::string result = "";
    if (x & PAGE_STATE_IS_ON_DISK)  { result += "D"; };
    if (x & PAGE_STATE_IS_LOCKED)   { result += "L"; };
    if (x & PAGE_STATE_IN_USE)      { result += "U"; };
    if (x & PAGE_STATE_IS_DIRTY)    { result += "W"; };
    if (result.length() == 0) { result = "-"; };
    return result;
}

void VirtualMemory::DumpVirtualPageTable() {
    std::map<uint32_t, VirtualMemoryPageInfo>::iterator x = virtualPageTable.begin();
    while (x != virtualPageTable.end()) {
        auto key = x->first;
        auto pageState = x->second.pageState;
        std::cout << "#" + std::to_string(key) + " " +
                     decodePageBits(pageState) + " "+std::to_string(x->second.physicalPage)+"\n";
        x++;
    };
}
