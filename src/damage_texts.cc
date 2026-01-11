#include "damage_texts.h"
#include "damageable.h"

void DamageTexts::update(units::MS elapsed_time){
    for (auto iter{damage_text_map_.begin()} ; iter != damage_text_map_.end();){
        if (auto damageable = iter -> second.lock()){
            iter -> first -> setPosition(damageable -> center_x(), damageable -> center_y());
        }
        if (iter -> first -> update(elapsed_time) || !iter -> second.expired()){
            ++iter;
        } else{
            iter = damage_text_map_.erase(iter);
        }
    }
}

void DamageTexts::addDamageable(std::shared_ptr<Damageable> damageable){
    damage_text_map_[damageable -> get_damage_text()] = std::weak_ptr<Damageable>{damageable};
}

void DamageTexts::draw(Graphics& graphics){
    for (const auto& damage_text : damage_text_map_){
       damage_text.first -> draw(graphics);
    }
}