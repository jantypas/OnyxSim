//
// Created by jantypas on 12/31/22.
//

#ifndef ONYXSIM_VIRTUAL_H
#define ONYXSIM_VIRTUAL_H
#include "../MemInterface.h"
#include <map>
#include <vector>

class VirtualMemoryPageInfo {
public :
    uint32_t    physicalPage;
    uint16_t    pageState;
};

class PhysicalMemoryPageInfo {
public :
    uint16_t pageState;
};

class Virtual : MemInterface {
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
    const char                                  *swapName;
    FILE                                        *swapFile;
    bool                                        isActive;
    bool MarkPhysicalPageAsUsed(uint32_t page);
    bool MarkPhysicalPageAsFree(uint32_t page);
    bool MarkVirtualPageAsUsed(uint32_t page);
    bool MarkVirtualPageAsFree(uint32_t page);
    bool WritePageToSwap(uint32_t page);
    bool ReadPageFromSwap(uint32_t page);
public :
    bool InitLinear(uint32_t pNumPages) override;
    bool InitBanked(uint32_t pNumMainBanks, uint32_t pBankSize, uint32_t pNumAlternateBanks) override;
    bool InitVirtual(uint32_t pNumVirtualPages, uint32_t pNumPhysicalPages, std::string swapFileName) override;
    bool Exit() override;
    bool ReadAddress(uint64_t addr, uint8_t *value) override;
    bool WriteAddress(uint64_t addr, uint8_t value) override;
    bool LoadPage(uint32_t page, uint8_t *buffer) override;
    bool SavePage(uint32_t page, uint8_t *buffer) override;
    bool GetBank(uint8_t *bank) override;
    bool SetBank(uint8_t bank) override;
    bool AllocateNPages(uint32_t pPages, uint32_t *pPagelist) override;\
    bool FreeNPages(uint32_t pPages, uint32_t *pPageList) override;
    bool SwapInPage(uint32_t page) override;
    bool SwapoutPage(uint32_t page) override;\
};


#endif //ONYXSIM_VIRTUAL_H
