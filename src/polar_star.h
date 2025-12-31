#ifndef POLAR_STAR_H_
#define POLAR_STAR_H_

#include "units.h"
#include "sprite.h"
#include "sprite_state.h"
#include "rectangle.h"

#include <map>
#include <memory>

class Map;
class Graphics;

class PolarStar{
    public:
        PolarStar(Graphics& graphcis);

        void updateProjectiles(units::MS elapsed_time, const Map& map);
        void draw(Graphics& graphics,
                units::Game player_x, 
                units::Game player_y, 
                HorizontalFacing horizontal_facing, 
                VerticalFacing vertical_facing,
                bool gun_up);

        void stopFire(){}
        void startFire(units::Game player_x,
                units::Game player_y,
                HorizontalFacing horizontal_facing, 
                VerticalFacing vertical_facing,
                bool gun_up);

    private:
        struct Projectile{
            Projectile(std::shared_ptr<Sprite> sprite,
                    HorizontalFacing horizontal_direction,
                    VerticalFacing vertical_direction,
                    units::Game x, units::Game y);

            // return true if alive
            bool update(units::MS elapsed_time, const Map& map);
            void draw(Graphics& graphics);

            private:
                Rectangle collisionRectangle() const;
                units::Game getX() const;
                units::Game getY() const;

                std::shared_ptr<Sprite> sprite_;
                const units::Game x_, y_;
                const HorizontalFacing horizontal_direction_;
                const VerticalFacing vertical_dirction_;
                units::Game offset_;
        };

        using SpriteTuple = std::tuple<HorizontalFacing, VerticalFacing>;
        struct SpriteState : public SpriteTuple{
            SpriteState(const SpriteTuple& tuple) : SpriteTuple{tuple} {}
            SpriteState(SpriteTuple&& tuple) : SpriteTuple{std::move(tuple)}{}
            HorizontalFacing horizontal_facing() const { return std::get<0>(*this); }
            VerticalFacing vertical_facing() const { return std::get<1>(*this); } 
        };

        units::Game gun_x(HorizontalFacing horizontal_facing, units::Game player_x) const {
            return horizontal_facing == HorizontalFacing::LEFT ? player_x - units::kHalfTile : player_x;
        }

        units::Game gun_y(VerticalFacing vertical_facing, units::Game player_y, bool gun_up) const;

        void initializeSprites(Graphics& graphics);
        void initializeSprite(Graphics& graphics, const SpriteState& sprite_state);
        std::map<SpriteState, std::shared_ptr<Sprite>> sprite_map_;
        std::shared_ptr<Sprite> horizontal_projectile_;
        std::shared_ptr<Sprite> vertical_projectile_;
        std::shared_ptr<Projectile> projectile_a_;
        std::shared_ptr<Projectile> projectile_b_;
};

#endif