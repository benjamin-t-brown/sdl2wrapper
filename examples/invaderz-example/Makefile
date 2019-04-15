CODE=Ship.cpp Projectile.cpp Timer.cpp Actor.cpp Game.cpp InvaderzExample.cpp
SRC=$(CODE:.cpp=.o)
FLAGS=-g -Wall -std=c++17

ifeq ($(OS),Windows_NT)
	EXE = InvaderzExample.exe
    LIBS= -L ../../lib -lSDL2Wrapper -mconsole -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
else
	EXE = InvaderzExample
    LIBS=-lSDL2main -lSDL2 -lSDL2_image -L ../../lib -lSDL2Wrapper
endif

$(EXE): $(SRC)
	g++ $(FLAGS) $(SRC) -o InvaderzExample $(LIBS)

.cpp.o:
	g++ $(FLAGS) -c $(@:.o=.cpp) -o $@

run: $(EXE)
	./$(EXE)

clean:
	rm -f $(EXE)
	rm -f *.o