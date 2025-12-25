#include "sprite.h"

#include <iostream>

Sprite::Sprite(Graphics& graphics, const std::string& file_path,
        units::Pixel source_x, units::Pixel source_y, units::Pixel width,
        units::Pixel height)
        : sprite_texture_{graphics.loadImage(file_path, true)} ,
        src_rect_{source_x, source_y, width, height} 
{
}

void Sprite::draw(Graphics& graphics, units::Game x, units::Game y) {
    SDL_Rect destination_rect{
            units::gameToPixel(x),
            units::gameToPixel(y),
            src_rect_.w,
            src_rect_.h};
    graphics.renderTexture(sprite_texture_, &src_rect_, &destination_rect);
}