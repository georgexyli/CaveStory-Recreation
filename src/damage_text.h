#ifndef DAMAGE_TEXT_H_
#define DAMAGE_TEXT_H_

#include "units.h"
#include "timer.h"

class Graphics;

class DamageText {
  public:
    DamageText();

    void setDamage(units::HP damage);
    bool update(units::MS elapsed_time);
    void setPosition(units::Game x, units::Game y);
    void draw(Graphics &graphics) const;
  private:
    bool should_rise_;
    units::Game offset_y_;
    units::HP damage_;
    Timer timer_;
    units::Game center_x_, center_y_;
};

#endif
