EXECUTABLE = geebee
INCLUDE_PATHS = -I/opt/homebrew/include
LIBRARY_PATHS = -L/opt/homebrew/lib
LIBRARIES = -lSDL2 -lSDL2_image
SOURCES = main.cpp

all: $(EXECUTABLE)

$(EXECUTABLE):
	g++ $(SOURCES) -o $(EXECUTABLE) -Wall $(LIBRARY_PATHS) $(LIBRARIES) $(INCLUDE_PATHS)

clean:
	rm -rf $(EXECUTABLE)

