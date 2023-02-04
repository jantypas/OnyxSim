//
// Created by jantypas on 2/3/23.
//

#include "Segments.h"

SegmentError  SegmentErrorTable[] {
        {   SEGMENT_ERROR_NONE,                 false,  "No errors"                                 },
        {   SEGMENT_ERROR_NOT_INITIALIZED,      false,  "Segment manager system not ready"           },
};

void Segments::ReportDebug(std::string service, std::string msg) {
    BOOST_LOG_TRIVIAL(debug) << "Segments:"+service+":"+msg;
};

void Segments::ReportError(std::string service, uint32_t code, std::string msg) {
    if (code == SEGMENT_ERROR_NONE) {
        BOOST_LOG_TRIVIAL(debug) << "Segments:"+service+":No error";
    } else {
        BOOST_LOG_TRIVIAL(error) << "Segments:"+service+":"+SegmentErrorTable[code]+":"+msg;
    }
}

bool Segments::CreateSegment(std::string pName, uint16_t pPrivs, uint8_t pNeedPriv, uint8_t pEscalatePriv,
                             uint32_t pNumPages, uint32_t pProcID, uint32_t *segmentID) {
    return false;
}

void Segments::Init() {

}

void Segments::Exit() {

};