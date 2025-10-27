CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra

OBJS = main.o camera.o image.o scene.o
TARGET = raytracer_m1

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

main.o: main.cpp camera.h image.h scene.h
	$(CXX) $(CXXFLAGS) -c main.cpp

camera.o: camera.cpp camera.h
	$(CXX) $(CXXFLAGS) -c camera.cpp

image.o: image.cpp image.h
	$(CXX) $(CXXFLAGS) -c image.cpp

scene.o: scene.cpp scene.h
	$(CXX) $(CXXFLAGS) -c scene.cpp

clean:
	rm -f $(OBJS) $(TARGET)
