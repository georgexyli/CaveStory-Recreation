#ifndef PLAYER_H_
#define PLAYER_H_

#include <memory>
#include <map>
#include "sprite_state.h"
#include "animated_sprite.h"
#include "rectangle.h"
#include "units.h"
#include "number_sprite.h"
#include "varying_width_sprite.h"
#include "timer.h"

class Graphics;
class Map;

class Player{
    public:
        Player(units::Game x, units::Game y, Graphics& Graphics);

        void update(units::MS elapsed_time_ms, const Map& map);
        void updateX(units::MS elapsed_time_ms, const Map& map);
        void updateY(units::MS elapsed_time_ms, const Map& map);
        void draw(Graphics& graphics) const;
        void drawHUD(Graphics& graphics);

        void startMovingLeft();
        void startMovingRight();
        void stopMoving();

        void lookUp();
        void lookDown();
        void lookHorizontal();

        void startJump();
        void stopJump();
        void takeDamage();

        Rectangle damageRectangle() const;
        units::Game center_x() const { return x_ + units::kHalfTile; }

    private:
        struct SpriteState {
            MotionType motion_type;
            HorizontalFacing horizontal_facing;
            VerticalFacing vertical_facing;

            SpriteState(MotionType motion_type = MotionType::STANDING, 
                    HorizontalFacing horizontal_facing = HorizontalFacing::LEFT,
                    VerticalFacing vertical_facing = VerticalFacing::HORIZONTAL);
            
            bool operator<(const SpriteState& other) const;
        };

        struct Health {
            Health(Graphics& graphics);

            void update(units::MS elapsed_time);
            void draw(Graphics& graphics);
            // returns true if we have died
            bool takeDamage(units::HP damage);
            private:
                units::Game fillOffset(units::HP health) const;
                units::HP damage_;
                Timer damage_timer_;

                units::HP max_health_;
                units::HP current_health_;
                Sprite health_bar_sprite_;
                VaryingWidthSprite health_fill_sprite_;
                VaryingWidthSprite damage_fill_sprite_;
        };

        void initializeSprites(Graphics& graphics);
        void initializeSprite(Graphics& graphics, const SpriteState& sprite);
        SpriteState getSpriteState() const;


        Rectangle leftCollision(units::Game delta) const;
        Rectangle rightCollision(units::Game delta) const;
        Rectangle bottomCollision(units::Game delta) const;
        Rectangle topCollision(units::Game delta) const;

        bool spriteIsVisible() const;
        bool onGround() const{ return on_ground_; }

        units::Game x_, y_;
        units::Velocity velocity_x_, velocity_y_;
        units::Accelaration acceleration_x_, accelaration_y_;
        bool on_ground_;
        bool jump_active_;
        bool interacting_;

        Health health_;
        Timer invincible_timer_;

        HorizontalFacing horizontal_facing_;
        VerticalFacing vertical_facing_;

        std::map<SpriteState, std::unique_ptr<Sprite>> sprites_;
};

#endif
