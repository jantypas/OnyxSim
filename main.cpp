#include <iostream>

#include <vector>
#include "Configuration/ConfigParameters.h"
#include "Memory/Local/VirtualMemory/VirtualMemory.h"

int main() {
    uint8_t value;
    std::vector<uint32_t> v;
    auto *config = new ConfigParameters();
    config->Init();
    auto swapname = config->getStringValue("swapFileName");
    auto memoryInterface = new VirtualMemory();
    auto x = memoryInterface->Init(config, 32, 16, swapname);
    auto y = memoryInterface->AllocateNPages(10, &v);
    memoryInterface->DumpVirtualPageTable();
}
