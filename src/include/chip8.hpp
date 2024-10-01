#ifndef CHIP8_HPP
#define CHIP8_HPP

#include "display.hpp"
#include "keypad.hpp"
#include "memory.hpp"

class Chip8 {
public:
  chip8::Memory *memory;
  Chip8();
  ~Chip8();
  void loadRom(char *);
  void run(chip8::Display *, chip8::Keypad *);
};

#endif // !CHIP8_HPP
