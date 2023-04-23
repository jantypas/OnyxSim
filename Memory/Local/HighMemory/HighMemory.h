//
// Created by jantypas on 12/31/22.
//
#include <map>
#ifndef ONYXSIM_HIGHMEMORY_H
#define ONYXSIM_HIGHMEMORY_H
#include "../../../Configuration/ConfigParameters.h"
#include "../../MemoryConstants.h"

#define MEMORY_ERROR_NONE               0
#define MEMORY_ERROR_ADDRESS_ERROR      1
#define MEMORY_ERROR_NOT_INITIALIZED    2
#define MEMORY_ERROR_NO_STORAGE         3

struct HighConfigParameters {
    std::map<uint32_t, struct config{
        bool        priv;
        uint32_t    numPages;
    }> configuration;
};

class HighMemoryError {
public:
    uint32_t    code;
    bool        isFatal;
    std::string msg;
    HighMemoryError(uint32_t pCode, bool pFatal, std::string pMsg) {
        code    = pCode;
        isFatal = pFatal;
        msg     = pMsg;
    }
    HighMemoryError(const HighMemoryError &t) {
        code    = t.code;
        isFatal = t.isFatal;
        msg     = t.msg;
    }
};

struct HighMemInfo {
    uint32_t numPages;
    uint32_t memPageSize;
};

class HighMemory {
private :
    uint8_t            *storage;
    uint32_t            numPages;
    bool                isActive;
    HighMemoryError    *LastMemoryError;
    HighMemInfo         info;
    void ReportError(std::string func, uint32_t errornumber);
public :
    bool Init(HighConfigParameters *conf, uint32_t pNumPages);
    bool Exit();
    bool ReadAddress(uint64_t addr, uint8_t *value);
    bool WriteAddress(uint64_t addr, uint8_t value);
    bool LoadPage(uint32_t page, uint8_t *buffer);
    bool SavePage(uint32_t page, uint8_t *buffer);
    HighMemInfo *GetInfo();
    HighMemoryError *GetLastError();
    HighMemory() { }
};


#endif //ONYXSIM_HIGHMEMORY_H
