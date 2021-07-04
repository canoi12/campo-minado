CC = g++
AR = ar
CFLAGS = -Wall -std=c++17
OUT = game
SRC = $(wildcard src/*.cpp)
INCLUDE = -Iinclude
LFLAGS = -L. -ltea -lgame -lSDL2 -lm -lpthread

OBJS = $(SRC:%.cpp=%.o)

.PHONY: all
.SECOND: $(OUT)

all: libgame.a
	$(CC) main.cpp -o $(OUT) $(INCLUDE) $(CFLAGS) $(LFLAGS)

libgame.a: $(OBJS)
	$(AR) rcs $@ $(OBJS)

%.o: %.cpp
	$(CC) -c $< -o $@ $(INCLUDE) $(CFLAGS)

clean:
	rm -f $(OBJS)
	rm -f libgame.a
	rm -f $(OUT)