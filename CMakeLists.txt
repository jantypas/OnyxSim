cmake_minimum_required(VERSION 3.24)
project(OnyxSim)

set(CMAKE_CXX_STANDARD 17)

add_executable(OnyxSim main.cpp Memory/MemInterface.cpp Memory/MemInterface.h Memory/Linear/Linear.cpp Memory/Linear/Linear.h Memory/Banked/Banked.cpp Memory/Banked/Banked.h Memory/Virtual/Virtual.cpp Memory/Virtual/Virtual.h Processes/Process.cpp Processes/Process.h CPU/CPUCore/Onyx1.cpp CPU/CPUCore/Onyx1.h IO/OnyxNet/OnyxNet.cpp IO/OnyxNet/OnyxNet.h Memory/Virtual/Swapper.cpp Memory/Virtual/Swapper.h Memory/Virtual/Physical.cpp Memory/Virtual/Physical.h Configuration/ConfigParameters.cpp Configuration/ConfigParameters.h IO/IOInterface.cpp IO/IOInterface.h BIOS/BIOSInterface.cpp BIOS/BIOSInterface.h Monitor/MonitorInterface.cpp Monitor/MonitorInterface.h CPU/CPUMicrocode.cpp CPU/CPUMicrocode.h Harness/Harness.cpp Harness/Harness.h)
