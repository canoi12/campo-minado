CC = gcc
CPP = g++
AR = ar
CFLAGS = -Wall -std=c++17
OUT = game
SRC = $(wildcard src/*.cpp)
INCLUDE = -Iinclude -Iexternal/tea -Iexternal/tea/external
LFLAGS = -L. -ltea -lgame -lSDL2 -lm -lpthread

OBJS = $(SRC:%.cpp=%.o)

.PHONY: all
.SECOND: $(OUT)

all: libgame.a libtea.a
	$(CPP) main.cpp -o $(OUT) $(INCLUDE) $(CFLAGS) $(LFLAGS)

libgame.a: $(OBJS)
	$(AR) rcs $@ $(OBJS)

libtea.a:
	$(CC) -c external/tea/tea.c -o tea.o $(INCLUDE) -Wall -std=c99 `sdl2-config --cflags`
	$(AR) rcs $@ tea.o

%.o: %.cpp
	$(CPP) -c $< -o $@ $(INCLUDE) $(CFLAGS)

clean:
	rm -f $(OBJS)
	rm -f libgame.a
	rm -f tea.o libtea.a
	rm -f $(OUT)