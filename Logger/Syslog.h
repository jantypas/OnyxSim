//
// Created by jantypas on 1/17/23.
//

#ifndef ONYXSIM_SYSLOG_H
#define ONYXSIM_SYSLOG_H
#include <syslog.h>
#include <string>

class Syslog {
public :
    ~Syslog();
    void Message(int priority, std::string msg);
};


#endif //ONYXSIM_SYSLOG_H
