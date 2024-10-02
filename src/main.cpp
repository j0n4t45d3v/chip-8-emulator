#include "include/chip8.hpp"
#include "include/display.hpp"
#include "include/keypad.hpp"
#include <iostream>

int main(int argc, char *argv[]) {
  chip8::Display *display = new chip8::Display();
  chip8::Keypad *keypad = new chip8::Keypad();
  Chip8 *chip8 = new Chip8();
  char romPath[] = "/home/jonatas/workspace/cpp/chip-8-emulator/roms/Pong.ch8";
  chip8->loadRom(romPath);
  chip8->run(display, keypad);
  return 0;
}
