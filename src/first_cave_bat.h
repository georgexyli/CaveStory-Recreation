#ifndef FIRST_CAVE_BAT_H_
#define FIRST_CAVE_BAT_H_

#include <memory>
#include <map>
#include "units.h"
#include "sprite_state.h"
#include "rectangle.h"

class Sprite;
class Graphics;

class FirstCaveBat{
    public:
        FirstCaveBat(units::Game x, units::Game y, Graphics& graphics);
        void draw(Graphics& graphics) const;
        void update(units::MS elapsed_time, units::Game player_x);

        Rectangle damageRectangle() const 
                { return Rectangle{x_ + units::kHalfTile,
                        y_ + units::kHalfTile, 0, 0}; }

    private:
        struct SpriteState{
            SpriteState(HorizontalFacing facing) : facing{facing}{}
            HorizontalFacing facing;

            bool operator<(const SpriteState& other) const{
                return facing < other.facing;
            }
        };

        void initializeSprites(Graphics& graphics);
        void initializeSprite(Graphics& graphics, const SpriteState& sprite);
        SpriteState getSpriteState() const;

        const units::Game center_y_;
        units::Game x_, y_;
        units::Degrees flight_angle_;
        HorizontalFacing facing_;
        std::map<SpriteState, std::shared_ptr<Sprite>> sprites_;
};

#endif // FIRST_CAVE_BAT_H_