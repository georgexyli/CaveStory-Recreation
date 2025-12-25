#include "backdrop.h"
#include "game.h"

namespace{
const units::Tile kBackgroundSize{4}; 
}

FixedBackdrop::FixedBackdrop(const std::string& file_path, Graphics& graphics) :
        texture_id_{graphics.loadImage(file_path)}
{
}

void FixedBackdrop::draw(Graphics& graphics) const {
    for (units::Tile x{}; x < Game::kScreenWidth; x += kBackgroundSize){
        for (units::Tile y{}; y < Game::kScreenHeight; y += kBackgroundSize){
            SDL_Rect dst_rect{
                units::tileToPixel(x), 
                units::tileToPixel(y),
                units::tileToPixel(kBackgroundSize),
                units::tileToPixel(kBackgroundSize)
            };
            graphics.renderTexture(texture_id_, nullptr, &dst_rect);
        }
    }
}