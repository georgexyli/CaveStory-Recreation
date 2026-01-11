#include "death_cloud_particle.h"
#include "particle_system.h"
#include "rand.h"
#include "units.h"

namespace {
const std::string kSpriteName{"NpcSym"};
const units::Tile kSourceX{1};
const units::Tile kSourceY{0};
const units::Tile kSourceWidth{1};
const units::Tile kSourceHeight{1};
const units::FPS kFps{18};
const units::Velocity kBaseVelocity{0.12f};
const units::Frame kNumFrames{7};
} // namespace

void DeathCloudParticle::createRandomDeathClouds(ParticleTools &particle_tools,
                                                 units::Game center_x,
                                                 units::Game center_y,
                                                 int num_particles) {
  for (int i{0}; i < num_particles; i++) {
    particle_tools.entity_system.addNewParticle(
        std::make_shared<DeathCloudParticle>(
            particle_tools.graphics, center_x, center_y,
            rand_double(0, 2) * kBaseVelocity, rand_angle()));
  }
}

DeathCloudParticle::DeathCloudParticle(Graphics &graphics, units::Game center_x,
                                       units::Game center_y,
                                       units::Velocity speed,
                                       units::Degrees angle)
    : center_x_{center_x}, center_y_{center_y}, speed_{speed},
      offset_{0, angle}, sprite_{graphics,
                                 kSpriteName,
                                 units::tileToPixel(kSourceX),
                                 units::tileToPixel(kSourceY),
                                 units::tileToPixel(kSourceWidth),
                                 units::tileToPixel(kSourceHeight),
                                 kFps,
                                 kNumFrames} {}

bool DeathCloudParticle::update(units::MS elapsed_time) {
  sprite_.update();
  offset_.magnitude += speed_ * elapsed_time.count();
  return sprite_.num_completed_loops() == 0;
}

void DeathCloudParticle::draw(Graphics &graphics) {
  sprite_.draw(graphics, center_x_ + offset_.get_x(),
               center_y_ + offset_.get_y());
}
