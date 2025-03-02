CC = g++
CFLAGS = -Iinclude/ -std=c++17 
LDFLAGS = -Llib/ -lSDL3 -lwinmm
TARGET = PhyseEng
SRC = main.cpp


OBJ = $(SRC:.cpp=.o)

$(TARGET): $(SRC)
	$(CC) -o $(TARGET) $(SRC) $(LDFLAGS) $(CFLAGS)

%.o: %.cpp
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -f $(TARGET)
