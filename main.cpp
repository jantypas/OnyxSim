#include <iostream>

#include <vector>
#include "Configuration/ConfigParameters.h"
#include "Memory/Local/VirtualMemory/VirtualMemory.h"
#include "Snarky/SnarkyBootup.h"

int main() {
    srand(time(NULL));
    uint8_t value;
    std::vector<uint32_t> v;
    SnarkyBootup sn = SnarkyBootup();
    for (uint32_t i = 0; i < 10; i++) {
        printf("Boost up message:\n%s\n", sn.getSnarkyBootMessage().c_str());
    };
    auto *config = new ConfigParameters();
    config->Init();
    auto swapname = config->getStringValue("swapFileName");
    auto memoryInterface = new VirtualMemory();
    auto x = memoryInterface->Init(config, 32, 16, swapname);
    memoryInterface->DumpVirtualPageTable();
    auto y = memoryInterface->AllocateNPages(10, &v);
    memoryInterface->DumpVirtualPageTable();
}
