CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pthread -O2

TARGET = main
SRC = main.cpp
OBJ = main.o

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o $(TARGET)
