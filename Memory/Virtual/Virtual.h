//
// Created by jantypas on 12/31/22.
//

#ifndef ONYXSIM_VIRTUAL_H
#define ONYXSIM_VIRTUAL_H
#include "../MemInterface.h"
#include <map>

class VirtualMemoryPageInfo {
public :
    uint32_t    physicalPage;
    bool        isInUse;
    bool        isDirty;
    bool        isLocked;
    bool        isOnDisk;
    bool        lastUsed;

};

class Virtual : MemInterface {
private :
    std::map<uint32_t, VirtualMemoryPageInfo>   virtualPageTable;
    std::map<uint32_t, bool>                    physicalPageBitmap;
    std::map<uint32_t, bool>                    virtualPageBitmap;
    uint8_t                                     *physicalStorage;
    uint32_t                                    numFreeVirtualPages;
    uint32_t                                    numUsedVirtualPages;
    uint32_t                                    numFreePhysicalPages;
    uint32_t                                    numUsedPhysicalPages;
    uint32_t                                    minVirtualPages;
    uint32_t                                    minPhysicalPages;
    const char *swapName;
    FILE                                        *swapFile;
    bool                                        isActive;
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
    bool AllocateNPages(uint32_t pPages, uint32_t *pPagelist) override;
    bool SwapIn(uint32_t page) override;
    bool Swapout(uint32_t page) override;
};


#endif //ONYXSIM_VIRTUAL_H
