
CXX=g++
CXX_FLAGS=-lSDL2

BIN_DIR=./bin
LIBS=./src/lib/*.cpp
OUT_BIN=$(BIN_DIR)/main

all: run

create_out_folder:
	@if [ ! -d $(BIN_DIR) ]; then mkdir -p $(BIN_DIR); fi

build: create_out_folder
	$(CXX) ./src/main.cpp $(LIBS) -o $(OUT_BIN) $(CXX_FLAGS)

run: build
	$(OUT_BIN)
