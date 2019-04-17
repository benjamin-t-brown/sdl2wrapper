#include <iostream>

#include "../include/SDL2Wrapper.h"
#include <functional>
#include <cctype>

class EventsExample
{
	SDL2Wrapper::Window& window;
	SDL2Wrapper::Events& events;

	int boxX;
	int boxY;
	bool shouldQuit;
  public:
	EventsExample(SDL2Wrapper::Window& windowA)
		: window(windowA), events(windowA.getEvents()), boxX(windowA.width / 2), boxY(windowA.height / 2), shouldQuit(false)
	{
		SDL2Wrapper::Store::createTexture("box", "red-box.png");
		SDL2Wrapper::Store::createSound("piano", "piano.wav");
		SDL2Wrapper::Store::createFont("default", "monofonto.ttf");
		window.setCurrentFont("default", 24);

		events.setKeyboardEvent("keydown", std::bind(&EventsExample::handleKeyDown, this, std::placeholders::_1));
		events.setMouseEvent("mousedown", std::bind(&EventsExample::handleMouseDown, this, std::placeholders::_1, std::placeholders::_2));
	}

	void handleMouseDown(int x, int y)
	{
		boxX = x;
		boxY = y;
	}

	void handleKeyDown(const std::string& key)
	{
		if (key == "Return")
		{
			window.playSound("piano");
		}
		if (key == "Escape")
		{
			shouldQuit = true;
		}
	}

	bool loop()
	{
		window.drawSprite("box", boxX, boxY, true);
		window.drawTextCentered("Click to move box", window.width / 2, window.height / 4, window.makeColor(255, 255, 255));
		window.drawTextCentered("Press 'Return' to play sound", window.width / 2, window.height - window.height / 4, window.makeColor(255, 255, 255));
		return !shouldQuit;
	}
};

void handleMouseMove(int x, int y)
{
	std::cout << "Mousemove: " << x << "," << y << std::endl;
}

void handleKeyDown(const std::string& key)
{
	std::cout << "Keydown: " << key << std::endl;
	if (key == "Return")
	{
	}
}

int main(int argc, char* argv[])
{
	std::cout << "[SDL2Wrapper] Events Example" << std::endl;

	try
	{
		SDL2Wrapper::Window window("Events Example", 800, 600);
		EventsExample example(window);
		window.startRenderLoop([&]() {
			return example.loop();
		});
	}
	catch (const std::string& e)
	{
		std::cout << e;
	}
	return 0;
}
