#include "polar_star.h"
#include "sprite.h"
#include <iostream>

namespace{
const std::string kArmsFile{"Arms"};

const int kPolarStarIndex{2};
const units::Game kGunWidth{3 * units::kHalfTile};
const units::Game kGunHeight{2 * units::kHalfTile};

const units::Game kUpOffset{units::tileToGame(2)};
const units::Game kDownOffset{units::tileToGame(4)};
const units::Game kHorizontalOffset{0};

const units::Game kLeftOffset{0};
const units::Game kRightOffset{units::tileToGame(1)};
}

PolarStar::PolarStar(Graphics& graphics) {
    initializeSprites(graphics);
}

void PolarStar::draw(
        Graphics& graphics, 
        units::Game x, 
        units::Game y, 
        HorizontalFacing horizontal_facing, 
        VerticalFacing vertical_facing,
        bool gun_up){
    if (horizontal_facing == HorizontalFacing::LEFT){
        x -= units::kHalfTile;
    }
    if (vertical_facing == VerticalFacing::UP){
        y -= units::kHalfTile / 2;
    } else if (vertical_facing == VerticalFacing::DOWN){
        y += units::kHalfTile / 2;
    }
    if (gun_up) y -= 2.0f;
    sprite_map_[std::make_tuple(horizontal_facing, vertical_facing)]-> draw(graphics, x, y);
}

void PolarStar::initializeSprites(Graphics& graphics){
    ENUM_FOREACH(hf, HORIZONTAL_TYPE, HorizontalFacing){
        ENUM_FOREACH(vf, VERTICAL_TYPE, VerticalFacing){
            initializeSprite(graphics, std::make_tuple(
                    static_cast<HorizontalFacing>(hf), 
                    static_cast<VerticalFacing>(vf)));
        }
    }
}


void PolarStar::initializeSprite(Graphics& graphics, const SpriteState& sprite_state){
    units::Game source_y = sprite_state.horizontal_facing() == HorizontalFacing::LEFT ? kLeftOffset : kRightOffset;
    switch (sprite_state.vertical_facing()) {
        case VerticalFacing::HORIZONTAL:
            source_y += kHorizontalOffset;
            break;
        case VerticalFacing::UP:
            source_y += kUpOffset;
            break;
        case VerticalFacing::DOWN:
            source_y += kDownOffset;
            break;
        default:
            break;
    }
    sprite_map_.emplace(sprite_state, std::make_shared<Sprite>(
            graphics,
            kArmsFile,
            units::gameToPixel(kPolarStarIndex * kGunWidth),
            units::gameToPixel(source_y),
            units::gameToPixel(kGunWidth),
            units::gameToPixel(kGunHeight)));
}