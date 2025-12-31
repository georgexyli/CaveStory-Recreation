#ifndef NUMBER_SPRITE_H_
#define NUMBER_SPRITE_H_

#include "units.h"
#include <memory>
#include <vector>

class Sprite;
class Graphics;

class NumberSprite {
public:
  static NumberSprite HUDNumber(Graphics &graphics, int number,
                                int num_digits) {
    return NumberSprite(graphics, number, num_digits, ColorType::WHITE,
                        OperatorType::NONE);
  }

  static NumberSprite DamageNumber(Graphics &graphics, int number) {
    return NumberSprite(graphics, number, 0, ColorType::RED,
                        OperatorType::MINUS);
  }

  static NumberSprite ExperienceNumber(Graphics &graphics, int number) {
    return NumberSprite(graphics, number, 0, ColorType::WHITE,
                        OperatorType::PLUS);
  }

  void draw(Graphics &graphics, units::Game x, units::Game y) const;
  void drawCentered(Graphics& graphics, units::Game x, units::Game y) const
      { draw(graphics, x - width() / 2, y - height() / 2); }

  units::Game width() const {
    return units::kHalfTile * reversed_glyphs_.size();
  }
  units::Game height() const { return units::kHalfTile; }

private:
  enum class ColorType { RED, WHITE };

  enum class OperatorType { PLUS, MINUS, NONE };

  NumberSprite(Graphics &graphics, int number, int num_digits, ColorType color,
               OperatorType op);

  units::Game padding_;
  std::vector<std::shared_ptr<Sprite>> reversed_glyphs_;
  ColorType color_;
  OperatorType op_;
};

#endif
