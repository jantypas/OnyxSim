cmake_minimum_required(VERSION 3.24)
project(OnyxSim)

set(CMAKE_CXX_STANDARD 17)

add_executable(OnyxSim main.cpp Memory/MemInterface.cpp Memory/MemInterface.h Memory/Linear/Linear.cpp Memory/Linear/Linear.h Memory/Banked/Banked.cpp Memory/Banked/Banked.h Memory/Virtual/Virtual.cpp Memory/Virtual/Virtual.h Processes/Process.cpp Processes/Process.h)
