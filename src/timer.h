#ifndef TIMER_H_
#define TIMER_H_

#include "units.h"
#include <set>

class Timer {
    public:
        Timer(units::MS expiration_time) : 
                current_time_{expiration_time},
                expiration_time_{expiration_time}{ timers_.insert(this); }
        ~Timer(){ timers_.erase(this); }

        // We want to delete copy and move constructors / assignment 
        // operators because of hidden global state for timers_ set

        Timer(const Timer& other) = delete;
        Timer(Timer&& other) = delete;
        Timer& operator=(const Timer& other) = delete;
        Timer& operator=(Timer&& other) = delete;

        /* Deleted constructors (not too interesting but I'm kinda new to 
        this stuff so adding a comment here) */
        
        void reset() { current_time_ = units::MS{0}; }
        bool active() const { return current_time_ < expiration_time_; }
        bool expired() const { return !active(); }
        units::MS current_time() const { return current_time_; }

        static void updateAll(units::MS elapsed_time);

    private:
        void update(units::MS elapsed_time){
            if (active()) current_time_ += elapsed_time;
        }

        units::MS current_time_;
        const units::MS expiration_time_;

        static std::set<Timer*> timers_;
};

#endif // TIMER_H_