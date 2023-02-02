//
// Created by jantypas on 1/5/23.
//

#include "../Configuration/ConfigParameters.h"
#include <string>
#include <map>

std::map<std::string, std::string>      systemConfigStrings;
std::map<std::string, uint32_t>         systemConfigI32;
std::map<std::string, bool>             systemConfigBool;

bool ConfigParameters::Init() {
    systemConfigStrings["swapFileName"] = "/tmp/swapfile.swp";
    return true;
}

bool ConfigParameters::getBoolValue(std::string name) {
    return systemConfigBool[name];
}

uint32_t ConfigParameters::getI32Value(std::string name) {
    return systemConfigI32[name];
}

std::string ConfigParameters::getStringValue(std::string name) {
    return systemConfigStrings[name];
}
