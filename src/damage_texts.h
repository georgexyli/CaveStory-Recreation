#ifndef DAMAGE_TEXTS_H_
#define DAMAGE_TEXTS_H_

#include <map>
#include <memory>
#include "damage_text.h"
#include "units.h"

class Damageable;

class DamageTexts {
    public:
        void update(units::MS elapsed_time);
        void draw(Graphics& graphics);
        void addDamageable(std::shared_ptr<Damageable> damageable);
    private:
        using DamageTextMap = std::map<std::shared_ptr<DamageText>, std::weak_ptr<Damageable>>;
        DamageTextMap damage_text_map_;
};

#endif