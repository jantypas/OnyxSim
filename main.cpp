#include <iostream>

#include <vector>
#include "Memory/Local/VirtualMemory/VirtualMemory.h"

int main() {
    uint8_t value;
    std::vector<uint32_t> v;
    ConfigParameters *config = new ConfigParameters();
    auto memoryInterface = new VirtualMemory();
    auto result = memoryInterface->Init(config, 32, 16, "/tmp/sweap");
    auto result2 = memoryInterface->AllocateNPages(1000, &v);
    printf("We got %d pages\n", v.size());
    result = memoryInterface->WriteAddress(5120, 12);
    result = memoryInterface->ReadAddress(5120, &value);
    memoryInterface->Exit();
}
