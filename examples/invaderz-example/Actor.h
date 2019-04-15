#pragma once

#include "SDL2Wrapper.h"

class Game;
class Projectile;
class Ship;

class Actor
{
  protected:
	Game& game;
	std::string animState;
	bool removeFlag;

  public:
	std::string spriteBase;
	double x;
	double y;
	double vx;
	double vy;
	float r;
	std::map<std::string, SDL2Wrapper::Animation> anims;
	Actor(Game& gameA, const std::string& spriteBaseA);
	virtual ~Actor();
	void set(const double xA, const double yA);
	void setV(const double vxA, const double vyA);
	void setVx(const double vxA);
	void setVy(const double vyA);
	void setAnimState(const std::string& state);
	void remove();
	const bool shouldRemove() const;

	virtual void onCollision(Projectile& proj);
	virtual void onCollision(Ship& ship);
	virtual void update();
	virtual void draw();
};
