#ifndef DAMAGEBLE_H_
#define DAMAGEBLE_H_

#include "units.h"
#include <memory>

class DamageText;

class Damageable{
    public:
        virtual units::Game center_x() const = 0 ;
        virtual units::Game center_y() const = 0 ;
        virtual std::shared_ptr<DamageText> get_damage_text() const = 0;

        virtual ~Damageable() = default;
};

#endif