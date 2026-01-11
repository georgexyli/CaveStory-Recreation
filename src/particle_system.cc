#include "particle_system.h"
#include "particle.h"

void ParticleSystem::update(units::MS elapsed_time){
    for (auto iter = particles_.begin(); iter != particles_.end(); ){
        if ((*iter) -> update(elapsed_time)){
            ++iter;
        } else {
            iter = particles_.erase(iter);
        }
    }
}

void ParticleSystem::draw(Graphics& graphics){
    for (const auto& particle: particles_){
        particle -> draw(graphics);
    }
}