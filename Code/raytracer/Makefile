CXX = g++
CXXFLAGS = -O2 -std=c++17 -Wall -Wextra

BIN = raytracer
SRC = main.cpp
OBJ = $(SRC:.cpp=.o)

$(BIN): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(OBJ) $(BIN)
