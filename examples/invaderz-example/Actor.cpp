#include "Actor.h"
#include "Game.h"
#include "Ship.h"
#include "Projectile.h"

Actor::Actor(Game& gameA, const std::string& spriteBaseA)
	: game(gameA), animState("default"), removeFlag(false), spriteBase(spriteBaseA), x(0.0), y(0.0), vx(0.0), vy(0.0), r(16.0)
{
	anims["default"] = SDL2Wrapper::Animation();
	game.window.setAnimationFromDefinition(spriteBaseA, anims["default"]);
	setAnimState("default");
}

Actor::~Actor()
{
	if (spriteBase == "badShip")
		std::cout << "DELETE ACTOR: " << spriteBase << std::endl;
}

void Actor::set(const double xA, const double yA)
{
	x = xA;
	y = yA;
}

void Actor::setV(const double vxA, const double vyA)
{
	vx = vxA;
	vy = vyA;
}

void Actor::setVx(const double vxA)
{
	vx = vxA;
}

void Actor::setVy(const double vyA)
{
	vy = vyA;
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

void Actor::onCollision(Projectile& proj)
{
}
void Actor::onCollision(Ship& proj)
{
}
void Actor::update()
{
	double frameRatio = game.window.getFrameRatio();
	x += vx * frameRatio;
	y += vy * frameRatio;
}

void Actor::draw()
{
	SDL2Wrapper::Animation& anim = anims[animState];
	game.window.drawAnimation(anim, static_cast<int>(x), static_cast<int>(y));
}