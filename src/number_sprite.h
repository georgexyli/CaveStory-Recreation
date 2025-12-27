#ifndef NUMBER_SPRITE_H_
#define NUMBER_SPRITE_H_

#include <memory>
#include <vector>
#include "units.h"

class Sprite;
class Graphics;

class NumberSprite{
    public:
        static NumberSprite HUDNumber(Graphics& graphics, int number, int num_digits){
            return NumberSprite(graphics, number, num_digits, ColorType::WHITE, OperatorType::NONE);
        }

        static NumberSprite DamageNumber(Graphics& graphics, int number){
            return NumberSprite(graphics, number, 0, ColorType::RED, OperatorType::MINUS);
        }

        static NumberSprite ExperienceNumber(Graphics& graphics, int number){
            return NumberSprite(graphics, number, 0, ColorType::WHITE, OperatorType::PLUS);
        }

        void draw(Graphics& graphics, units::Game x, units::Game y) const;

        units::Game width() const { return units::kHalfTile * reverse_sprites_.size(); }
        units::Game height() const { return units::kHalfTile; }

    private:
        enum class ColorType{
            RED,
            WHITE
        };

        enum class OperatorType{
            PLUS,
            MINUS,
            NONE
        };

        NumberSprite(Graphics& graphics, int number, int num_digits, ColorType color, OperatorType op);

        units::Game padding_;
        std::vector<std::shared_ptr<Sprite>> reverse_sprites_;
        ColorType color_;
        OperatorType op_;

};

#endif