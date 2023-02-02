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
#define CLR_FLAG(x, y)          x |= ~y
#define SET_FLAG(x, y)          x |= y
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

/***********************************************************************************************************
 *
 * Main routines
 *
 ******************************************************************************************************/


bool VirtualMemory::Init(ConfigParameters *conf, uint32_t pNumVirtualPages, uint32_t pNumPhysicalPages, std::string swapFileName) {
    ReportDebug("Init", "Starting("+std::to_string(pNumVirtualPages)+
        ":"+ std::to_string(pNumPhysicalPages)+
        ":"+swapFileName);
    // Try to allocate physical storage
    uint64_t sz = LOCAL_MEM_PAGE_SIZE*pNumPhysicalPages;
    physicalStorage = new uint8_t[sz];
    ReportDebug("Init", "Attempting to allocate "+std::to_string(sz)+" bytes");
    if (physicalStorage != nullptr) {
        ReportDebug("Init", "Allocated "+std::to_string(pNumPhysicalPages)+" pages");
    } else {
        ReportError("Init", MEMORY_ERROR_NO_PHYSICAL_PAGES, "Can't allocate page storage");
        return false;
    };
    // Allocate the virtual page map and fill the free pages section
    for (uint32_t ix = 0; ix < pNumVirtualPages; ix++) {
        virtualPageTable[ix].pageState = 0;
        virtualPageTable[ix].physicalPage = 0;
        virtualFreePagesList.push_back(ix);
    }
    // Allocate the physical page map and fill thr free pages section
    for (uint32_t ix = 0; ix < pNumPhysicalPages; ix++) {
        physicalFreePagesList.push_back(ix);
    };
    // Start the swapper
    if (!swapper.Init(conf->getStringValue("swapFileName"))) {
        ReportError("Init", MEMORY_ERROR_CANT_SWAP_IN_PAGE, "Can't start swapper");
        return false;
    }
    isActive = true;
    numVirtualPages = pNumVirtualPages;
    numPhysicalPages = pNumPhysicalPages;
    ReportDebug("Init", "No errors");
    return true;
}

bool VirtualMemory::Exit() {
    ReportDebug("Exit", "Started");
    if (!isActive) {
        ReportError("Exit", MEMORY_ERROR_NOT_INITIALIZED, "N/A");
        return false;
    }
    ReportDebug("Exit", "No errors");
    return true;
}


bool VirtualMemory::LoadPage(uint32_t page, uint8_t *buffer) {
    return true;
}

bool VirtualMemory::SavePage(uint32_t page, uint8_t *buffer) {
    return true;
}

bool VirtualMemory::SwapOutPageCandidates() {
    ReportDebug("SwapOutPageCandidates", "Started");
    if (physicalUsedPagesList.size() == 0) {
        ReportError("SwapOutPageCandidates", MEMORY_ERROR_NO_VIRTUAL_PAGES, "No virtual pages to swap out");
        return false;
    } else {
        std::vector<uint32_t>::iterator  ix = virtualUsedPagesList.begin();
        while (ix != virtualUsedPagesList.end()) {
            uint32_t pageState = virtualPageTable[*ix].pageState;
            if (IS_FLAG_CLEAR(pageState, PAGE_STATE_IS_ON_DISK) &&
                IS_FLAG_CLEAR(pageState, PAGE_STATE_IS_LOCKED)) {
                uint32_t ppage = virtualPageTable[*ix].physicalPage;
                swapper.WritePageToSwap(ppage, &physicalStorage[ppage*LOCAL_MEM_PAGE_SIZE]);
                SET_FLAG(virtualPageTable[ppage].pageState, PAGE_STATE_IS_ON_DISK);
                physicalFreePagesList.push_back(ppage);
                physicalUsedPagesList.erase(ix);
            }
        }
    }
}

bool VirtualMemory::AllocateNPages(uint32_t pPages, std::vector<uint32_t> *pPagelist) {
    ReportDebug("AllocateNPages", "Started("+std::to_string(pPages)+")");
    if (virtualFreePagesList.size() >= pPages) {
        // If we have more free pages than we need
        if (physicalFreePagesList.size() > pPages) {
            ReportDebug("AllocateNPages", "FreePages > requested pages");
            // Page by page, get a physical page, get a virtual page and link them
            for (uint32_t ix = 0; ix < pPages; ix++) {
                auto newPPage = physicalFreePagesList.end();   // Get a physical page
                physicalFreePagesList.pop_back();
                physicalUsedPagesList.push_back(*newPPage);                 // Put on used list
                // Do the same for virtual pages
                auto newVPage = virtualFreePagesList.end();
                virtualFreePagesList.pop_back();
                virtualUsedPagesList.push_back(*newVPage);
                // We got the pages, so now do the links
                auto p = *newVPage;
                virtualPageTable[p].physicalPage = *newPPage;
                SET_FLAG(virtualPageTable[p].pageState, PAGE_STATE_IN_USE);
                pPagelist->push_back(*newVPage);
            };
            ReportDebug("AllocateNPages", "No errors");
            return true;
        } else {
            ReportDebug("AllocateNPages", "FreePages < requested pages");
            uint8_t buffer[LOCAL_MEM_PAGE_SIZE];
            for (uint32_t ix = 0; ix < pPages; ix++) {
                uint32_t newVPage = *virtualFreePagesList.end();
                virtualFreePagesList.pop_back();
                virtualPageTable[newVPage].physicalPage = 0;
                SET_FLAG(virtualPageTable[newVPage].pageState, PAGE_STATE_IN_USE);
                SET_FLAG(virtualPageTable[newVPage].pageState, PAGE_STATE_IS_ON_DISK);
                swapper.WritePageToSwap(newVPage, buffer);
                pPagelist->push_back(newVPage);
            };
            ReportDebug("AllocateNPages", "No errors");
            return true;
        }
    } else {
        ReportError("AllocateNPages", MEMORY_ERROR_NO_VIRTUAL_PAGES, "Out of virtual pages");
        return false;
    };
}

bool VirtualMemory::SwapInPage(uint32_t page) {
    return true;
}

bool VirtualMemory::SwapOutPage(uint32_t page) {
    return true;
}

bool VirtualMemory::FreeNPages(uint32_t pPages, uint32_t *pPageList) {
    return true;
}

VirtualMemoryInfo *VirtualMemory::GetInfo() {
    return &info;
}

VirtualMemoryError *VirtualMemory::GetError() {
    return LastMemoryError;
}

void VirtualMemory::ReportError(std::string func, uint32_t errornumber, const std::string& comment) const {
    if (errornumber == MEMORY_ERROR_NONE) {
        BOOST_LOG_TRIVIAL(debug) << "VirtualMemory:" + func + ":" + VirtualErrorTable[errornumber].msg;
    } else
        BOOST_LOG_TRIVIAL(error) << "VirtualMemory:" + func + ": " + VirtualErrorTable[errornumber].msg + ":" + comment;
}


void VirtualMemory::ReportDebug(std::string func, const std::string &comment) const {
    if (VirtualMemoryDebug) {
        BOOST_LOG_TRIVIAL(debug) << "VirtualMemory:" + func + ":" + comment;
    };
}

std::string decodePageBits(uint32_t x) {
    std::string result = "";
    if (x & PAGE_STATE_IS_ON_DISK)  { result += "D"; } else { result += "-"; };
    if (x & PAGE_STATE_IS_LOCKED)   { result += "L"; } else { result += "-"; };
    if (x & PAGE_STATE_IN_USE)      { result += "U"; } else { result += "-"; };
    if (x & PAGE_STATE_IS_DIRTY)    { result += "W"; } else { result += "-"; };
    return result;
}

void VirtualMemory::DumpVirtualPageTable() {
    printf("\n---- Virtual Table -------------------\n" );
    printf("\nVirtual Page Table\n");
    std::map<uint32_t, VirtualMemoryPageInfo>::iterator x = virtualPageTable.begin();
    printf("Virt  State Phys\n");
    while (x != virtualPageTable.end()) {
        auto key = x->first;
        auto pageState = x->second.pageState;
        printf("#%04X %s  %04X\n",
               key, decodePageBits(pageState).c_str(), x->second.physicalPage);
        x++;
    };
    printf("\n----- Physical Table ----------------\n");
    auto y = physicalFreePagesList.begin();
    printf("Free: ");
    while (y != physicalFreePagesList.end()) {
        printf("%04X ", *y);
        y++;
    };
    printf("\n");
    y = physicalUsedPagesList.begin();
    printf("Used: ");
    while (y != physicalUsedPagesList.end()) {
        printf("%04X ", *y);
        y++;
    };
    printf("\n\n");

}

bool VirtualMemory::ReadAddress(uint32_t page, uint32_t addr, uint8_t *value) {
    ReportDebug("ReadAddress",  "VirtualMemory:ReadAddress: Start ("+
        std::to_string(page)+":"+std::to_string(addr)+")");    // Check ranges
    if ((page > numVirtualPages) && (addr >= LOCAL_MEM_PAGE_SIZE)) {
        ReportError("ReadAddress", MEMORY_ERROR_ADDRESS_ERROR, "Page check");
        return false;
    }
    // If the page is not in use, this is an error
    if (IS_FLAG_CLEAR(virtualPageTable[page].pageState, PAGE_STATE_IN_USE)) {
        ReportError("ReadAddress", MEMORY_ERROR_FREE_PAGE_ACCESS, "Page not use");
        return false;
    }
    // If page is not in memory, bring it in
    if (IS_FLAG_SET(virtualPageTable[page].pageState, PAGE_STATE_IS_ON_DISK)) {
        if (!SwapInPage(page)) {
            ReportError("ReadAddress", MEMORY_ERROR_CANT_SWAP_IN_PAGE, "Swapping in page");
            return false;
        }
    }
    uint32_t ppage = virtualPageTable[page].physicalPage;
    *value = physicalStorage[ppage*LOCAL_MEM_PAGE_SIZE+addr];
    ReportDebug("ReadAddress", "Returns "+std::to_string(*value));
    return true;
}

bool VirtualMemory::WriteAddress(uint32_t page, uint32_t addr, uint8_t value) {
    ReportDebug("WriteAderess",  "VirtualMemory:ReadAddress: Start ("+
                                std::to_string(page)+":"+std::to_string(addr)+":"+
                                std::to_string(value)+")");    // Check ranges
    if ((page > numVirtualPages) && (addr >= LOCAL_MEM_PAGE_SIZE)) {
        ReportError("WriteAddress", MEMORY_ERROR_ADDRESS_ERROR, "Page check");
        return false;
    }
    // If the page is not in use, this is an error
    if (IS_FLAG_CLEAR(virtualPageTable[page].pageState, PAGE_STATE_IN_USE)) {
        ReportError("WriteAddress", MEMORY_ERROR_FREE_PAGE_ACCESS, "Page not use");
        return false;
    }
    // If page is not in memory, bring it in
    if (IS_FLAG_SET(virtualPageTable[page].pageState, PAGE_STATE_IS_ON_DISK)) {
        if (!SwapInPage(page)) {
            ReportError("WriteAddress", MEMORY_ERROR_CANT_SWAP_IN_PAGE, "Swapping in page");
            return false;
        }
    }
    uint32_t ppage = virtualPageTable[page].physicalPage;
    physicalStorage[ppage*LOCAL_MEM_PAGE_SIZE+addr] = value;
    ReportDebug("WriteAddress", "No error");
    return true;
}
