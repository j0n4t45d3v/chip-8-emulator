#include "include/chip8.hpp"
#include "include/display.hpp"
#include "include/keypad.hpp"

int main(int argc, char *argv[]) {
  chip8::Display *display = new chip8::Display();
  chip8::Keypad *keypad = new chip8::Keypad();
  Chip8 *chip8 = new Chip8();
  chip8->run(display, keypad);
  return 0;
}
