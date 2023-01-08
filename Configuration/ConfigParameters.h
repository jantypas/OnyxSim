//
// Created by jantypas on 1/5/23.
//

#ifndef ONYXSIM_CONFIGPARAMETERS_H
#define ONYXSIM_CONFIGPARAMETERS_H
#include <stdint-gcc.h>
#include <string>

class ConfigParameters {
public :
    std::string SwapFileName = "/tmp/virtswap.swp";
};

extern ConfigParameters Configuration;

#endif //ONYXSIM_CONFIGPARAMETERS_H
