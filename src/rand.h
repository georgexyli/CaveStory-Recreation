#ifndef RAND_H_
#define RAND_H_

#include <random>
#include "units.h"

inline std::default_random_engine& rng(){
    static std::default_random_engine re(std::random_device{}());
    return re;
}

inline double rand_double(double low, double high){
    std::uniform_real_distribution<double> dist(low, high);
    return dist(rng());
}

inline units::Degrees rand_angle(){
    std::uniform_real_distribution<double> dist(0.0, 360.0);
    return dist(rng());
}

#endif // RAND_H_