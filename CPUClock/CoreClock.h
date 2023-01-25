//
// Created by jantypas on 1/23/23.
//

#ifndef ONYXSIM_CORECLOCK_H
#define ONYXSIM_CORECLOCK_H
#include <chrono>
#include <thread>
#include "../CPU/CPU.h"

#define BASE_CLOCK_TICK_NS     142 // 1 fantasy MIP

class CoreClock {
    uint64_t baseFrequency;
    bool running;
public :
    CoreClock() {
        baseFrequency = BASE_CLOCK_TICK_NS;
        running = false;
    }
    void SetClockingFrequency(uint64_t freq) {
        baseFrequency = freq;
        running = true;
    };
    void Tick(OnyxCPU *cpu) {
        if (baseFrequency != 0) {
            std::this_thread::sleep_for(std::chrono::nanoseconds (baseFrequency));
        };
        cpu.Tick();
    }
    void Run(OnyxCPU *cpu) {
        while (!running) {
            Tick(cpu);
        }
    }
};


#endif //ONYXSIM_CORECLOCK_H
