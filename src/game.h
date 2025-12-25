#ifndef GAME_H_
#define GAME_H_

#include <memory>
#include "units.h"

class Player;
class Graphics;
class Map;
class FirstCaveBat;

class Game {
    private:
        void eventLoop();
        void update(units::MS elapsed_time_ms);
        void draw(Graphics& graphics) const;

        std::unique_ptr<Player> player_;
        std::unique_ptr<FirstCaveBat> bat_;
        std::unique_ptr<Map> map_;

    public:
        Game();
        ~Game();

        static const units::Tile kScreenWidth;
        static const units::Tile kScreenHeight;
};

#endif  // GAME_H_
