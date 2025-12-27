#ifndef VARYING_WIDTH_H_
#define VARYING_WIDTH_H_

#include "sprite.h"

class VaryingWidthSprite : public Sprite{
    public:
        VaryingWidthSprite(Graphics& graphics,
                const std::string& file_path,
                units::Pixel source_x,
                units::Pixel source_y,
                units::Pixel initial_width,
                units::Pixel initial_height) : 
            Sprite{graphics, file_path, source_x, source_y, initial_width, initial_height}{}

        void set_width(units::Pixel width) { src_rect_.w = width; }
}; 

#endif