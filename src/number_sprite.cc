#include "number_sprite.h"
#include "sprite.h"

#include <assert.h>

namespace{
const std::string kNumberFile{"content/TextBox.pbm"};
const units::Game kSourceWhiteY{7 * units::kHalfTile};
const units::Game kSourceRedY{8 * units::kHalfTile};
const units::Game kDigitSizeX{units::kHalfTile};
const units::Game kSourceWidth{units::kHalfTile};
const units::Game kSourceHeight{units::kHalfTile};

const units::Game kPlusSourceX{4 * units::kHalfTile};
const units::Game kMinusSourceX{5 * units::kHalfTile};
const units::Game kOpSourceY{6 * units::kHalfTile};
}

NumberSprite::NumberSprite(Graphics& graphics, int number, int num_digits, ColorType color, OperatorType op) :
        color_{color},
        op_{op}
{
    assert(number >= 0);
    const units::Game kNumberSourceY = color == ColorType::RED ? kSourceRedY : kSourceWhiteY;
    int digit_count{};
    do {
        const int digit = number % 10;

        reverse_sprites_.push_back(std::make_shared<Sprite>(
                graphics, 
                kNumberFile,
                units::gameToPixel(digit * kDigitSizeX),
                units::gameToPixel(kNumberSourceY),
                units::gameToPixel(kSourceWidth),
                units::gameToPixel(kSourceHeight)));
        
        number /= 10;
        ++digit_count;
    } while(number > 0);

    assert(num_digits == 0 || num_digits >= digit_count);
    padding_ = num_digits == 0 ? 0.0f : units::kHalfTile * (num_digits - digit_count);
    switch (op){
        case OperatorType::PLUS: 
            reverse_sprites_.push_back(std::make_shared<Sprite>(
                    graphics, 
                    kNumberFile,
                    units::gameToPixel(kPlusSourceX),
                    units::gameToPixel(kOpSourceY),
                    units::gameToPixel(kSourceWidth),
                    units::gameToPixel(kSourceHeight)));
            break;
        case OperatorType::MINUS:
            reverse_sprites_.push_back(std::make_shared<Sprite>(
                    graphics, 
                    kNumberFile,
                    units::gameToPixel(kMinusSourceX),
                    units::gameToPixel(kOpSourceY),
                    units::gameToPixel(kSourceWidth),
                    units::gameToPixel(kSourceHeight)));
            break;
        default:
            break;
    }
}

void NumberSprite::draw(Graphics& graphics, units::Game x, units::Game y) const{
    for (auto i{0uz}; i < reverse_sprites_.size(); ++i) {
        const units::Game offset = units::kHalfTile * (reverse_sprites_.size() - 1 - i);
        reverse_sprites_[i] -> draw (graphics, x + offset + padding_, y);
    }
}