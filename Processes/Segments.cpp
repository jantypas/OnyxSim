//
// Created by jantypas on 2/3/23.
//

#include "Segments.h"

bool Segments::AllocateSegment(uint32_t numPages) {
    return false;
}

bool Segments::FreeSegment() {
    return false;
}

bool Segments::GrowSegment(uint32_t pagees) {
    return false;
}

bool Segments::LockSegment(bool lockState) {
    return false;
}

bool Segments::WriteAddress(uint32_t page, uint32_t offset, uint8_t value) {
    return false;
}

bool Segments::ReadAddress(uint32_t page, uint32_t offset, uint8_t *value) {
    return false;
}
