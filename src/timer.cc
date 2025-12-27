#include "timer.h"

// static
std::set<Timer*> Timer::timers_;

void Timer::updateAll(units::MS elapsed_time){
    for (auto& timer : timers_){
        timer -> update(elapsed_time);
    }
}