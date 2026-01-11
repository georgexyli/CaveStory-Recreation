#include "head_bump_particle.h" 
#include "rand.h"

namespace{
const units::Game kSourceX{116};
const units::Game kSourceY{54};
const units::Game kWidth{6};
const units::Game kHeight{6};

const units::MS kLifeTime{600};
const units::MS kFlashPeriod{25};

const units::Velocity kSpeed{0.12f};
}

HeadBumpParticle::HeadBumpParticle(
        Graphics& graphics,
        units::Game center_x,
        units::Game center_y) :
    center_x_{center_x},
    center_y_{center_y},
    sprite_(graphics, 
            "Caret", 
            units::gameToPixel(kSourceX), 
            units::gameToPixel(kSourceY), 
            units::gameToPixel(kWidth), 
            units::gameToPixel(kHeight)),
    timer_{kLifeTime, true},
    particle_a_{0,rand_angle()},
    max_offset_a_{static_cast<units::Game>(rand_double(4, 20))},
    particle_b_{0,rand_angle()},
    max_offset_b_{static_cast<units::Game>(rand_double(4, 20))}
{}

bool HeadBumpParticle::update(units::MS elapsed_time){
    particle_a_.magnitude = std::min(kSpeed * elapsed_time.count() + particle_a_.magnitude, max_offset_a_);
    particle_b_.magnitude = std::min(kSpeed * elapsed_time.count() + particle_b_.magnitude, max_offset_b_);
    return timer_.active();  
}

void HeadBumpParticle::draw(Graphics& graphics){
    if (timer_.current_time() / kFlashPeriod % 2 == 0){
        sprite_.draw(graphics, center_x_ + particle_a_.get_x(), center_y_ + particle_a_.get_y());
        sprite_.draw(graphics, center_x_ + particle_b_.get_x(), center_y_ + particle_b_.get_y());
    }
}