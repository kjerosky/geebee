EXECUTABLE = geebee
INCLUDE_PATHS = -I/opt/homebrew/include
LIBRARY_PATHS = -L/opt/homebrew/lib
LIBRARIES = -lSDL2 -lSDL2_image
FLAGS = -g -Wall -std=c++11
SOURCES = main.cpp bit_utils.cpp GameBoy.cpp Bus.cpp Cpu.cpp Cartridge.cpp Ppu.cpp Mapper_00.cpp

TEST_EXECUTABLE = run-cpu-test
TEST_SOURCES = run-cpu-test.cpp GameBoy.cpp Bus.cpp Cpu.cpp

all: $(EXECUTABLE)

$(EXECUTABLE):
	g++ $(SOURCES) -o $(EXECUTABLE) $(FLAGS) $(LIBRARY_PATHS) $(LIBRARIES) $(INCLUDE_PATHS)

$(TEST_EXECUTABLE):
	g++ $(TEST_SOURCES) -o $(TEST_EXECUTABLE) $(FLAGS) $(LIBRARY_PATHS) $(LIBRARIES) $(INCLUDE_PATHS)

clean:
	rm -rf $(EXECUTABLE) $(TEST_EXECUTABLE)
