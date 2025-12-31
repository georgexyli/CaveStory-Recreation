#include "animated_sprite.h"
#include "game.h"

AnimatedSprite::AnimatedSprite(Graphics& graphics, const std::string& file_path,
        units::Pixel source_x, units::Pixel source_y,
        units::Pixel width, units::Pixel height, units::FPS fps,
        units::Frame num_frames) : Sprite{graphics, file_path, 
        source_x, source_y, 
        width, height},
        frame_timer_{units::MS{1000 / fps}},
        num_frames_{num_frames},
        cur_frame_{0}
{
}

void AnimatedSprite::update() {
    if (frame_timer_.expired()){
        frame_timer_.reset();
        ++cur_frame_;
        if (cur_frame_ < num_frames_){
            src_rect_.x += src_rect_.w;
        } else {
            src_rect_.x -= (num_frames_-1) * src_rect_.w;
            cur_frame_ = 0;
        }
    }
}