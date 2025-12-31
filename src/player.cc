#include "player.h"

#include "graphics.h"
#include "game.h"
#include "map.h" 
#include "number_sprite.h"

#include <assert.h>
#include <iostream>

namespace {
const std::string kSpriteFile{"MyChar"};
const std::string kTextBoxFile{"TextBox"};

// movement constants
const units::Accelaration kWalkingAcceleration{0.00083007812f}; 
const units::Velocity kMaxSpeedX{0.15859375f}; 
const units::Accelaration kFriction{0.00049804687f};

const units::Velocity kJumpSpeed{0.25f}; 
const units::Velocity kShortJumpSpeed{kJumpSpeed / 1.5f};
const units::Accelaration kAirAccelerataion{0.0003125f};
const units::Accelaration kJumpGravity{0.0003125f};

const units::Velocity kMaxSpeedY{0.2998046875f};
const units::Accelaration kGravity{0.00078125f};

// character frame constants
const units::Frame kJumpFrame{1};
const units::Frame kFallFrame{2};
const units::Frame kWalkFrame{0};
const units::Frame kStandFrame{0};
const units::Frame kCharacterFrame{0};
const units::Frame kUpFrameOffset{3};
const units::Frame kBackFrame{7};
const units::Frame kDownFrame{6};

const Rectangle kCollisionX{6, 10, 20, 12};
const Rectangle kCollisionY{10, 2, 12, 30};

const units::MS kInvincibleFlashTime{50};
const units::MS kInvincibleTime{3000};

struct CollisionInfo{
    bool collided;
    units::Tile row, col;
};

CollisionInfo getWallCollisionInfo(const Map& map, const Rectangle& rectangle){
    CollisionInfo info{false, 0, 0};
    std::vector<Map::CollisionTile> tiles{map.getCollidingTiles(rectangle)};
    for (auto& tile: tiles){
        if (tile.tile_type == Map::TileType::WALL_TILE){
            info = {true, tile.row, tile.col};
            break;
        }
    } 
    return info;
}
}

Player::Player(units::Game x, units::Game y, Graphics& graphics) : x_{x}, y_{y},
        velocity_x_{0.0f}, 
        velocity_y_{0.0f},
        acceleration_x_{0.0f}, 
        on_ground_{false},
        horizontal_facing_{HorizontalFacing::LEFT},
        intended_vertical_facing_{VerticalFacing::HORIZONTAL},
        jump_active_{false}, 
        interacting_{false},
        health_{graphics}, 
        invincible_timer_{kInvincibleTime},
        polar_star_{graphics}
{
    initializeSprites(graphics);
}

void Player::update(units::MS elapsed_time_ms, const Map& map){
    sprites_[getSpriteState()] -> update(elapsed_time_ms);

    health_.update(elapsed_time_ms);
    damage_text_.update(elapsed_time_ms);
    walking_animation_.update();
    updateX(elapsed_time_ms, map);
    updateY(elapsed_time_ms, map);
}

void Player::updateX(units::MS elapsed_time_ms, const Map& map){
    velocity_x_ += acceleration_x_ * elapsed_time_ms.count();
    if (acceleration_x_ < 0.0f){
        velocity_x_ = std::max(velocity_x_, -kMaxSpeedX);
    } else if (acceleration_x_ > 0.0f){
        velocity_x_ = std::min(velocity_x_, kMaxSpeedX);
    } else if (on_ground_) {
        velocity_x_ = velocity_x_ > 0.0f ? 
                std::max(0.0f, velocity_x_ - kFriction * elapsed_time_ms.count()):
                std::min(0.0f, velocity_x_ + kFriction * elapsed_time_ms.count());
    }

    const units::Game delta{velocity_x_ * elapsed_time_ms.count()};

    if (delta > 0.0f){
        CollisionInfo info{getWallCollisionInfo(map, rightCollision(delta))};

        if (info.collided){
            x_ = units::tileToGame(info.col) - kCollisionX.right();
            velocity_x_ = 0.0f;
        } else {
            x_ += delta;
        }

        info = getWallCollisionInfo(map, leftCollision(0));
        if (info.collided) {
            x_ = units::tileToGame(info.col) + kCollisionX.right();
        } 
    } else {
        CollisionInfo info{getWallCollisionInfo(map, leftCollision(delta))};

        if (info.collided){
            x_ = units::tileToGame(info.col) + kCollisionX.right();
            velocity_x_ = 0.0f;
        } else {
            x_ += delta;
        }

        info = getWallCollisionInfo(map, rightCollision(0));
        if (info.collided) {
            x_ = units::tileToGame(info.col) - kCollisionX.right();
        } 
    }
}

void Player::updateY(units::MS elapsed_time_ms, const Map& map){
    const units::Accelaration gravity{jump_active_ && velocity_y_ < 0.0f ?
            kJumpGravity : kGravity};
    velocity_y_ = std::min(velocity_y_ + gravity * elapsed_time_ms.count(),
            kMaxSpeedY);

    const units::Game delta{velocity_y_ * elapsed_time_ms.count()};
    if (delta > 0.0f){
        CollisionInfo info{getWallCollisionInfo(map, bottomCollision(delta))};

        on_ground_ = info.collided;
        if (info.collided) {
            y_ = units::tileToGame(info.row) - kCollisionY.bottom();
            velocity_y_ = 0.0f;
        } else {
            y_ += delta;
        }

        info = getWallCollisionInfo(map, topCollision(0));
        if (info.collided) {
            y_ = units::tileToGame(info.row) + kCollisionY.height();
        } 
    } else {
        CollisionInfo info{getWallCollisionInfo(map, topCollision(delta))};

        if (info.collided) {
            y_ = units::tileToGame(info.row) + kCollisionY.height();
            velocity_y_ = 0.0f;
        } else {
            y_ += delta;
            on_ground_ = false;
        }

        info = getWallCollisionInfo(map, bottomCollision(0));
        if (info.collided) {
            y_ = units::tileToGame(info.row) - kCollisionY.bottom();
            on_ground_ = true;
        } 
    } 
}

void Player::draw(Graphics& graphics) {
    if (spriteIsVisible()){
        const bool gun_up = motionType() == MotionType::WALKING && (walking_animation_.stride() != StrideType::MIDDLE);
        polar_star_.draw(graphics, x_, y_, horizontal_facing_, vertical_facing(), gun_up);
        (*sprites_.find(getSpriteState())).second -> draw(graphics, x_, y_);
    }
}

void Player::drawHUD(Graphics& graphics) {
    if (spriteIsVisible()){
        health_.draw(graphics);
    }
    damage_text_.draw(graphics, center_x(), center_y());
}

void Player::startMovingLeft(){
    if (on_ground_ && acceleration_x_ == 0) walking_animation_.reset();

    acceleration_x_ = on_ground_ ?  -kWalkingAcceleration : -kAirAccelerataion;
    horizontal_facing_ = HorizontalFacing::LEFT;
    interacting_ = false;
}

void Player::startMovingRight(){
    if (on_ground_ && acceleration_x_ == 0) walking_animation_.reset();
    acceleration_x_ = on_ground_ ? kWalkingAcceleration : kAirAccelerataion;
    horizontal_facing_ = HorizontalFacing::RIGHT;
    interacting_ = false;
}

void Player::stopMoving(){
    acceleration_x_ = 0.0f;
}

void Player::lookUp(){
    intended_vertical_facing_ = VerticalFacing::UP;
    interacting_ = false;
}

void Player::lookDown(){
    if (intended_vertical_facing_ == VerticalFacing::DOWN) return;
    intended_vertical_facing_ = VerticalFacing::DOWN;
    interacting_ = on_ground_;
}

void Player::lookHorizontal(){
    intended_vertical_facing_ = VerticalFacing::HORIZONTAL;
}

void Player::stopJump(){
    jump_active_ = false;
}


void Player::startJump(){
    interacting_ = false;
    jump_active_ = true;
    if (on_ground_){
        velocity_y_ = -kJumpSpeed;
    } 
}

void Player::takeDamage(units::HP damage){
    if (invincible_timer_.active()) return;
    health_.takeDamage(damage);
    damage_text_.setDamage(damage);

    velocity_y_ = std::min(-kShortJumpSpeed, velocity_y_);
    invincible_timer_.reset();
}

Rectangle Player::damageRectangle() const {
    return Rectangle{x_ + kCollisionX.left(),
            y_ + kCollisionY.top(),
            kCollisionX.width(),
            kCollisionY.width()};
}

void Player::initializeSprites(Graphics& graphics){
    ENUM_FOREACH(mt, MOTION_TYPE, MotionType){
        ENUM_FOREACH(hf, HORIZONTAL_TYPE, HorizontalFacing){
            ENUM_FOREACH(vf, VERTICAL_TYPE, VerticalFacing){
                ENUM_FOREACH(st, STRIDE_TYPE, StrideType){
                    initializeSprite(graphics, std::make_tuple(
                            static_cast<MotionType>(mt), 
                            static_cast<HorizontalFacing>(hf), 
                            static_cast<VerticalFacing>(vf),
                            static_cast<StrideType>(st)));
                }
            }
        }
    }
}

void Player::initializeSprite(Graphics& graphics, const SpriteState& state){
    units::Tile tile_y{state.horizontal_facing() == HorizontalFacing::LEFT ? 
            kCharacterFrame : 
            kCharacterFrame + 1};

    units::Tile tile_x;
    switch (state.motion_type()){
        case MotionType::WALKING:
            tile_x = kWalkFrame;
            break;
        case MotionType::STANDING:
            tile_x = kStandFrame;
            break;
        case MotionType::INTERACTING:
            tile_x = kBackFrame;
            break;
        case MotionType::JUMPING:
            tile_x = kJumpFrame;
            break;
        case MotionType::FALLING:
            tile_x = kFallFrame;
            break;
        default:
            break;
    }
    switch (state.vertical_facing()) {
        case VerticalFacing::UP:
            tile_x += kUpFrameOffset;
            break;
        case VerticalFacing::DOWN:
            tile_x = kDownFrame;
            break;
        default:
            break;
    }

    if (state.motion_type() == MotionType::WALKING){
        switch (state.strideType()){
            case StrideType::LEFT:
                tile_x += 1;
                break;
            case StrideType::RIGHT:
                tile_x += 2;
                break;
            default:
                break;
        }
        sprites_.emplace(state, std::make_unique<Sprite>(
                        graphics, 
                        kSpriteFile, 
                        units::tileToPixel(tile_x), 
                        units::tileToPixel(tile_y), 
                        units::tileToPixel(1),
                        units::tileToPixel(1)));
    } else {
        sprites_.emplace(state, std::make_unique<Sprite>(graphics, kSpriteFile,
                        units::tileToPixel(tile_x), 
                        units::tileToPixel(tile_y), 
                        units::tileToPixel(1),
                        units::tileToPixel(1)));
    }
}

MotionType Player::motionType() const {
    MotionType motion;  
    if (interacting_){
        motion = MotionType::INTERACTING;
    }
    else if (on_ground_){
        motion = acceleration_x_ == 0.0f ? MotionType::STANDING : MotionType::WALKING;
    } else{
        motion = velocity_y_ < 0.0f ? MotionType::JUMPING : MotionType::FALLING;
    }   
    return motion;
}

Player::SpriteState Player::getSpriteState() const{
    return SpriteState{std::make_tuple(
            motionType(), 
            horizontal_facing_, 
            vertical_facing(), 
            walking_animation_.stride())};
}

Rectangle Player::leftCollision(units::Game delta) const {
    assert(delta <= 0);
    return Rectangle(x_ + kCollisionX.left() + delta,
            y_ + kCollisionX.top(),
            kCollisionX.width() / 2 - delta,
            kCollisionX.height());
}

Rectangle Player::rightCollision(units::Game delta) const {
    assert(delta >= 0);
    return Rectangle(x_ + kCollisionX.left() + kCollisionX.width() / 2,
            y_ + kCollisionX.top(),
            kCollisionX.width() / 2 + delta,
            kCollisionX.height());
}

Rectangle Player::topCollision(units::Game delta) const {
    assert(delta <= 0);
    return Rectangle(x_ + kCollisionY.left(),
            y_ + kCollisionY.top() + delta,
            kCollisionY.width(),
            kCollisionY.height() / 2 - delta);
}

Rectangle Player::bottomCollision(units::Game delta) const {
    assert(delta >= 0);
    return Rectangle(x_ + kCollisionY.left(),
            y_ + kCollisionY.top() + kCollisionY.height() / 2,
            kCollisionY.width(),
            kCollisionY.height() / 2 + delta);
}

bool Player::spriteIsVisible() const{
    return !(invincible_timer_.active() && invincible_timer_.current_time() / kInvincibleFlashTime % 2 == 0); 
}