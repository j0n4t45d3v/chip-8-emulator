#ifndef KEYPAD_HPP
#define KEYPAD_HPP

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <cstdint>
#include <cstring>
#include <unordered_map>

namespace chip8 {
const int KEYPAD_SIZE = 16;
const uint8_t INVALID_KEY = 0xFF;

class Keypad {
public:
  uint8_t keys[KEYPAD_SIZE];
  Keypad();
  ~Keypad();
  void keypressed(SDL_Event &);

private:
  std::unordered_map<SDL_Keycode, uint8_t> keymap = {
      {SDLK_1, 0x0}, {SDLK_2, 0x1}, {SDLK_3, 0x2}, {SDLK_4, 0x3},
      {SDLK_q, 0x4}, {SDLK_w, 0x5}, {SDLK_e, 0x6}, {SDLK_r, 0x7},
      {SDLK_a, 0x8}, {SDLK_s, 0x9}, {SDLK_d, 0xA}, {SDLK_f, 0xB},
      {SDLK_z, 0xC}, {SDLK_x, 0xD}, {SDLK_c, 0xE}, {SDLK_v, 0xF}};
  uint8_t getKey(SDL_Event &);
};

} // namespace chip8

#endif // !KEYPAD_HPP
