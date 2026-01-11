#include "first_cave_bat.h"
#include "animated_sprite.h"
#include "graphics.h"
#include "sprite_state.h"

namespace{
const std::string bat_file{"NpcCemet"};
const units::Frame kNumFlyFrames{3};
const units::FPS kFlyFps{13};
const units::AngularVelocity kAngularVelocity{120.0f / 1000};
const units::Game kFlightAmplitude{5 * units::kHalfTile};
const units::HP kContactDamage{1};
}

FirstCaveBat::FirstCaveBat(units::Game x, units::Game y, Graphics& graphics) :
        flight_center_y_{y},
        x_{x}, 
        y_{y},
        flight_angle_{},
        facing_{HorizontalFacing::RIGHT},
        damage_text_{std::make_shared<DamageText>()},
        alive_{true}
{
    initializeSprites(graphics);
}

void FirstCaveBat::draw(Graphics& graphics) {
    (*sprites_.find(getSpriteState())).second -> draw(graphics, x_, y_);
}

bool FirstCaveBat::update(units::MS elapsed_time, units::Game player_x){
    flight_angle_ += kAngularVelocity * elapsed_time.count();
    facing_ = x_ + units::kHalfTile > player_x ? 
            HorizontalFacing::LEFT : 
            HorizontalFacing::RIGHT;
    y_ = flight_center_y_ + kFlightAmplitude * units::Game(std::sin(units::degreesToRadians(flight_angle_)));
    sprites_[getSpriteState()] -> update();
    return alive_;
}

units::HP FirstCaveBat::contactDamage() const {
    return kContactDamage;
}

void FirstCaveBat::initializeSprite(Graphics& graphics, const SpriteState& sprite_state){
    units::Tile tile_y = sprite_state.facing() == HorizontalFacing::RIGHT ? 3 : 2;
    sprites_.emplace(sprite_state, std::make_shared<AnimatedSprite>(
                graphics, 
                bat_file, 
                units::tileToPixel(2), 
                units::tileToPixel(tile_y), 
                units::tileToPixel(1), 
                units::tileToPixel(1),
                kFlyFps, kNumFlyFrames));
}

void FirstCaveBat::initializeSprites(Graphics& graphics){
    ENUM_FOREACH(facing, HORIZONTAL_TYPE, HorizontalFacing) {
        initializeSprite(graphics, std::make_tuple(static_cast<HorizontalFacing>(facing)));
    }
}

FirstCaveBat::SpriteState FirstCaveBat::getSpriteState() const{
    return SpriteState{std::make_tuple(facing_)};
}