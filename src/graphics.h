#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include <SDL.h>
#include <map>

class Graphics {

    public:
        Graphics();
        ~Graphics();

        void renderTexture(SDL_Texture* src, SDL_Rect* src_rect, SDL_Rect* dst_rect);
        SDL_Texture* loadImage(const std::string& file_name, bool black_is_transparent = false);
        void clear();
        void flip();

    private:
        SDL_Window* window_;
        SDL_Renderer* renderer_;

        std::map<std::string, SDL_Texture*> sprite_sheets_;
};

#endif  // GRAPHICS_H_