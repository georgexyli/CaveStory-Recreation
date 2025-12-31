#include "polar_star.h"
#include "sprite.h"
#include <iostream>
#include "map.h"    

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

const units::Game kNozzleHorizontalY{23};
const units::Game kNozzleHorizontalLeftX{10};
const units::Game kNozzleHorizontalRightX{38};

const units::Game kNozzleUpY{4};
const units::Game kNozzleUpLeftX{27};
const units::Game kNozzleUpRightX{21};

const units::Game kNozzleDownY{28};
const units::Game kNozzleDownLeftX{29};
const units::Game kNozzleDownRightX{19};

const units::Game kProjectTileSourceY{units::tileToGame(2)};
const units::Game kHorizontalProjectileSourceX{units::tileToGame(8)};
const units::Game kVerticalProjectileSourceX{units::tileToGame(9)};

const units::Velocity kProjectileSpeed{0.6f};
const units::Game kProjectileMaxOffset{7 * units::kHalfTile};

const units::Game kProjectileWidth{4.0f};
const units::Game kProjectileHeight{32.0f};
}

PolarStar::PolarStar(Graphics& graphics){
    initializeSprites(graphics);
}

void PolarStar::updateProjectiles(units::MS elapsed_time, const Map& map){
    if (projectile_a_) {
        if(!projectile_a_ -> update(elapsed_time, map)){
            projectile_a_ = nullptr;
        }
    }
    if (projectile_b_) {
        if(!projectile_b_ -> update(elapsed_time, map)){
            projectile_b_ = nullptr;
        }
    }
}

void PolarStar::draw(
        Graphics& graphics, 
        units::Game player_x, 
        units::Game player_y, 
        HorizontalFacing horizontal_facing, 
        VerticalFacing vertical_facing,
        bool gun_up){
    units::Game x{gun_x(horizontal_facing, player_x)};
    units::Game y{gun_y(vertical_facing, player_y, gun_up)};

   sprite_map_[std::make_tuple(horizontal_facing, vertical_facing)]-> draw(graphics, x, y);
   if (projectile_a_) projectile_a_ -> draw(graphics);
   if (projectile_b_) projectile_b_ -> draw(graphics);
}

units::Game PolarStar::gun_y(VerticalFacing vertical_facing, units::Game player_y, bool gun_up) const {
    units::Game gun_y{player_y};
    if (vertical_facing == VerticalFacing::UP){
        gun_y -= units::kHalfTile / 2;
    } else if (vertical_facing == VerticalFacing::DOWN){
        gun_y += units::kHalfTile / 2;
    }
    if (gun_up) gun_y -= 2.0f;
    return gun_y;
}

void PolarStar::startFire(units::Game player_x, units::Game player_y,
        HorizontalFacing horizontal_facing, VerticalFacing vertical_facing, 
        bool gun_up){
    if (projectile_a_ && projectile_b_) return;
    units::Game bullet_x{gun_x(horizontal_facing, player_x) - units::kHalfTile};
    units::Game bullet_y{gun_y(vertical_facing, player_y, gun_up) - units::kHalfTile};

    switch (vertical_facing){
        case VerticalFacing::HORIZONTAL:
            bullet_y += kNozzleHorizontalY;
            if (horizontal_facing == HorizontalFacing::LEFT){
                bullet_x += kNozzleHorizontalLeftX;
            } else {
                bullet_x += kNozzleHorizontalRightX;
            }
            break;
        case VerticalFacing::UP:
            bullet_y += kNozzleUpY;
            if (horizontal_facing == HorizontalFacing::LEFT){
                bullet_x += kNozzleUpLeftX;
            } else {
                bullet_x += kNozzleUpRightX;
            }
            break;
        case VerticalFacing::DOWN:
            bullet_y += kNozzleDownY;
            if (horizontal_facing == HorizontalFacing::LEFT){
                bullet_x += kNozzleDownLeftX;
            } else {
                bullet_x += kNozzleDownRightX;
            }
        default:
            break;
    }

    if (!projectile_a_){
        projectile_a_ = std::make_shared<Projectile>(
                vertical_facing == VerticalFacing::HORIZONTAL ? horizontal_projectile_ : vertical_projectile_,
                horizontal_facing,
                vertical_facing,
                bullet_x, bullet_y);
    } else {
        projectile_b_ = std::make_shared<Projectile>(
                vertical_facing == VerticalFacing::HORIZONTAL ? horizontal_projectile_ : vertical_projectile_,
                horizontal_facing,
                vertical_facing,
                bullet_x, bullet_y);        
    }
}

void PolarStar::initializeSprites(Graphics& graphics){
    horizontal_projectile_ = std::make_shared<Sprite>(
            graphics,
            "Bullet",
            units::gameToPixel(kHorizontalProjectileSourceX),
            units::gameToPixel(kProjectTileSourceY),
            units::tileToPixel(1),
            units::tileToPixel(1));
    vertical_projectile_ = std::make_shared<Sprite>(
            graphics,
            "Bullet",
            units::gameToPixel(kVerticalProjectileSourceX),
            units::gameToPixel(kProjectTileSourceY),
            units::tileToPixel(1),
            units::tileToPixel(1));
 
    ENUM_FOREACH(hf, HORIZONTAL_TYPE, HorizontalFacing){
        ENUM_FOREACH(vf, VERTICAL_TYPE, VerticalFacing){
            initializeSprite(graphics, std::make_tuple(
                    static_cast<HorizontalFacing>(hf), 
                    static_cast<VerticalFacing>(vf)));
        }
    }
}

void PolarStar::initializeSprite(Graphics& graphics, const SpriteState& sprite_state){
    units::Game source_y{sprite_state.horizontal_facing() == HorizontalFacing::LEFT ? kLeftOffset : kRightOffset};
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

PolarStar::Projectile::Projectile(std::shared_ptr<Sprite> sprite,
        HorizontalFacing horizontal_direction,
        VerticalFacing vertical_direction,
        units::Game x, units::Game y) : 
    sprite_{sprite},
    horizontal_direction_{horizontal_direction},
    vertical_dirction_{vertical_direction},
    x_{x},
    y_{y},
    offset_{0}
    {}

bool PolarStar::Projectile::update(units::MS elapsed_time, const Map& map){
    offset_ += elapsed_time.count() * kProjectileSpeed;
    std::vector<Map::CollisionTile> colliding_tiles{map.getCollidingTiles(collisionRectangle())};
    for (const auto& tile : colliding_tiles){
        if (tile.tile_type == Map::TileType::WALL_TILE){
            return false;
        }
    }
    return offset_ < kProjectileMaxOffset;
}

Rectangle PolarStar::Projectile::collisionRectangle() const {
    const units::Game width{vertical_dirction_ == VerticalFacing::HORIZONTAL ? 
            units::tileToGame(1) : 
            kProjectileWidth};
    const units::Game height{vertical_dirction_ != VerticalFacing::HORIZONTAL ? 
            units::tileToGame(1) : 
            kProjectileWidth};
    return Rectangle{getX() + units::kHalfTile - width / 2,
            getY() + units::kHalfTile - height /2,
            width, 
            height};
}

units::Game PolarStar::Projectile::getX() const{
    if (vertical_dirction_ == VerticalFacing::HORIZONTAL){
        return x_ + (horizontal_direction_ == HorizontalFacing::LEFT ? -offset_ : offset_);
    } 
    return x_;
}

units::Game PolarStar::Projectile::getY() const{
    units::Game y{y_};
    switch (vertical_dirction_){
        case VerticalFacing::UP:
            y -= offset_;
            break;
        case VerticalFacing::DOWN:
            y += offset_;
            break;
        default:
            break;
    }
    return y;
}



void PolarStar::Projectile::draw(Graphics& graphics) {
    sprite_ -> draw(graphics, getX(), getY());
}

