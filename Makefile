EXECUTABLE = geebee
INCLUDE_PATHS = -I/opt/homebrew/Cellar/libpng/1.6.40/include -I/opt/homebrew/Cellar/openal-soft/1.22.2/include
LIBRARY_PATHS = -L/opt/homebrew/Cellar/libpng/1.6.40/lib -L/opt/homebrew/Cellar/openal-soft/1.22.2/lib
LIBRARIES = -framework OpenGL -framework GLUT -framework Carbon -framework OpenAL -lpng
SOURCES = main.cpp

all:
	g++ $(SOURCES) -o $(EXECUTABLE) -arch arm64 -std=c++17 -mmacosx-version-min=10.15 -Wall $(LIBRARY_PATHS) $(LIBRARIES) $(INCLUDE_PATHS)

clean:
	rm -rf $(EXECUTABLE)

