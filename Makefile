CXX = g++
CXXFLAGS = -Wall -std=c++17 -I./libs/ -I/opt/homebrew/include
LDFLAGS = -L/opt/homebrew/lib -lSDL3 -lSDL3_image -lSDL3_ttf -lSDL3_mixer -llua
SRCFLAGS = src/*.cpp src/ECS/*.cpp src/Game/*.cpp src/Logger/*.cpp src/AssetStore/*.cpp
OBJ_NAME = gameengine

build:
	$(CXX) $(CXXFLAGS) $(SRCFLAGS) $(LDFLAGS) -o $(OBJ_NAME)

run:
	./$(OBJ_NAME)

clean:
	rm -f $(OBJ_NAME)
