#include <iostream>

#include "../include/SDL2Wrapper.h"
#include <functional>

int main(int argc, char* argv[])
{
	std::cout << "[SDL2Wrapper] Simple Example" << std::endl;

	SDL2Wrapper::Window window("Simple Example", 800, 600);
	SDL2Wrapper::Store::createTexture("background", "clouds.png");
	window.startRenderLoop([&]() {
		window.drawSprite("background", 0, 0, false);
		return true;
	});

	return 0;
}


