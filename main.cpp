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
    printf("Boost up message:\n%s\n", sn.getSnarkyBootMessage().c_str());
    auto *config = new ConfigParameters();
    config->Init();
    auto swapname = config->getStringValue("swapFileName");
    auto memoryInterface = new VirtualMemory();
    auto x = memoryInterface->Init(config, 32, 16, swapname);
    auto y = memoryInterface->AllocateNPages(4, &v);
    auto z = memoryInterface->WriteAddress(v[0], 128, 52);
    auto a = memoryInterface->ReadAddress(v[0], 100, &value);
    auto b = memoryInterface->ReadAddress(v[0], 128, &value);
    memoryInterface->DumpVirtualPageTable();
    memoryInterface->Exit();
}
