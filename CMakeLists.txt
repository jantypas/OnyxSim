cmake_minimum_required(VERSION 3.24)
project(OnyxSim)

set(CMAKE_CXX_STANDARD 17)

add_executable(OnyxSim main.cpp Memory/MemInterface.cpp Memory/MemInterface.h Memory/Local/Linear/Linear.cpp Memory/Local/Linear/Linear.h Memory/Local/Banked/Banked.cpp Memory/Local/Banked/Banked.h Memory/Local/Virtual/Virtual.cpp Memory/Local/Virtual/Virtual.h Processes/Process.cpp Processes/Process.h CPU/CPUCore/Onyx1.cpp CPU/CPUCore/Onyx1.h IO/OnyxNet/OnyxNet.cpp IO/OnyxNet/OnyxNet.h Memory/Local/Virtual/Swapper.cpp Memory/Local/Virtual/Swapper.h Configuration/ConfigParameters.cpp Configuration/ConfigParameters.h IO/IOInterface.cpp IO/IOInterface.h BIOS/BIOSInterface.cpp BIOS/BIOSInterface.h Monitor/MonitorInterface.cpp Monitor/MonitorInterface.h CPU/Microcode/CPUMicrocode.cpp CPU/Microcode/CPUMicrocode.h Harness/Harness.cpp Harness/Harness.h BIOS/Timer.cpp BIOS/Timer.h Snarky/SnarkyBootup.cpp Snarky/SnarkyBootup.h)
