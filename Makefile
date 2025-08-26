OUTPUT_DIR = ./output
TARGET = $(OUTPUT_DIR)/game

build:
	mkdir -p $(OUTPUT_DIR)
	g++ -w -std=c++14 -Wfatal-errors \
	./src/*.cpp \
	-o $(TARGET) \
	-I"./lib/lua" \
	-L"./lib/lua" \
	-llua5.4 \
	-lSDL2 \
	-lSDL2_image \
	-lSDL2_ttf \
	-lSDL2_mixer

clean:
	rm -rf $(OUTPUT_DIR)

launch:
	$(TARGET)
