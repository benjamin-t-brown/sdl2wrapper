CODE=SDL2Includes.cpp Sprite.cpp Events.cpp Animation.cpp Store.cpp AssetLoader.cpp Timer.cpp Window.cpp
SRC=$(CODE:.cpp=.o)
FLAGS=-g -Wall -std=c++17
LIB=libSDL2Wrapper.a

main: $(SRC)
	ar rvs $(LIB) $(SRC)
	mkdir -p ../lib ../include
	cp *.h ../include
	cp $(LIB) ../lib

.cpp.o:
	g++ $(FLAGS) -c $(@:.o=.cpp) -o $@

clean:
	rm -f $(LIB)
	rm -f *.o
	rm -r ../lib
	rm -r ../include