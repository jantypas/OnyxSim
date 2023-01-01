//
// Created by jantypas on 12/31/22.
//

#ifndef ONYXSIM_PROCESS_H
#define ONYXSIM_PROCESS_H
#include <string>

class ProcessSegmen {
public :
    uint16_t        processID;
    std::string     name;
    uint32_t        numVirtualPages;
    uint32_t        *virtualPages;
    bool            canRead;
    bool            canWrite;
    bool            canExecute;
    bool            requireSuperuser;
    bool            promoteToSuperuser;
    bool            lockedInMemory;
};

class Process {
public :
    uint32_t        userid;
    uint32_t        groupid;
    std::string     commandline;
    uint8_t         numSegments;
    ProcessSegmen   segments;
    bool            superuser;
};


#endif //ONYXSIM_PROCESS_H
