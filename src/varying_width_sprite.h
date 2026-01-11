#ifndef VARYING_WIDTH_H_
#define VARYING_WIDTH_H_

#include "sprite.h"
#include <assert.h>

class VaryingWidthSprite : public Sprite{
    public:
        VaryingWidthSprite(Graphics& graphics,
                const std::string& file_path,
                units::Pixel source_x,
                units::Pixel source_y,
                units::Pixel max_width,
                units::Pixel initial_width,
                units::Pixel initial_height) : 
            Sprite{graphics, file_path, source_x, source_y, initial_width, initial_height},
            max_width_{max_width}{}

        // percentage from 0 to 1 inclusive
        void set_percentage_width(float percentage) { 
            assert(percentage >= 0.0f && percentage <= 1.0f);
            src_rect_.w = static_cast<unsigned int> (percentage * max_width_); 
        }
    private:
        const units::Pixel max_width_;
}; 

#endif