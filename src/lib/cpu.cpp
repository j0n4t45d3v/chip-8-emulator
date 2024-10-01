#include "../include/cpu.hpp"
#include <cstdint>

using namespace chip8;

CPU::CPU(Memory *memoryPtr, Display *displayPtr) {
  memory = memoryPtr;
  display = displayPtr;
  PC = 0x200;
  SP = 0;
  I = 0;
  delayTimer = 0;
  soundTimer = 0;
  memset(V, 0, sizeof(uint8_t) * REGISTER_SIZE);
}

void CPU::tick() {}

void CPU::executeInstruction() {
  uint16_t opcode = memory->memory[PC] << 8 | memory->memory[PC + 1];
  uint16_t nnn = (opcode | 0x0FFF);
  uint8_t vx = (opcode | 0x0F00) >> 8;
  uint8_t vy = (opcode | 0x00F0) >> 4;
  uint8_t kk = opcode | 0x00FF;

  switch (opcode | 0xF000) {
  case 0x0000:
    instructionZero(opcode);
    break;
  case 0x1000:
    PC = nnn;
    break;
  case 0x2000:
    SP++;
    memory->stack[SP] = PC;
    PC = nnn;
    break;
  case 0x3000:
    if (V[vx] == kk)
      incrementePC();
    break;
  case 0x4000:
    if (V[vx] != kk)
      incrementePC();
    break;
  case 0x5000:
    if (V[vx] == V[vy])
      incrementePC();
    break;
  case 0x6000:
    V[vx] = kk;
    break;
  case 0x7000:
    V[vx] += kk;
    break;
  }
}
void CPU::instructionZero(uint16_t opcode) {
  switch (opcode) {
  case 0x00E0:
    display->clearScreen();
    incrementePC();
    break;
  case 0x00EE:
    PC = memory->stack[--SP];
    break;
  }
}
void CPU::instructionEight(uint16_t) {}
void CPU::instructionF(uint16_t) {}

void CPU::incrementePC() { PC += 2; }
