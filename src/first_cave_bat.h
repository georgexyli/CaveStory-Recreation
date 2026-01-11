#ifndef FIRST_CAVE_BAT_H_
#define FIRST_CAVE_BAT_H_

#include <memory>
#include <map>
#include "units.h"
#include "sprite_state.h"
#include "rectangle.h"
#include "damage_text.h"
#include "damageable.h"

class Sprite;
class Graphics;

class FirstCaveBat : public Damageable{
    public:
        FirstCaveBat(units::Game x, units::Game y, Graphics& graphics);
        void draw(Graphics& graphics);
        bool update(units::MS elapsed_time, units::Game player_x);

        Rectangle damageRectangle() const 
                { return Rectangle{x_ + units::kHalfTile,
                        y_ + units::kHalfTile, 0, 0}; }

        Rectangle collisionRectangle() const 
                { return Rectangle{x_, y_, units::tileToGame(1), units::tileToGame(1)}; }

        units::HP contactDamage() const; 
        void takeDamage(units::HP damage){
            damage_text_ -> setDamage(damage);
            alive_ = false;
        }

        units::Game center_x() const {
            return x_ + units::kHalfTile;
        }

        units::Game center_y() const {
            return y_ + units::kHalfTile;
        }
        
        std::shared_ptr<DamageText> get_damage_text() const { return damage_text_; }
    private:
        using SpriteTuple = std::tuple<HorizontalFacing>;
        struct SpriteState : public SpriteTuple{
            SpriteState(const SpriteTuple& tuple) : SpriteTuple{tuple}{}
            SpriteState(SpriteTuple&& tuple) : SpriteTuple{std::move(tuple)}{}
            HorizontalFacing facing() const { return std::get<0>(*this); };
        };
       
        void initializeSprites(Graphics& graphics);
        void initializeSprite(Graphics& graphics, const SpriteState& sprite);
        SpriteState getSpriteState() const;

        const units::Game flight_center_y_;
        bool alive_;
        units::Game x_, y_;
        units::Degrees flight_angle_;
        HorizontalFacing facing_;
        std::map<SpriteState, std::shared_ptr<Sprite>> sprites_;
        std::shared_ptr<DamageText> damage_text_;
};

#endif // FIRST_CAVE_BAT_H_