//
// Created by jantypas on 1/17/23.
//

#include "Syslog.h"
#include <string>

Syslog::~Syslog() {
    closelog();
}

void Syslog::Message(int priority, std::string msg) {
    syslog(priority, msg.c_str());
}