#include "../include/keypad.hpp"
#include <cstdint>

using namespace chip8;

Keypad::Keypad() { memset(keys, 0, sizeof(uint8_t) * KEYPAD_SIZE); }

void Keypad::keypressed(SDL_Event &event) {
  uint8_t key = getKey(event);
  if (key != INVALID_KEY) {
    switch (event.type) {
    case SDL_KEYDOWN:
      keys[key] = 1;
      break;
    case SDL_KEYUP:
      keys[key] = 0;
      break;
    }
  }
}

uint8_t Keypad::getKey(SDL_Event &event) {
  SDL_Keycode keyPressed = event.key.keysym.sym;
  auto it = keymap.find(keyPressed);
  if (it != keymap.end())
    return keymap[keyPressed];
  return INVALID_KEY;
}
