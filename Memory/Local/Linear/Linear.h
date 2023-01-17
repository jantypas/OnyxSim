//
// Created by jantypas on 12/31/22.
//

#ifndef ONYXSIM_LINEAR_H
#define ONYXSIM_LINEAR_H
#include "../../MemInterface.h"

class Linear : MemInterface {
private :
    uint8_t     *storage;
    uint32_t    numPages;
    bool        isActive;
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
    bool SwapInPage(uint32_t page) override;
    bool SwapOutPage(uint32_t page) override;
};


#endif //ONYXSIM_LINEAR_H
