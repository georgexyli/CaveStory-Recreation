#ifndef DAMAGE_TEXT_H_
#define DAMAGE_TEXT_H_

#include "units.h"
#include "timer.h"

class Graphics;

class DamageText {
  public:
    DamageText();

    void setDamage(units::HP damage);
    void update(units::MS elapsed_time);
    void draw(Graphics &graphics, units::Game center_x, units::Game center_y) const;
  private:
    units::Game offset_y_;
    units::HP damage_;
    Timer timer_;
};

#endif
