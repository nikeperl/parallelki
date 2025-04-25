CC = g++
CFLAGS = -O2 -Wall
SRC = src/main.cpp
TARGET = sin_array_sum

all:
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) -DTYPE=double

float:
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) -DTYPE=float

clean:
	rm -f $(TARGET)