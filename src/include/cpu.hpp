#ifndef CPU_HPP
#define CPU_HPP

#include "display.hpp"
#include "memory.hpp"
#include <cstdint>

namespace chip8 {

const int REGISTER_SIZE = 16;

class CPU {
public:
  Memory *memory;
  Display *display;

  uint8_t V[REGISTER_SIZE];
  uint16_t I;
  uint16_t PC;
  uint8_t SP;

  uint8_t delayTimer;
  uint8_t soundTimer;

  CPU(Memory *, Display *);
  ~CPU();
  void tick();

private:
  void executeInstruction();
  void instructionZero(uint16_t);
  void instructionEight(uint16_t);
  void instructionF(uint16_t);
  void incrementePC();
};
} // namespace chip8

#endif // !CPU_HPP
