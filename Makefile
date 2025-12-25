CXX = g++ 
CXXFLAGS = $(shell sdl2-config --cflags) -std=c++23
LDFLAGS = $(shell sdl2-config --libs)

SRC = $(wildcard src/*.cc)
BIN = gen/cavestory

compile: $(BIN)

$(BIN): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

run: $(BIN)
	./$(BIN)

clean:
	rm -f $(BIN)
