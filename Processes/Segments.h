//
// Created by jantypas on 2/3/23.
//

#ifndef ONYXSIM_SEGMENTS_H
#define ONYXSIM_SEGMENTS_H
#include <map>
#include <string>
#include <vector>
#include "boost/log/trivial.hpp"

#define SEGMENT_ERROR_NONE                  0
#define SEGMENT_ERROR_NOT_INITIALIZED       1
#define SEGMENT_ERROR_ADDRESS_ERROR         2

class SegmentError {
public:
    uint32_t    code;
    bool        isFatal;
    std::string msg;
    SegmentError(uint32_t pCode, bool pFatal, std::string pMsg) {
        code    = pCode;
        isFatal = pFatal;
        msg     = pMsg;
    }
    SegmentError(const SegmentError &t) {
        code    = t.code;
        isFatal = t.isFatal;
        msg     = t.msg;
    }
};

class SegmentInfo {
public :
    std::string             name;
    uint16_t                privs;
    uint8_t                 needPriv;
    uint8_t                 escalatePriv;
    std::vector<uint32_t>   virtPages;
    uint16_t                processID;
    uint32_t                numPages;
};

class Segments {
    std::map<uint32_t, SegmentInfo> segmentTable;
    void ReportError(std::string service, uint32_t code, std::string msg);
    void ReportDebug(std::string service, std::string msg);
public :
    void        Init();
    void        Exit();
    bool        CreateSegment(
                    std::string pName, uint16_t pPrivs, uint8_t pNeedPriv, uint8_t pEscalatePriv,
                    uint32_t pNumPages, uint32_t pProcID, uint32_t *segmentID
               );
    bool        DestroySegment(uint32_t segmentid);
    bool        LockSegment(uint32_t segmentid, bool isLocked);
    bool        ReadAddress(uint32_t segmentid, uint32_t addr, uint32_t, uint8_t *value);
    bool        WriteAddress(uint32_t segmentid, uint32_t addr, uint32_t offset, uint8_t value);
    bool        LoadPage(uint32_t segmentid, uint32_t page, void *buffer);
    bool        SavePage(uint32_t segmmentid, uint32_t page, void *buffer);
};
#endif //ONYXSIM_SEGMENTS_H
