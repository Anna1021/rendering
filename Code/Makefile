CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra
OBJ = main.o image.o camera.o
BIN = raytracer_m1

all: $(BIN)
$(BIN): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ)
main.o: main.cpp image.h camera.h
	$(CXX) $(CXXFLAGS) -c main.cpp
image.o: image.cpp image.h
	$(CXX) $(CXXFLAGS) -c image.cpp
camera.o: camera.cpp camera.h
	$(CXX) $(CXXFLAGS) -c camera.cpp
clean:
	rm -f $(OBJ) $(BIN)
