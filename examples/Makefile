CODE=SimpleExample.cpp EventsExample.cpp InvaderzExample.cpp
SRC=$(CODE:.cpp=.exe)
FLAGS=-g -Wall -std=c++17 -I../include
ifeq ($(OS),Windows_NT)
    LIBS= -L ../lib -lSDL2Wrapper -mconsole -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
else
    LIBS=-lSDL2main -lSDL2 -lSDL2_image -L ../lib -lSDL2Wrapper
endif

all: $(SRC)

InvaderzExample.cpp:
	(cd invaderz-example && make)

%.exe: %.cpp
	g++ $(FLAGS) $< -o $@ $(LIBS)

clean:
	rm -f *.exe
	rm -f *.o