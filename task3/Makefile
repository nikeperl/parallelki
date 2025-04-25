CXX = g++
CXXFLAGS = -O2 -std=c++17 -pthread

TARGET = matrix_vector_mult
SRC = src/main1.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
