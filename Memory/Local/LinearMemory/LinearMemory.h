//
// Created by jantypas on 12/31/22.
//

#ifndef ONYXSIM_LINEARMEMORY_H
#define ONYXSIM_LINEARMEMORY_H
#include "../../../Logger/Syslog.h"
#include "../../../Configuration/ConfigParameters.h"
#include "../../MemoryConstants.h"

#define MEMORY_ERROR_NONE               0
#define MEMORY_ERROR_ADDRESS_ERROR      1
#define MEMORY_ERROR_NOT_INITIALIZED    2

class LinearMemoryError {
public:
    uint32_t    code;
    bool        isFatal;
    std::string msg;
    LinearMemoryError(uint32_t pCode, bool pFatal, std::string pMsg) {
        code    = pCode;
        isFatal = pFatal;
        msg     = pMsg;
    }
    LinearMemoryError(const LinearMemoryError &t) {
        code    = t.code;
        isFatal = t.isFatal;
        msg     = t.msg;
    }
};

struct LinearMemInfo {
    uint32_t numPages;
    uint32_t memPageSize;
};

class LinearMemory {
private :
    Syslog              logger;
    uint8_t            *storage;
    uint32_t            numPages;
    bool                isActive;
    LinearMemoryError  *LastMemoryError;
    LinearMemInfo       info;
public :
    bool Init(ConfigParameters *conf, uint32_t pNumPages);
    bool Exit();
    bool ReadAddress(uint64_t addr, uint8_t *value);
    bool WriteAddress(uint64_t addr, uint8_t value);
    bool LoadPage(uint32_t page, uint8_t *buffer);
    bool SavePage(uint32_t page, uint8_t *buffer);
    LinearMemInfo *GetInfo();
    LinearMemoryError *GetLastError();
};


#endif //ONYXSIM_LINEARMEMORY_H
