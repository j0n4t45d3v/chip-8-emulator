#include "../include/display.hpp"
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

using namespace chip8;

Display::Display() {
  memset(screen, 0, sizeof(screen));
  quit = false;
}

Display::~Display() {}

void Display::render() {
  for (int x = 0; x < SCREEN_HEIGHT; x++) {
    for (int y = 0; y < SCREEN_WIDTH; y++) {
      // printf("PIXEL COM VALOR: %02x\n", screen[x][y]);
      if (screen[x][y] == 1) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // white
      } else {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // black
      }

      SDL_Rect *pixel = new SDL_Rect();
      pixel->x = x * 10;
      pixel->y = y * 10;
      pixel->w = 10;
      pixel->h = 10;

      SDL_RenderFillRect(renderer, pixel);
    }
  }
  SDL_RenderPresent(renderer);
}

void Display::wait(int microsecond) { SDL_Delay(microsecond); }

bool Display::initSDL() {
  if ((SDL_Init(SDL_INIT_VIDEO) == -1)) {
    printf("Could not initialize SDL: %s.\n", SDL_GetError());
    return false;
  }

  window = SDL_CreateWindow("Emulador de Chip-8", SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH * 10,
                            SCREEN_HEIGHT * 10, SDL_WINDOW_SHOWN);

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  return true;
}

void Display::closeSDL() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void Display::clearScreen() {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Cor preta (RGB)
  SDL_RenderClear(renderer); // Limpa a tela com a cor definida
}
