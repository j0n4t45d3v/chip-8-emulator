#include "../include/cpu.hpp"
#include <cstdint>

using namespace chip8;
/*
 * COLA
 * nnn or addr - A 12-bit value, the lowest 12 bits of the instruction
 * n or nibble - A 4-bit value, the lowest 4 bits of the instruction
 * x - A 4-bit value, the lower 4 bits of the high byte of the instruction
 * y - A 4-bit value, the upper 4 bits of the low byte of the instruction
 * kk or byte - An 8-bit value, the lowest 8 bits of the instruction
 * */

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
  case 0x8000:
    instructionEight(opcode);
    break;
  case 0x9000:
    if (V[vx] != V[vy])
      incrementePC();
    break;
  case 0xA000:
    I = nnn;
    break;
  case 0xB000:
    PC = nnn + V[0x0];
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
void CPU::instructionEight(uint16_t opcode) {
  uint8_t vx = (opcode | 0x0F00) >> 8;
  uint8_t vy = (opcode | 0x00F0) >> 4;
  switch (opcode | 0x000F) {
  case 0x0000:
    V[vx] = V[vy];
    break;
  case 0x0001:
    V[vx] |= V[vy];
    break;
  case 0x0002:
    V[vx] &= V[vy];
    break;
  case 0x0003:
    V[vx] ^= V[vy];
    break;
  case 0x0004: {
    uint16_t sumVxVy = V[vx] + V[vy];
    V[0xF] = (sumVxVy > 0xFF) ? 1 : 0;
    V[vx] = sumVxVy | 0x00FF;
    break;
  }
  case 0x0005:
    V[0xF] = (V[vx] > V[vy]) ? 1 : 0;
    V[vx] -= V[vy];
    break;
  case 0x0006:
    V[0xF] = ((V[vx] | 0x0F) == 1) ? 1 : 0;
    V[vx] = V[vx] >> 1;
    break;
  case 0x0007:
    V[0xF] = (V[vy] > V[vx]) ? 1 : 0;
    V[vy] -= V[vx];
    break;
  case 0x000E:
    V[0xF] = (((V[vx] | 0xF0) >> 1) == 1) ? 1 : 0;
    V[vx] = V[vx] << 1;
    break;
  }
}

void CPU::instructionF(uint16_t) {}

void CPU::incrementePC() { PC += 2; }
