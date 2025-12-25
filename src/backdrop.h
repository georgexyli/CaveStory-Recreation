#ifndef BACKDROP_H_
#define BACKDROP_H_

#include "graphics.h"
#include <string>

class Backdrop{
    public:
        virtual ~Backdrop() = default;
        virtual void draw(Graphics& graphics) const = 0;
};

class FixedBackdrop : public Backdrop{
    public:
        FixedBackdrop(const std::string& file_path, Graphics& graphics);
        void draw(Graphics& graphics) const override;
    private:
        SDL_Texture* texture_id_;
};

#endif // BACKDROP_H_