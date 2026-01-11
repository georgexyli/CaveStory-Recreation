#ifndef GAME_H_
#define GAME_H_

#include "damage_texts.h"
#include "particle_system.h"
#include "units.h"
#include <memory>

class Player;
class Graphics;
class Map;
class FirstCaveBat;

class Game {
private:
  void eventLoop();
  void update(units::MS elapsed_time_ms, Graphics &graphics);
  void draw(Graphics &graphics);

  std::shared_ptr<Player> player_;
  std::shared_ptr<FirstCaveBat> bat_;
  std::unique_ptr<Map> map_;
  ParticleSystem front_particle_system_;
  ParticleSystem entity_particle_system_;

public:
  Game();
  ~Game();

  static const units::Tile kScreenWidth;
  static const units::Tile kScreenHeight;
  DamageTexts damage_texts_;
};

#endif // GAME_H_
