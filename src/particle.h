#ifndef PARTICLE_H_
#define PARTICLE_H_

#include "units.h"

class Graphics;

class Particle{
    public:
        virtual bool update(units::MS elapsed_time) = 0;
        virtual void draw(Graphics& graphics) = 0;
        virtual ~Particle() = default;
};



#endif // PARTICLE_H_