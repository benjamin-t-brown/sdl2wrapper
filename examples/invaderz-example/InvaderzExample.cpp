#include <iostream>

#include "Game.h"
#include "GameOptions.h"

#include "SDL2Wrapper.h"
#include <ctime>
#include <functional>
#include <map>
#include <vector>

int main(int argc, char* argv[])
{
	std::cout << "[SDL2Wrapper] Invaders Example" << std::endl;
	srand(time(NULL));

	try
	{
		SDL2Wrapper::Window window("Invaders Example", GameOptions.width, GameOptions.height);
		Game game(window);

		// SDL2Wrapper::Store::logFonts();
		// SDL2Wrapper::Store::logSprites();
		// SDL2Wrapper::Store::logAnimationDefinitions();
		window.startRenderLoop([&]() { return game.loop(); });

		std::cout << "[SDL2Wrapper] Program End." << std::endl;
	}
	catch (const std::string& e)
	{
		std::cout << e;
	}
	SDL_Quit();
	std::cout << "end." << std::endl;
	return 0;
}
