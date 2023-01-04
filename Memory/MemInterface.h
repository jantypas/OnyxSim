//
// Created by jantypas on 12/31/22.
//
#include <stdint-gcc.h>
#include <string>
#ifndef ONYXSIM_MEMINTERFACE_H
#define ONYXSIM_MEMINTERFACE_H

#define MEM_PAGE_SIZE       4096
#define MIN_PHTSICAL_PAGES  16
#define MIN_VIRTUAL_PAGES   32
#define MIN_MAIN_BANKS      4
#define MIN_BANK_SIZE       16
#define MIN_ALTERNATE_BANKS 2

#define MEMORY_ERROR_NONE               0
#define MEMORY_ERROR_NOT_INITIALIZED    1
#define MEMORY_ERROR_NOT_IMPLEMENTED    2
#define MEMORY_ERROR_ADDRESS_ERROR      3
#define MEMORY_ERROR_FREE_PAGE_ACCESS   4
#define MEMORY_ERROR_CANT_SWAP_IN_PAGE  5
#define MEMORY_ERROR_CANT_SWAP_OUT_PAGE 6
#define MEMORY_ERROR_PAGE_IS_LOCKED     7
#define MEMORY_ERROR_PAGE_IS_ON_DISK    8
#define MEMORY_ERROR_USED_PAGE_ACCESS   9
#define MEMORY_ERROR_NO_PHYSICAL_PAGES  10
#define MEMORY_ERROR_NO_VIRTUAL_PAGES   11

#define PAGE_STATE_IN_USE           0x0001
#define PAGE_STATE_IS_LOCKED        0x0002
#define PAGE_STATE_IS_DIRTY         0x0004
#define PAGE_STATE_IS_ON_DISK       0x0008
#define PAGE_STATE_PRIV_NORMAL      0x0000
#define PAGE_STATE_PRIV_SERVICE     0x0010
#define PAGE_STATE_PRIV_DRIVER      0x0020
#define PAGE_STATE_PRIV_SYSTEM      0x0030


struct MemoryErrorMessage {
    std::string msg;
    uint32_t    code;
    bool        isFatal;
};

class MemInterface {
public :
    virtual bool InitLinear(uint32_t pNumPages) = 0;
    virtual bool InitBanked(uint32_t pNumMainBanks, uint32_t pBankSize, uint32_t pNumAlternateBanks) = 0;
    virtual bool InitVirtual(uint32_t pNumVirtualPages, uint32_t pNumPhysicalPages, std::string swapFileName) = 0;
    virtual bool Exit() = 0;
    virtual bool ReadAddress(uint64_t addr, uint8_t *value) = 0;
    virtual bool WriteAddress(uint64_t addr, uint8_t value) = 0;
    virtual bool LoadPage(uint32_t page, uint8_t *buffer) = 0;
    virtual bool SavePage(uint32_t page, uint8_t *buffer) = 0;
    virtual bool SetBank(uint8_t bank) = 0;
    virtual bool GetBank(uint8_t *bank) = 0;
    virtual bool AllocateNPages(uint32_t pPages, uint32_t *pPagelist) = 0;
    virtual bool FreeNPages(uint32_t pPages, uint32_t *pPageList) = 0;
    virtual bool SwapoutPage(uint32_t page) = 0;
    virtual bool SwapInPage(uint32_t page)  = 0;
};

extern MemoryErrorMessage   MemoryErrorTable[];
extern MemoryErrorMessage   *LastMemoryError;
#endif //ONYXSIM_MEMINTERFACE_H
