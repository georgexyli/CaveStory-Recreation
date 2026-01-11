#ifndef PARTICLE_SYSTEM_H_
#define PARTICLE_SYSTEM_H_

#include "units.h"
#include <memory>
#include <set>

class Particle;
class Graphics;

class ParticleSystem {
public:
  void addNewParticle(std::shared_ptr<Particle> particle) {
    particles_.insert(particle);
  }
  void update(units::MS elapsed_time);
  void draw(Graphics &graphics);

private:
  using ParticleSet = std::set<std::shared_ptr<Particle>>;
  ParticleSet particles_;
};

struct ParticleTools {
  ParticleSystem &front_system;
  ParticleSystem &entity_system;
  Graphics &graphics;
};

#endif // PARTICLE_SYSTEM_H_
