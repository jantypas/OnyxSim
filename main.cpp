#include <iostream>

#include "Memory//MemInterface.h"
#include "Memory/Local/LinearMemory/LinearMemory.h"
#include "Memory/Local/BankedMemory/BankedMemory.h"
#include "Memory/Local/VirtualMemory/VirtualMemory.h"

int main() {
    uint8_t value;
    ConfigParameters *config = new ConfigParameters();
    auto memoryInterface = new LinearMemory();
    auto result = memoryInterface->InitLinear(config, 1024);
    result = memoryInterface->ReadAddress(5120, &value);
}
