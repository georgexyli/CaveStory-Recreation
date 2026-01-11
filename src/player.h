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
#include "damage_text.h"
#include "polar_star.h"
#include "damageable.h"
#include "gun_experience_hud.h"

class Graphics;
class Map;
struct ParticleTools;

class Player : public Damageable{
    public:
        Player(units::Game x, units::Game y, Graphics& Graphics);

        void update(units::MS elapsed_time_ms, const Map& map, ParticleTools& particle_tools);
        void updateX(units::MS elapsed_time_ms, const Map& map);
        void updateY(units::MS elapsed_time_ms, const Map& map, ParticleTools& particle_tools);
        void draw(Graphics& graphics);
        void drawHUD(Graphics& graphics);

        void startMovingLeft();
        void startMovingRight();
        void stopMoving();

        void lookUp();
        void lookDown();
        void lookHorizontal();

        void startJump();
        void stopJump();

        void startFire(ParticleTools& particle_tools);
        void stopFire();

        void takeDamage(units::HP damage);

        Rectangle damageRectangle() const;
        units::Game center_x() const { return x_ + units::kHalfTile; }
        units::Game center_y() const { return y_ + units::kHalfTile; }
        std::shared_ptr<DamageText> get_damage_text() const { return damage_text_; }

        std::vector<std::shared_ptr<Projectile>> getProjectiles(){
            return polar_star_.getProjectiles();
        }
    private:
        enum class StrideType {
            FIRST_STRIDE_TYPE,
            MIDDLE = FIRST_STRIDE_TYPE,
            LEFT,
            RIGHT,
            LAST_STRIDE_TYPE
        };
        using SpriteTuple = std::tuple<MotionType, HorizontalFacing, VerticalFacing, StrideType>;
        struct SpriteState : public SpriteTuple{
            MotionType motion_type() const { return std::get<0>(*this); }
            HorizontalFacing horizontal_facing() const { return std::get<1>(*this); }
            VerticalFacing vertical_facing() const { return std::get<2>(*this); }
            StrideType strideType() const { return get<3>(*this); }

            SpriteState(const SpriteTuple& tuple) : SpriteTuple{tuple} {}     
            SpriteState(SpriteTuple&& tuple) : SpriteTuple{std::move(tuple)}{}       
        };

        struct WalkingAnimation{
            WalkingAnimation();

            void update();
            void reset();

            StrideType stride() const;
            private:
                Timer frame_timer_;
                int cur_index_;
                bool forward_;
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
        MotionType motionType() const;
        bool gun_up() const {
            return motionType() == MotionType::WALKING && (walking_animation_.stride() != StrideType::MIDDLE);
        }
        VerticalFacing vertical_facing() const 
            { return on_ground_ && intended_vertical_facing_ 
                    == VerticalFacing::DOWN ?
                    VerticalFacing::HORIZONTAL :
                    intended_vertical_facing_; }

        units::Game x_, y_;
        units::Velocity velocity_x_, velocity_y_;
        units::Accelaration acceleration_x_, accelaration_y_;
        bool on_ground_;
        bool jump_active_;
        bool interacting_;

        Health health_;
        Timer invincible_timer_;
        std::shared_ptr<DamageText> damage_text_;
        WalkingAnimation walking_animation_;

        GunExperienceHUD gun_experience_hud_;
        PolarStar polar_star_;

        HorizontalFacing horizontal_facing_;
        VerticalFacing intended_vertical_facing_;

        std::map<SpriteState, std::unique_ptr<Sprite>> sprites_;
        
};

#endif
