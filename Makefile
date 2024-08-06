EXECUTABLE = geebee
INCLUDE_PATHS = -I/opt/homebrew/include
LIBRARY_PATHS = -L/opt/homebrew/lib
LIBRARIES = -lSDL2 -lSDL2_image
FLAGS = -Wall -std=c++11
SOURCES = main.cpp GameBoy.cpp Bus.cpp Cpu.cpp

all: $(EXECUTABLE)

$(EXECUTABLE):
	g++ $(SOURCES) -o $(EXECUTABLE) $(FLAGS) $(LIBRARY_PATHS) $(LIBRARIES) $(INCLUDE_PATHS)

clean:
	rm -rf $(EXECUTABLE)
