//
// Created by jantypas on 12/31/22.
//

#include "MemInterface.h"

MemoryErrorMessage  MemoryErrorTable[] = {
        {"None",                            MEMORY_ERROR_NONE,                  false},
        {"Memory not initialized",          MEMORY_ERROR_NOT_INITIALIZED,       true},
        {"Memory function not implemented", MEMORY_ERROR_NOT_IMPLEMENTED,       false},
        {"Memory address error",            MEMORY_ERROR_ADDRESS_ERROR,         true},
        {"Trying to access a free page",    MEMORY_ERROR_FREE_PAGE_ACCESS,      true},
        {"Can't swap in virtual page",      MEMORY_ERROR_CANT_SWAP_IN_PAGE,     true},
        {"Can't swap out virtual page",     MEMORY_ERROR_CANT_SWAP_OUT_PAGE,    true},
        {"Pace is locked in memory",        MEMORY_ERROR_PAGE_IS_LOCKED,        false},
        {"Page is still on disk",           MEMORY_ERROR_PAGE_IS_ON_DISK,       false},
        {"Trying to access a page in use",  MEMORY_ERROR_USED_PAGE_ACCESS,      true},
        {"No more physical pages",          MEMORY_ERROR_NO_PHYSICAL_PAGES,     true},
        {"No more virtual pages",           MEMORY_ERROR_NO_VIRTUAL_PAGES,      true}
};

MemoryErrorMessage *LastMemoryError;
