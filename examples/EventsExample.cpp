#include <iostream>

#include "../include/SDL2Wrapper.h"
#include <functional>

struct
{
	SDL2Wrapper::Window* w;
	SDL2Wrapper::Window& getWindow()
	{
		return *w;
	}
} WindowInterface;

void handleMouseMove(int x, int y)
{
	std::cout << "Mousemove: " << x << "," << y << std::endl;
}

void handleKeyDown(const std::string& key) {
	std::cout << "Keydown: " << key << std::endl;
}

int main(int argc, char* argv[])
{
	std::cout << "[SDL2Wrapper] Events Example" << std::endl;

	try
	{
		SDL2Wrapper::Window window("Events Example", 800, 600);
		WindowInterface.w = &window;

		SDL2Wrapper::Store::createTexture("red-box", "red-box.png");

		Events& events = window.getEvents();
		events.setMouseEvent("mousemove", handleMouseMove);
		events.setKeyboardEvent("keydown", handleKeyDown);

		window.startRenderLoop([&]() {
			window.drawSprite("red-box", events.mouseX, events.mouseY, true);
			return true;
		});
	}
	catch (const std::string& e)
	{
		std::cout << e;
	}
	return 0;
}
