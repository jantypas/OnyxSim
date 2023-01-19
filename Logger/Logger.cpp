//
// Created by jantypas on 1/17/23.
//

#include "Logger.h"
#include <string>

Logger::Logger(std::string name) {
    app = name;
}
Logger::~Logger() {
    closelog();
}

void Logger::Message(int priority, std::string msg) {
    std::string line;
    line = app+":"+msg;
    syslog(priority, "%s", line.c_str());
}
