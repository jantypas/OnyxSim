//
// Created by jantypas on 2/3/23.
//

#ifndef ONYXSIM_SEGMENTS_H
#define ONYXSIM_SEGMENTS_H
#include <string>
#include <vector>

class Segments {
private :
    std::string             name;
    bool                    privReadable;
    bool                    privWritable;
    bool                    privExecutable;
    uint8_t                 needPriv;
    uint8_t                 escalatePriv;
    bool                    locked;
    std::vector<uint32_t>   virtPages;
    uint16_t                processID;
public :
    bool                    AllocateSegment(uint32_t numPages);
    bool                    FreeSegment();
    bool                    GrowSegment(uint32_t pagees);
    bool                    LockSegment(bool lockState);
    bool                    ReadAddress(uint32_t page, uint32_t offset, uint8_t *value);
    bool                    WriteAddress(uint32_t page, uint32_t offset, uint8_t value);
};


#endif //ONYXSIM_SEGMENTS_H
