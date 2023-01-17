#include <iostream>

#include "Memory//MemInterface.h"
#include "Memory/Local/LinearMemory/LinearMemory.h"
#include "Memory/Local/BankedMemory/BankedMemory.h"
#include "Memory/Local/VirtualMemory/VirtualMemory.h"

int main() {
    ConfigParameters *config = new ConfigParameters();
    auto memoryInterface = new VirtualMemory();
    auto result = memoryInterface->InitLinear(config, 1024);
}
