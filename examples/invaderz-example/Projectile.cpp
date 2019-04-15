#include "Projectile.h"
#include "Game.h"
#include "Ship.h"
#include "GameOptions.h"

Projectile::Projectile(Game& gameA, const std::string& spriteBaseA, const int allegianceA, const double speedA, const int damageA)
	: Actor(gameA, spriteBaseA), speed(speedA), allegiance(allegianceA), damage(damageA)
{
	r = GameOptions.spriteSize / 4;
	if (allegiance)
	{
		setVy(speed);
	}
	else
	{
		setVy(-speed);
	}
}

void Projectile::onCollision(Ship& ship)
{
	if (!ship.isExploding() && ship.allegiance != allegiance)
	{
		remove();
	}
}

void Projectile::update()
{
	Actor::update();
	if (y > game.height + GameOptions.spriteSize || y < -GameOptions.spriteSize)
	{
		remove();
	}
}