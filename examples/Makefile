CODE=SimpleExample.cpp EventsExample.cpp invaderz
SRC=$(CODE:.cpp=.exe)
FLAGS=-g -Wall -std=c++17 -I../include
ifeq ($(OS),Windows_NT)
    LIBS= -L ../lib -lSDL2Wrapper -mconsole -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
else
    LIBS= -L ../lib -lSDL2Wrapper -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
endif

all: $(SRC)

%.exe: %.cpp
	g++ $(FLAGS) $< -o $@ $(LIBS)

invaderz:
	(cd invaderz-example && make)

clean:
	rm -f *.exe
	rm -f *.o
	(cd invaderz-example && make clean)