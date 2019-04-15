#include "Ship.h"
#include "Game.h"
#include "Projectile.h"
#include "GameOptions.h"

Ship::Ship(Game& gameA, const std::string& spriteBaseA, const int allegianceA, const int hpA)
	: Actor(gameA, spriteBaseA), speed(2), shouldSwitchToDefaultState(false), allegiance(allegianceA), hp(hpA)
{
	anims["damaged"] = SDL2Wrapper::Animation();
	game.window.setAnimationFromDefinition(spriteBaseA + "_Damaged", anims["damaged"]);
	anims["explosion"] = SDL2Wrapper::Animation();
	game.window.setAnimationFromDefinition("explosion", anims["explosion"]);
}

Ship::~Ship()
{
}

void Ship::onCollision(Projectile& proj)
{
	if (!isExploding() && proj.allegiance != allegiance)
	{
		hp -= 1;
		if (hp <= 0)
		{
			setAnimState("explosion");
			game.addBoolTimer(28, removeFlag);
			setV(0.0, 0.0);
			game.modifyScore(GameOptions.pointsPerDestroyedShip);
			game.spawnEnemyShips(1);
		}
		else
		{
			setAnimState("damaged");
			shouldSwitchToDefaultState = false;
			game.addBoolTimer(40, shouldSwitchToDefaultState);
		}
	}
}

bool Ship::isExploding() const
{
	return animState == "explosion";
}

void Ship::update()
{
	Actor::update();

	if (shouldSwitchToDefaultState)
	{
		shouldSwitchToDefaultState = false;
		setAnimState("default");
	}

	if (x > game.width)
	{
		x = game.width;
	}
	else if (x < 0)
	{
		x = 0;
	}
}
