#include "../include/memory.hpp"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <iostream>

using namespace chip8;

Memory::Memory() {
  memset(memory, 0, sizeof(uint8_t) * MEMORY_SIZE);
  memset(stack, 0, sizeof(uint16_t) * STACK_SIZE);
  storeSpriteSet();
}

void Memory::storeRom(FILE *rom, int romSize) {
  if (romSize > MEMORY_SIZE) {
    std::cout << "Estouro de memoria! A rom informada é muito grande\n";
    exit(1);
  }

  fread(&memory[0x200], sizeof(uint8_t), romSize, rom);
}

void Memory::storeSpriteSet() {
  for (int i = 0; i < SPRITE_SET_SIZE; i++) {
    memory[i] = spriteSet[i];
  }
}
