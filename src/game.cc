#include "game.h"

#include <SDL.h>

#include "graphics.h"
#include "player.h"
#include "input.h"
#include "map.h"
#include "first_cave_bat.h"
#include "timer.h"

namespace {
const units::FPS kFps{60};
const units::MS kMaxFrameTime{5 * 1000 / 60};
};  

const units::Tile Game::kScreenWidth{20};
const units::Tile Game::kScreenHeight{15};

Game::Game() {
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_ShowCursor(SDL_DISABLE);
    eventLoop();
}

Game::~Game() { SDL_Quit(); }

void Game::eventLoop() {
    Graphics graphics;
    SDL_Event event;
    Input input;

    player_ = std::make_unique<Player>(units::tileToGame(kScreenWidth/2), units::tileToGame(kScreenHeight/2), graphics);
    bat_ = std::make_unique<FirstCaveBat>(units::tileToGame(5), units::tileToGame(kScreenHeight/2), graphics);
    map_.reset(Map::createTestMap(graphics));

    bool running{true};
    units::TimePoint last_update_time{std::chrono::steady_clock::now()};

    while (running) {
        const units::TimePoint start_time_ms{std::chrono::steady_clock::now()};
        
        input.beginNewFrame();
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_KEYDOWN:
                    input.keyDownEvent(event);
                    break;
                case SDL_KEYUP:
                    input.keyUpEvent(event);
                    break;
                default:
                    break;
            }
        }

        if (input.wasKeyPressed(SDL_SCANCODE_ESCAPE)) running = false;

        if (input.isKeyHeld(SDL_SCANCODE_LEFT) && input.isKeyHeld(SDL_SCANCODE_RIGHT)){
            player_ -> stopMoving();
        } else if (input.isKeyHeld(SDL_SCANCODE_LEFT)){
            player_ -> startMovingLeft();
        } else if (input.isKeyHeld(SDL_SCANCODE_RIGHT)){
            player_ -> startMovingRight();
        } else {
            player_ -> stopMoving();
        }

        if (input.isKeyHeld(SDL_SCANCODE_UP) && input.isKeyHeld(SDL_SCANCODE_DOWN)){
            player_ -> lookHorizontal();
        } else if (input.isKeyHeld(SDL_SCANCODE_UP)){
            player_ -> lookUp();
        } else if (input.isKeyHeld(SDL_SCANCODE_DOWN)){
            player_ -> lookDown();
        } else {
            player_ -> lookHorizontal();
        }

        if (input.wasKeyPressed(SDL_SCANCODE_SPACE)){
            player_ -> startJump();
        } else if (input.wasKeyReleased(SDL_SCANCODE_SPACE)){
            player_ -> stopJump();
        }

        const units::TimePoint cur_time_ms{std::chrono::steady_clock::now()};
        const units::MS elapsed_time{std::min(kMaxFrameTime, std::chrono::duration_cast<units::MS>(cur_time_ms - last_update_time))};
        update(elapsed_time);
        last_update_time = cur_time_ms;

        draw(graphics);
        const units::MS elapsed_time_ms{std::chrono::duration_cast<units::MS>(std::chrono::steady_clock::now() - start_time_ms)};
        const units::MS ms_per_frame {1000/kFps};
        if (elapsed_time_ms < ms_per_frame) {
            const units::MS frame_time = ms_per_frame - elapsed_time_ms;
            SDL_Delay(static_cast<Uint32>(frame_time.count()));
        }
    }
}

void Game::update(units::MS elapsed_time_ms) {
    Timer::updateAll(elapsed_time_ms);

    player_ -> update(elapsed_time_ms, *map_);
    bat_ -> update(elapsed_time_ms, player_ -> center_x());
    map_ -> update(elapsed_time_ms);
    if (bat_ -> damageRectangle().collidesWidth(player_ -> damageRectangle())){
        player_ -> takeDamage(bat_ -> contactDamage());
    }
}

void Game::draw(Graphics& graphics) const{
    graphics.clear();

    map_ -> drawBackground(graphics);
    bat_ -> draw(graphics);
    player_ -> draw(graphics);
    map_ -> draw(graphics);
    player_ -> drawHUD(graphics);

    graphics.flip();
}