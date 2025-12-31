#include "graphics.h"
#include <iostream>
#include "game.h"

Graphics::Graphics()
        : window_{SDL_CreateWindow(
                "Cave Story", 
                SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED, 
                units::tileToPixel(Game::kScreenWidth),
                units::tileToPixel(Game::kScreenHeight), 
                SDL_WINDOW_FULLSCREEN)},
        renderer_{SDL_CreateRenderer(
                window_,
                -1, 
                SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE)} {
    SDL_RenderSetLogicalSize(renderer_, units::tileToPixel(Game::kScreenWidth), units::tileToPixel(Game::kScreenHeight));
}

Graphics::~Graphics() {
    for (auto& sprite_texture : sprite_sheets_){
        SDL_DestroyTexture(sprite_texture.second);
    }
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
}

void Graphics::renderTexture(SDL_Texture* texture, SDL_Rect* src_rect,
        SDL_Rect* dst_rect) {
    SDL_RenderCopy(renderer_, texture, src_rect, dst_rect);
}

void Graphics::flip() { SDL_RenderPresent(renderer_); }

SDL_Texture* Graphics::loadImage(const std::string& file_name, bool black_is_transparent) {
    const std::string file_path{ config::getGraphicsQuality() == config::GraphicsQuality::HIGH_QUALITY ?
            "content/" + file_name + ".bmp" :
            "content/" + file_name + ".pbm" };
    if (sprite_sheets_.contains(file_path)){
        return sprite_sheets_[file_path];
    }
    SDL_Surface* temp_surface = SDL_LoadBMP(file_path.c_str());
    if (black_is_transparent){
        SDL_SetColorKey(temp_surface, SDL_TRUE,
                SDL_MapRGB(temp_surface->format, 0, 0, 0));
    }

    sprite_sheets_.emplace(file_path, SDL_CreateTextureFromSurface(renderer_, temp_surface));
    SDL_FreeSurface(temp_surface);
    
    return sprite_sheets_[file_path];
}

void Graphics::clear() {
    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
    SDL_RenderClear(renderer_);
}
