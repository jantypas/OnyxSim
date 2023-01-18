#include <iostream>

#include "Memory/Local/LinearMemory/LinearMemory.h"

int main() {
    uint8_t value;
    ConfigParameters *config = new ConfigParameters();
    auto memoryInterface = new LinearMemory();
    auto result = memoryInterface->Init(config, 1024);
    result = memoryInterface->ReadAddress(5120, &value);
}
