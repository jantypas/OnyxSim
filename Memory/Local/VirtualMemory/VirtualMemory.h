//
// Created by jantypas on 12/31/22.
//

#ifndef ONYXSIM_VIRTUALMEMORY_H
#define ONYXSIM_VIRTUALMEMORY_H
#include <map>
#include <vector>
#include "../../../Configuration/ConfigParameters.h"
#include "Swapper.h"
#include "../../MemoryConstants.h"

#define MIN_SWAPPABLE_PAGES         16

#define MEMORY_ERROR_NONE                   0
#define MEMORY_ERROR_ADDRESS_ERROR          1
#define MEMORY_ERROR_FREE_PAGE_ACCESS       2
#define MEMORY_ERROR_USED_PAGE_ACCECSS      3
#define MEMORY_ERROR_NOT_INITIALIZED        4
#define MEMORY_ERROR_CANT_SWAP_IN_PAGE      5
#define MEMORY_ERROR_CANT_SWAP_OUT_PAGE     6
#define MEMORY_ERROR_PAGE_IS_LOCKED         7
#define MEMORY_ERROR_NO_VIRTUAL_PAGES       8
#define MEMORY_ERROR_NO_PHYSICAL_PAGES      9
#define MEMORY_ERROR_UNKNOWN_MEMORY_ERROR   10
#define MEMORY_ERROR_NO_STORAGE             11

#define PAGE_STATE_IS_LOCKED    0x0001
#define PAGE_STATE_IS_ON_DISK   0x0002
#define PAGE_STATE_IN_USE       0x0004
#define PAGE_STATE_IS_DIRTY           0x0008

class VirtualMemoryError {
public:
    uint32_t    code;
    bool        isFatal;
    std::string msg;
    VirtualMemoryError(uint32_t pCode, bool pFatal, std::string pMsg) {
        code    = pCode;
        isFatal = pFatal;
        msg     = pMsg;
    }
    VirtualMemoryError(const VirtualMemoryError &t) {
        code    = t.code;
        isFatal = t.isFatal;
        msg     = t.msg;
    }
};

struct VirtualMemoryInfo {
    uint32_t virtualPages;
    uint32_t physicalPages;
    uint32_t swapIns;
    uint32_t swapOuts;
};

class VirtualMemoryPageInfo {
public :
    uint32_t    physicalPage;
    uint16_t    pageState;
    bool        recentlyUsed;
};

class PhysicalMemoryPageInfo {
public :
    uint16_t pageState;
};

class VirtualMemory {
private :
    std::map<uint32_t, VirtualMemoryPageInfo>   virtualPageTable;
    std::map<uint32_t, PhysicalMemoryPageInfo>  physicalPageTable;
    std::vector<uint32_t>                       physicalFreePagesList;
    std::vector<uint32_t>                       physicalUsedPagesList;
    std::vector<uint32_t>                       virtualFreePagesList;
    std::vector<uint32_t>                       virtualUsedPagesList;
    std::vector<uint32_t>                       lruCache;
    uint32_t                                    numPhysicalPages;
    uint32_t                                    numVirtualPages;
    uint8_t                                     *physicalStorage;
    uint32_t                                    minVirtualPages;
    uint32_t                                    minPhysicalPages;
    bool                                        isActive;
    Swapper                                     swapper;
    VirtualMemoryInfo                           info;

    bool markPhysicalPageAsFree(uint32_t page);
    bool markPhysicalPageAsUsed(uint32_t page);
    bool markVirtualPageAsFree(uint32_t page);
    bool markVirtualPageAsUsed(uint32_t page, uint32_t physPage);
    bool markVirtualPageAsSwapped(uint32_t page);
    bool findFreePagesFromTheLRU(std::vector<uint32_t> &pages);
    bool SwapOutPageList(std::vector<uint32_t> &list);
    bool SwapOutPageCandidates();
    void ReportError(std::string func, uint32_t errornumber) const;
public:
    VirtualMemory() {
    }
    VirtualMemoryError                         *LastMemoryError;
    bool Init(ConfigParameters *conf, uint32_t pNumVirtualPages, uint32_t pNumPhysicalPages, std::string swapFileName);
    bool Exit();
    bool ReadAddress(uint64_t addr, uint8_t *value);
    bool WriteAddress(uint64_t addr, uint8_t value);
    bool LoadPage(uint32_t page, uint8_t *buffer);
    bool SavePage(uint32_t page, uint8_t *buffer);
    bool AllocateNPages(uint32_t pPages, std::vector<uint32_t> *pPagelist);
    bool FreeNPages(uint32_t pPages, uint32_t *pPageList);
    bool SwapInPage(uint32_t page);
    bool SwapOutPage(uint32_t page);
    VirtualMemoryInfo   *GetInfo();
    VirtualMemoryError  *GetError();
    void DumpVirtualPageTable();
};


#endif //ONYXSIM_VIRTUALMEMORY_H
