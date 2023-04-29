//
// Created by jantypas on 4/29/23.
//

#include <stdint-gcc.h>
#include <string>
#ifndef ONYXSIM_ABSTRACTERROR_H
#define ONYXSIM_ABSTRACTERROR_H


class AbstractError {
public :
    uint_fast64_t   code;
    std::string     msg;
    std::string     publicURL;
};


#endif //ONYXSIM_ABSTRACTERROR_H
