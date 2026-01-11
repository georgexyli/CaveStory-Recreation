#ifndef KINEMATICS_H_
#define KINEMATICS_H_

#include "units.h"

class Kinematics {
public:
  units::Game position;
  units::Velocity velocity;

  units::Game delta(units::MS elapsed_time) {
    return velocity * elapsed_time.count();
  }
};

#endif // KINEMATICS_H_
