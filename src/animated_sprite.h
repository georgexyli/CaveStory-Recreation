#ifndef ANIMATED_SPRITE_H_
#define ANIMATED_SPRITE_H_

#include "timer.h"

#include "sprite.h"

class AnimatedSprite : public Sprite {
    public:
        AnimatedSprite(Graphics& graphics, const std::string& file_path,
                units::Pixel source_x, units::Pixel source_y, units::Pixel width,
                units::Pixel height, units::FPS fps, units::Frame num_frames);

                
        void update() override;
        int num_completed_loops() const { return num_completed_loops_; }

    private:
        int num_completed_loops_;
        const units::Frame num_frames_;
        units::Frame cur_frame_;
        Timer frame_timer_;
};

#endif // ANIMATED_SPRITE_H_