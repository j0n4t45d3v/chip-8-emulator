#include "../include/memory.hpp"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <ostream>

using namespace chip8;

Memory::Memory() {
  memset(memory, 0, sizeof(uint8_t) * MEMORY_SIZE);
  memset(stack, 0, sizeof(uint16_t) * STACK_SIZE);
  storeSpriteSet();
}

Memory::~Memory() {}

void Memory::storeRom(FILE *rom, int romSize) {
  if (romSize > MEMORY_SIZE) {
    std::cout << "Estouro de memoria! A rom informada é muito grande\n";
    exit(1);
  }

  size_t bytesRead = fread(&memory[0x200], sizeof(uint8_t), romSize, rom);

  std::cout
      << " ==================================================== MEMORIA "
         "ALOCADA ========================================================="
      << std::endl
      << std::endl;
  int counter = 0;
  for (uint8_t hex : memory) {
    printf(" %02x ", hex);
    counter++;
    if (counter >= 32) {
      counter = 0;
      std::cout << std::endl;
    }
  }
  std::cout
      << std::endl
      << " ==================================================== MEMORIA "
         "ALOCADA ========================================================="
      << std::endl;
  if (bytesRead != romSize) {
    std::cout << "Erro ao carregar a ROM. Bytes lidos: " << bytesRead << " de "
              << romSize << std::endl;
    exit(1);
  }
}

void Memory::storeSpriteSet() {
  for (int i = 0; i < SPRITE_SET_SIZE; i++) {
    memory[i] = spriteSet[i];
  }
}
