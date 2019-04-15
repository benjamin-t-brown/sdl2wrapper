#pragma once

#include "../../include/SDL2Wrapper.h"

#include <string>
#include <vector>
#include <memory>

class Actor;
class Ship;
class Projectile;
class BoolTimer;

class Game
{
  public:
	SDL2Wrapper::Window& window;
	std::vector<int> background;

	std::unique_ptr<Ship> playerShip;
	std::vector<std::unique_ptr<Ship>> enemyShips;
	std::vector<std::unique_ptr<Ship>> shipsToAdd;
	std::vector<std::unique_ptr<Projectile>> projectiles;
	std::vector<std::unique_ptr<BoolTimer>> timers;

	int width;
	int height;
	int score;
	bool playerMayFire;
	int enemyFireRate;

	Game(SDL2Wrapper::Window& windowA);
	~Game();
	void initPlayer();
	void initWorld();
	void spawnEnemyShips(const int amount);
	void addProjectile(const std::string& type, const int x, const int y);
	void addBoolTimer(const int maxFrames, bool& ref);
	void modifyScore(const int value);
	void handleKeyDown(const std::string& key);
	void handleKeyUp(const std::string& key);
	void handleKeyUpdate();
	bool collidesWith(Actor& a, Actor& b);
	void checkCollisions(Ship& a);
	void drawUI();
	bool loop();
};
