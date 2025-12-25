#include "player.h"
#include "graphics.h"
#include "game.h"
#include "map.h"
#include <assert.h>
#include <iostream>

namespace {
const std::string kSpriteFile{"content/MyChar.pbm"};
const std::string kTextBoxFile{"content/TextBox.pbm"};

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

const units::Frame kWalkFrames{3};
const units::FPS kWalkFps{15};

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

// HUD Constants
const units::Game kHealthBarX{units::tileToGame(1)};
const units::Game kHealthBarY{units::tileToGame(2)};
const units::Game kHealthBarSourceX{0};
const units::Game kHealthBarSourceY{5 * units::kHalfTile};
const units::Game kHealthBarSourceWidth{units::tileToGame(4)};
const units::Game kHealthBarSourceHeight{units::kHalfTile};

const units::Game kHealthFillX{5 * units::kHalfTile};
const units::Game kHealthFillY{units::tileToGame(2)};

const units::Game kHealthFillSourceX{0};
const units::Game kHealthFillSourceY{3 * units::kHalfTile};
const units::Game kHealthFillSourceWidth{5 * units::kHalfTile - 2.0f};
const units::Game kHealthFillSourceHeight{units::kHalfTile};

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
        velocity_x_{0.0f}, velocity_y_{0.0f},
        acceleration_x_{0.0f}, on_ground_{false},
        horizontal_facing_{HorizontalFacing::LEFT},
        vertical_facing_{VerticalFacing::HORIZONTAL},
        jump_active_{false}, interacting_{false},
        invincible_{false}, invincible_time_{0}
{
    initializeSprites(graphics);
}

void Player::update(units::MS elapsed_time_ms, const Map& map){
    sprites_[getSpriteState()] -> update(elapsed_time_ms);

    if (invincible_){
        invincible_time_ += elapsed_time_ms;
        invincible_ = invincible_time_ < kInvincibleTime;
    }

    updateX(elapsed_time_ms, map);
    updateY(elapsed_time_ms, map);
}

void Player::updateX(units::MS elapsed_time_ms, const Map& map){
    velocity_x_ += acceleration_x_ * elapsed_time_ms;
    if (acceleration_x_ < 0.0f){
        velocity_x_ = std::max(velocity_x_, -kMaxSpeedX);
    } else if (acceleration_x_ > 0.0f){
        velocity_x_ = std::min(velocity_x_, kMaxSpeedX);
    } else if (on_ground_) {
        velocity_x_ = velocity_x_ > 0.0f ? 
                std::max(0.0f, velocity_x_ - kFriction * elapsed_time_ms):
                std::min(0.0f, velocity_x_ + kFriction * elapsed_time_ms);
    }

    const units::Game delta = velocity_x_ * elapsed_time_ms;

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
    const units::Accelaration gravity = jump_active_ && velocity_y_ < 0.0f ?
            kJumpGravity : kGravity;
    velocity_y_ = std::min(velocity_y_ + gravity * elapsed_time_ms,
            kMaxSpeedY);

    const units::Game delta = velocity_y_ * elapsed_time_ms;
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

void Player::draw(Graphics& graphics) const {
    if (spriteIsVisible()){
        (*sprites_.find(getSpriteState())).second -> draw(graphics, x_, y_);
    }
}

void Player::drawHUD(Graphics& graphics) const {
    if (spriteIsVisible()){
        health_bar_sprite_ -> draw(graphics, kHealthBarX, kHealthBarY);
        health_fill_sprite_ -> draw(graphics, kHealthFillX, kHealthFillY);
        three_ -> draw(graphics, units::tileToGame(2), units::tileToGame(2));
    }
}

void Player::startMovingLeft(){
    acceleration_x_ = on_ground_ ?  -kWalkingAcceleration : -kAirAccelerataion;
    horizontal_facing_ = HorizontalFacing::LEFT;
    interacting_ = false;
}

void Player::startMovingRight(){
    acceleration_x_ = on_ground_ ? kWalkingAcceleration : kAirAccelerataion;
    horizontal_facing_ = HorizontalFacing::RIGHT;
    interacting_ = false;
}

void Player::stopMoving(){
    acceleration_x_ = 0.0f;
}

void Player::lookUp(){
    vertical_facing_ = VerticalFacing::UP;
    interacting_ = false;
}

void Player::lookDown(){
    if (vertical_facing_ == VerticalFacing::DOWN) return;
    vertical_facing_ = VerticalFacing::DOWN;
    interacting_ = on_ground_;
}

void Player::lookHorizontal(){
    vertical_facing_ = VerticalFacing::HORIZONTAL;
}

void Player::stopJump(){
    jump_active_ = false;
}


void Player::startJump(){
    interacting_ = false;
    jump_active_ = true;
    if (onGround()){
        velocity_y_ = -kJumpSpeed;
    } 
}

void Player::takeDamage(){
    if (invincible_) return;
    velocity_y_ = std::min(-kShortJumpSpeed, velocity_y_);
    invincible_ = true;
    invincible_time_ = 0;
}

Rectangle Player::damageRectangle() const {
    return Rectangle{x_ + kCollisionX.left(),
            y_ + kCollisionY.top(),
            kCollisionX.width(),
            kCollisionY.width()};
}

void Player::initializeSprites(Graphics& graphics){
    health_bar_sprite_ = std::make_unique<Sprite>(
            graphics,
            kTextBoxFile,
            units::gameToPixel(kHealthBarSourceX),
            units::gameToPixel(kHealthBarSourceY),
            units::gameToPixel(kHealthBarSourceWidth),
            units::gameToPixel(kHealthBarSourceHeight));
    
    health_fill_sprite_ = std::make_unique<Sprite>(
            graphics,
            kTextBoxFile,
            units::gameToPixel(kHealthFillSourceX),
            units::gameToPixel(kHealthFillSourceY),
            units::gameToPixel(kHealthFillSourceWidth),
            units::gameToPixel(kHealthFillSourceHeight));
            
    three_ = std::make_unique<Sprite>(
            graphics,
            kTextBoxFile,
            units::gameToPixel(3 * units::kHalfTile),
            units::gameToPixel(7 * units::kHalfTile),
            units::gameToPixel(units::kHalfTile),
            units::gameToPixel(units::kHalfTile)); 

    for (int mt{static_cast<int>(MotionType::FIRST_MOTION_TYPE)}; 
            mt < static_cast<int>(MotionType::LAST_MOTION_TYPE); ++mt){
        for (int hf{static_cast<int>(HorizontalFacing::FIRST_HORIZONTAL_TYPE)};
                hf < static_cast<int>(HorizontalFacing::LAST_HORIZONTAL_TYPE); ++hf){
            for (int vf{static_cast<int>(VerticalFacing::FIRST_VERTICAL_TYPE)};
                    vf < static_cast<int>(VerticalFacing::LAST_VERTICAL_TYPE); ++vf){
                initializeSprite(graphics, SpriteState(
                        static_cast<MotionType>(mt), 
                        static_cast<HorizontalFacing>(hf), 
                        static_cast<VerticalFacing>(vf)));
            }
        }
    }
}

void Player::initializeSprite(Graphics& graphics, const SpriteState& state){
    units::Tile tile_y{state.horizontal_facing == HorizontalFacing::LEFT ? 
            kCharacterFrame : 
            kCharacterFrame + 1};

    units::Tile tile_x;
    switch (state.motion_type){
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

    tile_x += state.vertical_facing == VerticalFacing::UP ? 
            kUpFrameOffset : 0;

    if (state.motion_type == MotionType::WALKING){
        sprites_[state] = 
                std::make_unique<AnimatedSprite>(
                        graphics, 
                        kSpriteFile, 
                        units::tileToPixel(tile_x), 
                        units::tileToPixel(tile_y), 
                        units::tileToPixel(1),
                        units::tileToPixel(1),
                        kWalkFps, kWalkFrames);
    } else {
        if (state.vertical_facing == VerticalFacing::DOWN && 
                (state.motion_type == MotionType::JUMPING ||
                state.motion_type == MotionType::FALLING )){
            tile_x = kDownFrame;
        }
        sprites_[state] = 
                std::make_unique<Sprite>(graphics, kSpriteFile,
                        units::tileToPixel(tile_x), 
                        units::tileToPixel(tile_y), 
                        units::tileToPixel(1),
                        units::tileToPixel(1));
    }
}

Player::SpriteState Player::getSpriteState() const{
    MotionType motion;
    if (interacting_){
        motion = MotionType::INTERACTING;
    }
    else if (on_ground_){
        motion = acceleration_x_ == 0.0f ? MotionType::STANDING : MotionType::WALKING;
    } else{
        motion = velocity_y_ < 0.0f ? MotionType::JUMPING : MotionType::FALLING;
    }   
    return SpriteState{motion, horizontal_facing_, vertical_facing_};
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

Player::SpriteState::SpriteState(MotionType motion_type, 
        HorizontalFacing horizontal_facing, VerticalFacing vertical_facing) : 
        motion_type{motion_type}, horizontal_facing{horizontal_facing},
        vertical_facing{vertical_facing}
{
}

bool Player::SpriteState::operator<(const SpriteState& other) const{
    if (motion_type != other.motion_type) {
        return motion_type < other.motion_type;
    }
    else if (horizontal_facing != other.horizontal_facing){
        return horizontal_facing < other.horizontal_facing;
    } 
    return vertical_facing < other.vertical_facing;
}

bool Player::spriteIsVisible() const{
    return !(invincible_ && invincible_time_ / kInvincibleFlashTime % 2 == 0); 
}