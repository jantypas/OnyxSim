//
// Created by jantypas on 1/5/23.
//

#ifndef ONYXSIM_CONFIGPARAMETERS_H
#define ONYXSIM_CONFIGPARAMETERS_H
#include <stdint-gcc.h>
#include <string>
#include <variant>

struct ConfigParameters {
public :
    bool Init();
    std::string getStringValue(std::string name);
    uint32_t    getI32Value(std::string name);
    bool        getBoolValue(std::string name);
};

#endif //ONYXSIM_CONFIGPARAMETERS_H
