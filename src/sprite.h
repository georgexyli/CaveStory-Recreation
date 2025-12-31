#ifndef SPRITE_H_
#define SPRITE_H_

#include <SDL.h>

#include <string>

#include "graphics.h"
#include "units.h"

class Sprite {
    public:
        Sprite(Graphics& graphics, const std::string& file_path, units::Pixel source_x,
                units::Pixel source_y, units::Pixel width, units::Pixel height);

        virtual ~Sprite() = default;

        virtual void update() {}
        void draw(Graphics& graphics, units::Game x, units::Game y);

    private:
        SDL_Texture* sprite_texture_;

    protected:
        SDL_Rect src_rect_;
};

#endif // SPRITE_H_