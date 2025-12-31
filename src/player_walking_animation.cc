#include "player.h"

namespace{
const units::Frame kWalkFrames{3};
const units::FPS kWalkFps{15};
}

Player::WalkingAnimation::WalkingAnimation() :
    frame_timer_{std::chrono::milliseconds{1000} / kWalkFps},
    cur_index_{0},
    forward_{true}{}

void Player::WalkingAnimation::reset(){
    forward_ = true;
    cur_index_ = 0;
    frame_timer_.reset();
}

void Player::WalkingAnimation::update(){
    if (frame_timer_.expired()) {
        frame_timer_.reset();

        if (forward_){
            ++cur_index_;
            forward_ = cur_index_ != kWalkFrames - 1;
        } else {
            --cur_index_;
            forward_ = cur_index_ == 0;
        }
    }

}

Player::StrideType Player::WalkingAnimation::stride() const {
    switch(cur_index_){
        case 0:
            return StrideType::LEFT;
        case 1:
            return StrideType::MIDDLE;
        case 2:
            return StrideType::RIGHT;
        default:
            return StrideType::MIDDLE;
    }
}