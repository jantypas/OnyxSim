//
// Created by jantypas on 12/31/22.
//

#ifndef ONYXSIM_VIRTUALMEMORY_H
#define ONYXSIM_VIRTUALMEMORY_H
#include "../../MemInterface.h"
#include <map>
#include <vector>
#include "../../../Configuration/ConfigParameters.h"
#include "Swapper.h"

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

class VirtualMemory : MemInterface {
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

    bool markPhysicalPageAsFree(uint32_t page);
    bool markPhysicalPageAsUsed(uint32_t page);
    bool markVirtualPageAsFree(uint32_t page);
    bool markVirtualPageAsUsed(uint32_t page, uint32_t physPage);
    bool markVirtualPageAsSwapped(uint32_t page);
    bool findFreePagesFromTheLRU(std::vector<uint32_t> &pages);
    bool SwapOutPageList(std::vector<uint32_t> &list);
    bool SwapOutPageCandidates();
public:
    bool InitLinear(ConfigParameters *conf, uint32_t pNumPages) override;
    bool InitVirtual(ConfigParameters *conf, uint32_t pNumVirtualPages, uint32_t pNumPhysicalPages, std::string swapFileName) override;
    bool Exit() override;
    bool ReadAddress(uint64_t addr, uint8_t *value) override;
    bool WriteAddress(uint64_t addr, uint8_t value) override;
    bool LoadPage(uint32_t page, uint8_t *buffer) override;
    bool SavePage(uint32_t page, uint8_t *buffer) override;
    bool AllocateNPages(uint32_t pPages, std::vector<uint32_t> &pPagelist) override;\
    bool FreeNPages(uint32_t pPages, uint32_t *pPageList) override;
    bool SwapInPage(uint32_t page) override;
    bool SwapOutPage(uint32_t page) override;
    MemoryInfo GetInfo() override;
};


#endif //ONYXSIM_VIRTUALMEMORY_H
