#include "../include/cpu.hpp"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <time.h>

using namespace chip8;
/*
 * COLA
 * nnn or addr - A 12-bit value, the lowest 12 bits of the instruction
 * n or nibble - A 4-bit value, the lowest 4 bits of the instruction
 * x - A 4-bit value, the lower 4 bits of the high byte of the instruction
 * y - A 4-bit value, the upper 4 bits of the low byte of the instruction
 * kk or byte - An 8-bit value, the lowest 8 bits of the instruction
 * */

CPU::CPU(Memory *memoryPtr, Display *displayPtr, Keypad *keypadPtr) {
  memory = memoryPtr;
  display = displayPtr;
  keypad = keypadPtr;
  PC = 0x200;
  SP = 0;
  I = 0;
  delayTimer = 0;
  soundTimer = 0;
  memset(V, 0, sizeof(uint8_t) * REGISTER_SIZE);
  srand(time(NULL));
}

CPU::~CPU() {
  delete memory;
  delete display;
}

void CPU::tick() {
  executeInstruction();
  if (delayTimer > 0)
    delayTimer--;
  if (soundTimer > 0)
    soundTimer--;
}

void CPU::executeInstruction() {
  uint16_t opcode = memory->memory[PC] << 8 | memory->memory[PC + 1];
  printf("============= INICIO INSTRUÇÃO ATUAL: %04x ============= \n\n",
         opcode);
  uint16_t nnn = (opcode & 0x0FFF);
  uint8_t vx = (opcode & 0x0F00) >> 8;
  uint8_t vy = (opcode & 0x00F0) >> 4;
  uint8_t kk = opcode & 0x00FF;

  switch (opcode & 0xF000) {
  case 0x0000:
    instructionZero(opcode);
    break;
  case 0x1000:
    PC = nnn;
    printf("PC MOVIDO PARA O ENDEREÇO DE MEMORIA: %04x; PC = %04x\n",
           memory->memory[PC], PC);
    break;
  case 0x2000:
    memory->stack[SP++] = PC;
    PC = nnn;
    printf("PC MOVIDO PARA O ENDEREÇO DE MEMORIA: %04x; PC = %04x E ADICIONADO "
           "O VALOR DE PC NA STACK\n",
           memory->memory[PC], PC);
    break;
  case 0x3000:
    if (V[vx] == kk) {
      printf("INCREMENTADO O PC %04x", opcode);
      incrementePC();
    }
    incrementePC();
    break;
  case 0x4000:
    if (V[vx] != kk) {
      printf("INCREMENTADO O PC %04x\n", opcode);
      incrementePC();
    }
    incrementePC();
    break;
  case 0x5000:
    if (V[vx] == V[vy]) {
      printf("INCREMENTADO O PC %04x\n", opcode);
      incrementePC();
    }
    incrementePC();
    break;
  case 0x6000:
    V[vx] = kk;
    printf("SETADO O VALOR DE V[%02x] PARA %02x\n", vx, kk);
    incrementePC();
    break;
  case 0x7000:
    printf("V[%02x] = %02x\n", vx, V[vx]);
    V[vx] += kk;
    printf("ADICONADO O VALOR DE %02x EM V[%02x]\n", kk, vx);
    incrementePC();
    break;
  case 0x8000:
    instructionEight(opcode);
    incrementePC();
    break;
  case 0x9000:
    printf("Vx = %02x e Vy = %02x", V[vx], V[vy]);
    if (V[vx] != V[vy]) {
      incrementePC();
      printf("INCREMENTADO PC\n");
    }
    incrementePC();
    break;
  case 0xA000:
    I = nnn;
    printf("SETA O VALOR DO REGISTRADOR I PRA %04x\n", nnn);
    incrementePC();
    break;
  case 0xB000:
    PC = nnn + V[0x0];
    printf("SETA O VALOR DO PC PRA %04x SOMANDO O VALOR nnn = %04x EO "
           "REGISTRADOR V0 = %02x\n",
           PC, nnn, V[0x0]);
    break;
  case 0xC000: {
    uint8_t numberRandom = rand() % 256;
    V[vx] = numberRandom & kk;
    printf("SETA O VALOR DE V[%02x] PRA O AND BITWISE DE %02x E %02x\n", vx, kk,
           numberRandom);
    incrementePC();
    break;
  }
  case 0xD000: {
    uint8_t n = opcode & 0x000F;
    V[0xF] = 0;
    for (int i = 0; i < n; i++) {
      uint8_t byte = memory->memory[I + i];
      for (int j = 0; j < 8; j++) {
        uint8_t bit = (byte >> (7 - j)) & 0x1;
        uint8_t *pixelPtr =
            &display->screen[(vx + j) % SCREEN_HEIGHT][(vy + i) % SCREEN_WIDTH];

        if (bit == 1 && *pixelPtr == 1) {
          V[0xF] = 1;
        }
        printf("O PIXEL %02x FOI SETADO XOR %02x; %02x ^ %02x = %02x\n",
               *pixelPtr, bit, *pixelPtr, bit, *pixelPtr ^ bit);
        *pixelPtr ^= bit;
        printf("VALOR REAL SETADO = %02x\n", *pixelPtr);
      }
    }
    display->render();
    printf("DESENHA DO OS SPRITES DE %02x ATÉ %02x\n", I, n);
    incrementePC();
    break;
  }
  case 0xE000:
    instructionE(opcode);
    break;
  case 0xF000:
    instructionF(opcode);
    break;
  default:
    printf("INSTRUÇÃO: %04x; NÃO É UMA INSTRUÇÃO VALIDA\n", opcode);
    printf("VALOR DO PC = %02x, SP = %02x, I = %02x, x = %02x e y = %02x\n", PC,
           SP, I, vx, vy);
    exit(1);
  }
  printf("VALOR DO PC = %04x, SP = %02x, I = %04x, x = %02x, y = %02x e kk = "
         "%02x\n",
         PC, SP, I, vx, vy, kk);
  printf("\n============= FINAL INSTRUÇÃO ATUAL: %04x ============= \n",
         opcode);
}
void CPU::instructionZero(uint16_t opcode) {
  switch (opcode) {
  case 0x00E0:
    display->clearScreen();
    incrementePC();
    printf("LIMPOU A TELA\n");
    break;
  case 0x00EE:
    for (uint8_t value : memory->stack)
      printf("VALOR: %02x\n", value);
    PC = memory->stack[--SP];
    printf("PC MOVIDO PARA O ENDEREÇO DE MEMORIA: %02x, PC = %02x E SUBTRAIDO "
           "O VALOR SP; SP = %02x\n",
           memory->memory[PC], PC, SP);
    incrementePC();
    break;
  default:
    printf("INSTRUÇÃO: %04x; NÃO É UMA INSTRUÇÃO VALIDA\n", opcode);
    printf("VALOR DO PC = %02x, SP = %02x, I = %02x", PC, SP, I);
    exit(1);
  }
}
void CPU::instructionEight(uint16_t opcode) {
  uint8_t vx = (opcode & 0x0F00) >> 8;
  uint8_t vy = (opcode & 0x00F0) >> 4;
  switch (opcode & 0x000F) {
  case 0x0000:
    printf("COPIANDO V[%x] (%02x) PARA V[%x]\n", vy, V[vy], vx);
    V[vx] = V[vy];
    break;
  case 0x0001:
    printf("V[%x] (%02x) = V[%x] (%02x) | V[%x] (%02x)\n", vx, V[vx], vx, V[vx],
           vy, V[vy]);
    V[vx] |= V[vy];
    break;
  case 0x0002:
    printf("V[%x] (%02x) = V[%x] (%02x) & V[%x] (%02x)\n", vx, V[vx], vx, V[vx],
           vy, V[vy]);
    V[vx] &= V[vy];
    break;
  case 0x0003:
    printf("V[%x] (%02x) = V[%x] (%02x) ^ V[%x] (%02x)\n", vx, V[vx], vx, V[vx],
           vy, V[vy]);
    V[vx] ^= V[vy];
    break;
  case 0x0004: {
    uint16_t sumVxVy = V[vx] + V[vy];
    V[0xF] = (sumVxVy > 0xFF) ? 1 : 0;
    printf("SOMA: V[%x] (%02x) + V[%x] (%02x) = %04x, V[0xF] = %d\n", vx, V[vx],
           vy, V[vy], sumVxVy, V[0xF]);
    V[vx] = sumVxVy & 0x00FF;
    break;
  }
  case 0x0005:
    printf("V[0xF] = %d, V[%x] (%02x) - V[%x] (%02x)\n",
           (V[vx] > V[vy] ? 1 : 0), vx, V[vx], vy, V[vy]);
    V[0xF] = (V[vx] > V[vy]) ? 1 : 0;
    V[vx] -= V[vy];
    break;
  case 0x0006:
    V[0xF] = ((V[vx] & 0x01) == 1);
    printf("V[0xF] = %d, V[%x] (%02x) >> 1\n", V[0xF], vx, V[vx]);
    V[vx] = V[vx] >> 1;
    break;
  case 0x0007:
    V[0xF] = (V[vy] > V[vx]) ? 1 : 0;
    printf("V[0xF] = %d, V[%x] (%02x) - V[%x] (%02x)\n", V[0xF], vy, V[vy], vx,
           V[vx]);
    V[vy] -= V[vx];
    break;
  case 0x000E:
    V[0xF] = (V[vx] & 0x80) >> 7;
    printf("V[0xF] = %d, V[%x] (%02x) << 1\n", V[0xF], vx, V[vx]);
    V[vx] = V[vx] << 1;
    break;
  default:
    printf("INSTRUÇÃO: %04x; NÃO É UMA INSTRUÇÃO VALIDA\n", opcode);
    printf("VALOR DO PC = %02x, SP = %02x, I = %02x", PC, SP, I);
    exit(1);
  }
}

void CPU::instructionE(uint16_t opcode) {
  uint8_t vx = (opcode & 0x0F00) >> 8;
  uint8_t vy = (opcode & 0x00F0) >> 4;
  switch (opcode & 0x00FF) {
  case 0x009E:
    if (keypad->keys[vx] == 1) {
      printf("A KEY %02x FOI PRECIONADA\n", keypad->keys[vx]);
      incrementePC();
    }
    incrementePC();
    break;
  case 0x00A1:
    if (keypad->keys[vx] == 0) {
      printf("A KEY %02x FOI DEIXADO DE SER PRECIONADO\n", keypad->keys[vx]);
      incrementePC();
    }
    incrementePC();
    break;
  default:
    printf("VALOR DO PC = %02x, SP = %02x, I = %02x", PC, SP, I);
    printf("INSTRUÇÃO: %04x; NÃO É UMA INSTRUÇÃO VALIDA\n", opcode);
    exit(1);
  }
}

void CPU::instructionF(uint16_t opcode) {
  uint8_t vx = (opcode & 0x0F00) >> 8;
  uint8_t vy = (opcode & 0x00F0) >> 4;

  switch (opcode & 0x00FF) {
  case 0x0007:
    V[vx] = delayTimer;
    printf("ATRIBUINDO V[%x] = delayTimer (%02x)\n", vx, delayTimer);
    incrementePC();
    break;
  case 0x000A:
    while (true) {
      for (int i = 0; i < KEYPAD_SIZE; i++) {
        uint8_t pressed = keypad->keys[i];
        if (pressed == 1) {
          V[vx] = i;
          break;
        }
      }
    }
    incrementePC();
    break;
  case 0x0015:
    delayTimer = V[vx];
    printf("ATRIBUINDO delayTimer = V[%x] (%02x)\n", vx, V[vx]);
    incrementePC();
    break;
  case 0x0018:
    soundTimer = V[vx];
    printf("ATRIBUINDO soundTimer = V[%x] (%02x)\n", vx, V[vx]);
    incrementePC();
    break;
  case 0x001E:
    I += V[vx];
    printf("I += V[%x] (%02x), I = %04x\n", vx, V[vx], I);
    incrementePC();
    break;
  case 0x0029:
    I = memory->spriteSet[V[vx]];
    printf("I = memory->spriteSet[V[%x] (%02x)] = %04x\n", vx, V[vx], I);
    incrementePC();
    break;
  case 0x0033: {
    uint8_t hundreds = V[vx] / 100;
    uint8_t tens = (V[vx] / 10) % 10;
    uint8_t units = V[vx] % 10;
    memory->memory[I] = hundreds;
    memory->memory[I + 1] = tens;
    memory->memory[I + 2] = units;
    printf("ARMAZENANDO EM MEMÓRIA O VALOR %02x: I[%04x] = %02x, I[%04x] = "
           "%02x, I[%04x] = "
           "%02x\n",
           V[vx], I, hundreds, I + 1, tens, I + 2, units);
    incrementePC();
    break;
  }
  case 0x0055:
    for (int i = 0; i <= vx; i++) {
      memory->memory[I + i] = V[i];
      printf("ARMAZENANDO: memory[%04x] = V[%x] (%02x)\n", I + i, i, V[i]);
    }
    incrementePC();
    break;
  case 0x0065:
    for (int i = 0; i <= vx; i++) {
      V[i] = memory->memory[I + i];
      printf("CARREGANDO: V[%x] = memory[%04x] (%02x)\n", i, I + i,
             memory->memory[I + i]);
    }
    incrementePC();
    break;
  default:
    printf("INSTRUÇÃO: %04x; NÃO É UMA INSTRUÇÃO VALIDA\n", opcode);
    printf("VALOR DO PC = %02x, SP = %02x, I = %02x, x = %02x e y = %02x", PC,
           SP, I, vx, vy);
    exit(1);
  }
}

void CPU::incrementePC() { PC += 2; }
