#include <iostream>

#include "../include/SDL2Wrapper.h"
#include <functional>
#include <map>
#include <vector>

class Game;

class Actor
{
	Game& game;
	std::string animState;
	bool removeFlag;
  public:
	std::string spriteBase;
	int x;
	int y;
	std::map<std::string, SDL2Wrapper::Animation> anims;
	Actor(Game& gameA, const std::string& spriteBaseA);
	void set(const int xA, const int yA);
	void setAnimState(const std::string& state);
	void remove();
	const bool shouldRemove() const;
	virtual void update();
	virtual void draw();
};

class Ship : public Actor
{
public:
	Ship(Game& gameA, const std::string& spriteBaseA, const int allegiance) : Actor(gameA, spriteBaseA)
	{

	}
};

class Projectile : public Actor
{
};

class Game
{
  public:
	SDL2Wrapper::Window& window;
	std::vector<int> background;
	std::vector<std::unique_ptr<Actor>> actors;
	int width;
	int height;

	Game(SDL2Wrapper::Window& windowA)
		: window(windowA)
	{
		width = 800;
		height = 600;

		SDL2Wrapper::Store::createFont("default", "monofonto.ttf");
		window.setCurrentFont("default", 18);

		SDL2Wrapper::loadAssetsFromFile<SDL2Wrapper::Sprite>("invadersSpriteAssets.txt");
		SDL2Wrapper::loadAssetsFromFile<SDL2Wrapper::Animation>("invadersAnimAssets.txt");

		initWorld();
	}

	void initWorld()
	{
		int bgWidth = width / 32;
		int bgHeight = height / 32;

		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				actors.push_back(std::make_unique<Ship>(*this, "badShip", 1));
				std::unique_ptr<Actor>& ship = actors.back();
				ship->set(50 + j * 64 + (i % 2) * 32, 50 + i * 48);
			}
		}
	}

	bool loop()
	{
		for (auto& act: actors)
		{
			act->update();
			act->draw();
		}
		return true;
	}
};

Actor::Actor(Game& gameA, const std::string& spriteBaseA)
	: game(gameA), animState("default"), removeFlag(false), spriteBase(spriteBaseA), x(0), y(0)
{
	anims["default"] = SDL2Wrapper::Animation();
	game.window.setAnimationFromDefinition(spriteBaseA, anims["default"]);
	setAnimState("default");
}

void Actor::set(const int xA, const int yA)
{
	x = xA;
	y = yA;
}

void Actor::setAnimState(const std::string& state)
{
	if (anims.find(state) != anims.end())
	{
		animState = state;
		SDL2Wrapper::Animation& anim = anims[animState];
		anim.start();
	}
}

void Actor::remove()
{
	removeFlag = true;
}

const bool Actor::shouldRemove() const
{
	return removeFlag;
}

void Actor::update()
{

}

void Actor::draw()
{
	SDL2Wrapper::Animation& anim = anims[animState];
	game.window.drawAnimation(anim, x, y);
}

int main(int argc, char* argv[])
{
	std::cout << "[SDL2Wrapper] Invaders Example" << std::endl;

	try
	{
		SDL2Wrapper::Window window("Invaders Example", 800, 600);
		Game game(window);

		SDL2Wrapper::Store::logFonts();
		SDL2Wrapper::Store::logSprites();
		SDL2Wrapper::Store::logAnimationDefinitions();
		window.startRenderLoop([&]() {
			return game.loop();
		});
	}
	catch (const std::string& e)
	{
		std::cout << e;
	}
	return 0;
}
