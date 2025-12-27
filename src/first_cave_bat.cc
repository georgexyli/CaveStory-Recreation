#include "first_cave_bat.h"
#include "animated_sprite.h"
#include "graphics.h"
#include "sprite_state.h"

namespace{
const std::string bat_file{"content/NpcCemet.pbm"};
const units::Frame kNumFlyFrames{3};
const units::FPS kFlyFps{13};
const units::AngularVelocity kAngularVelocity{120.0f / 1000};
const units::Game kFlightAmplitude{5 * units::kHalfTile};
}

FirstCaveBat::FirstCaveBat(units::Game x, units::Game y, Graphics& graphics) :
        center_y_{y},
        x_{x}, 
        y_{y},
        flight_angle_{},
        facing_{HorizontalFacing::RIGHT}
{
    initializeSprites(graphics);
}

void FirstCaveBat::draw(Graphics& graphics) const{
    (*sprites_.find(getSpriteState())).second -> draw(graphics, x_, y_);
}

void FirstCaveBat::update(units::MS elapsed_time, units::Game player_x){
    flight_angle_ += kAngularVelocity * elapsed_time.count();
    facing_ = x_ + units::kHalfTile > player_x ? 
            HorizontalFacing::LEFT : 
            HorizontalFacing::RIGHT;
    y_ = center_y_ + kFlightAmplitude * units::Game(std::sin(units::degreesToRadians(flight_angle_)));
    sprites_[getSpriteState()] -> update(elapsed_time);
}

void FirstCaveBat::initializeSprite(Graphics& graphics, const SpriteState& sprite_state){
    units::Tile tile_y = sprite_state.facing == HorizontalFacing::RIGHT ? 3 : 2;
    sprites_[sprite_state] = std::make_shared<AnimatedSprite>(
                graphics, 
                bat_file, 
                units::tileToPixel(2), 
                units::tileToPixel(tile_y), 
                units::tileToPixel(1), 
                units::tileToPixel(1),
                kFlyFps, kNumFlyFrames);
}

void FirstCaveBat::initializeSprites(Graphics& graphics){
    for (int facing{static_cast<int>(HorizontalFacing::FIRST_HORIZONTAL_TYPE)};
            facing < static_cast<int>(HorizontalFacing::LAST_HORIZONTAL_TYPE); 
            ++facing){
        initializeSprite(graphics, SpriteState{static_cast<HorizontalFacing>(facing)});
    }
}

FirstCaveBat::SpriteState FirstCaveBat::getSpriteState() const{
    return SpriteState{facing_};
}