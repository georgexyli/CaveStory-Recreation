#ifndef POLAR_STAR_H_
#define POLAR_STAR_H_

#include "units.h"
#include "sprite.h"
#include "sprite_state.h"

#include <map>
#include <memory>

class Graphics;

class PolarStar{
    public:
        PolarStar(Graphics& graphcis);

        void draw(Graphics& graphics,
                units::Game x, 
                units::Game y, 
                HorizontalFacing horizontal_facing, 
                VerticalFacing vertical_facing,
                bool gun_up);

    private:
        using SpriteTuple = std::tuple<HorizontalFacing, VerticalFacing>;
        struct SpriteState : public SpriteTuple{
            SpriteState(const SpriteTuple& tuple) : SpriteTuple{tuple} {}
            SpriteState(SpriteTuple&& tuple) : SpriteTuple{std::move(tuple)}{}
            HorizontalFacing horizontal_facing() const { return std::get<0>(*this); }
            VerticalFacing vertical_facing() const { return std::get<1>(*this); } 
        };

        void initializeSprites(Graphics& graphics);
        void initializeSprite(Graphics& graphics, const SpriteState& sprite_state);
        std::map<SpriteState, std::shared_ptr<Sprite>> sprite_map_;
};

#endif