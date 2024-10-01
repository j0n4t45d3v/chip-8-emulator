#include "../include/chip8.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <ostream>

Chip8::Chip8() { memory = new chip8::Memory(); }

void Chip8::loadRom(char *filename) {
  FILE *rom = std::fopen(filename, "rb");
  if (rom == nullptr) {
    std::cout << "Falha ao abrir a rom verifique se ela existe\n";
    exit(1);
  }

  fseek(rom, 0, SEEK_END);
  int romSize = ftell(rom);
  rewind(rom);

  memory->storeRom(rom, romSize);
  fclose(rom);
}

void Chip8::run(chip8::Display *display, chip8::Keypad *keypad) {
  if (!display->initSDL())
    exit(1);
  bool running = true;
  SDL_Event event;

  while (running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE) {
        running = false;
      }
      keypad->keypressed(event);
    }
    display->render();
    std::cout << "Esperando..." << std::endl;
    // display->wait(5000);
  }

  display->closeSDL();
}
