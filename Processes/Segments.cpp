//
// Created by jantypas on 2/3/23.
//

#include "Segments.h"

SegmentError  SegmentErrorTable[] {
        {   SEGMENT_ERROR_NONE,                 false,  "No errors"                                 },
        {   SEGMENT_ERROR_NOT_INITIALIZED,      true,   "Segment manager system not ready"          },
        {   SEGMENT_ERROR_ADDRESS_ERROR,        true,   "Address out of range"                      },
};

void Segments::ReportDebug(std::string service, std::string msg) {
    BOOST_LOG_TRIVIAL(debug) << "Segments:"+service+":"+msg;
};

void Segments::ReportError(std::string service, uint32_t code, std::string msg) {
    if (code == SEGMENT_ERROR_NONE) {
        BOOST_LOG_TRIVIAL(debug) << "Segments:"+service+":No error";
    } else {
        BOOST_LOG_TRIVIAL(error) << "Segments:"+service+":"+SegmentErrorTable[code].msg+":"+msg;
    }
}

bool Segments::CreateSegment(std::string pName, uint16_t pPrivs, uint8_t pNeedPriv, uint8_t pEscalatePriv,
                             uint32_t pNumPages, uint32_t pProcID, uint32_t *segmentID) {
    return false;
}

void Segments::Init() {

}

void Segments::Exit() {

}

bool Segments::DestroySegment(uint32_t segmentid) {
    return false;
}

bool Segments::LockSegment(uint32_t segmentid, bool isLocked) {
    return false;
}

bool Segments::ReadAddress(uint32_t segmentid, uint32_t addr, uint32_t, uint8_t *value) {
    return false;
}

bool Segments::WriteAddress(uint32_t segmentid, uint32_t addr, uint32_t offset, uint8_t value) {
    return false;
}

bool Segments::LoadPage(uint32_t segmentid, uint32_t page, void *buffer) {
    return false;
}

bool Segments::SavePage(uint32_t segmmentid, uint32_t page, void *buffer) {
    return false;
};