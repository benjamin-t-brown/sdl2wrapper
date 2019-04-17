CODE=Ship.cpp Projectile.cpp Actor.cpp Game.cpp
SRC=$(CODE:.cpp=.o)
FLAGS=-g -Wall -std=c++17

ifeq ($(OS),Windows_NT)
	EXE = InvaderzExample.exe
    LIBS= -L ../../lib -lSDL2Wrapper -mconsole -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
else
	EXE = InvaderzExample
    LIBS= -L ../../lib -lSDL2Wrapper -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
endif

$(EXE): $(SRC)
	g++ $(FLAGS) $(SRC) InvaderzExample.cpp -o $(EXE) $(LIBS)

.cpp.o:
	g++ $(FLAGS) -c $(@:.o=.cpp) -o $@

run: $(EXE)
	./$(EXE)

clean:
	rm -f $(EXE)
	rm -f *.o