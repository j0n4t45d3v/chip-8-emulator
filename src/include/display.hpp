#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <cstdint>

namespace chip8 {
const int SCREEN_HEIGHT = 31;
const int SCREEN_WIDTH = 63;
class Display {
public:
  SDL_Window *window;
  SDL_Renderer *renderer;
  bool quit;
  uint8_t screen[SCREEN_HEIGHT][SCREEN_WIDTH];
  Display();
  ~Display();
  void render();
  void wait(int);
  bool initSDL();
  void closeSDL();
  void clearScreen();
};
} // namespace chip8

#endif // !DISPLAY_HPP
