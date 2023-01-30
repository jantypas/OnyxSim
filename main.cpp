#include <iostream>

#include "Memory/Local/VirtualMemory/VirtualMemory.h"

int main() {
    uint8_t value;
    ConfigParameters *config = new ConfigParameters();
    auto memoryInterface = new VirtualMemory();
    auto result = memoryInterface->Init(config, 1024, 16, "/tmp/sweap");
    result = memoryInterface->WriteAddress(5120, 12);
    result = memoryInterface->ReadAddress(5120, &value);
    memoryInterface->Exit();
}
