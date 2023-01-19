//
// Created by jantypas on 1/17/23.
//

#ifndef ONYXSIM_LOGGER_H
#define ONYXSIM_LOGGER_H
#include <syslog.h>
#include <string>

class Logger {
private :
    std::string app;
public :
    ~Logger();
    Logger(std::string name);
    void Message(int priority, std::string msg);
};


#endif //ONYXSIM_LOGGER_H
