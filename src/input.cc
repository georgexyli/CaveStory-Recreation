#include "input.h"

void Input::beginNewFrame(){
    pressed_keys_.clear();
    released_keys_.clear();
}

void Input::keyDownEvent(const SDL_Event& event){
    pressed_keys_[event.key.keysym.scancode] = true;
    held_keys_[event.key.keysym.scancode] = true;
}

void Input::keyUpEvent(const SDL_Event& event){
    released_keys_[event.key.keysym.scancode] = true;
    held_keys_[event.key.keysym.scancode] = false;
}

bool Input::wasKeyPressed(SDL_Scancode key) const{
    auto it = pressed_keys_.find(key);
    return it != pressed_keys_.end() && (*it).second;
}

bool Input::wasKeyReleased(SDL_Scancode key) const{
    auto it = released_keys_.find(key);
    return it != released_keys_.end() && (*it).second;
}

bool Input::isKeyHeld(SDL_Scancode key) const{
    auto it = held_keys_.find(key);
    return it != held_keys_.end() && (*it).second;
}