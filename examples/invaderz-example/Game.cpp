#include "Game.h"
#include "GameOptions.h"
#include "Actor.h"
#include "Ship.h"
#include "Projectile.h"
#include "Timer.h"

#include <algorithm>
#include <cctype>
#include <string>

const float distance(const int x1, const int y1, const int x2, const int y2)
{
	return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

Game::Game(SDL2Wrapper::Window& windowA)
	: window(windowA), width(windowA.width), height(windowA.height), score(0), playerMayFire(true), enemyFireRate(GameOptions.enemyFireRate)
{
	SDL2Wrapper::Store::createFont("default", "assets/monofonto.ttf");
	window.setCurrentFont("default", 18);

	SDL2Wrapper::loadAssetsFromFile<SDL2Wrapper::Sprite>("assets/invadersSpriteAssets.txt");
	SDL2Wrapper::loadAssetsFromFile<SDL2Wrapper::Animation>("assets/invadersAnimAssets.txt");

	SDL2Wrapper::Events& events = window.getEvents();
	events.setKeyboardEvent("keydown", std::bind(&Game::handleKeyDown, this, std::placeholders::_1));

	playerShip = std::make_unique<Ship>(*this, "goodShip", 0, 20);
	initPlayer();
	initWorld();
}

Game::~Game()
{
	std::cout << "Clean game." << std::endl;
}

void Game::initPlayer()
{
	playerShip->set(width / 2, height - GameOptions.spriteSize * 2);
	playerShip->hp = GameOptions.playerShipHP;
}

void Game::initWorld()
{
	score = 0;

	if (!background.size())
	{
		for (int i = 0; i < height; i += GameOptions.spriteSize)
		{
			for (int j = 0; j < width; j += GameOptions.spriteSize)
			{
				background.push_back(rand() % 3);
			}
		}
	}

	enemyShips.clear();
	int x = 50 + GameOptions.spriteSize * 2 * GameOptions.spriteSize;
	int y = 50 + (GameOptions.spriteSize + GameOptions.spriteSize / 2);
	enemyShips.push_back(std::make_unique<Ship>(*this, "badShip", 1, GameOptions.enemyShipHP));
	std::unique_ptr<Ship>& ship = enemyShips.back();
	ship->set(static_cast<double>(x), static_cast<double>(y));
	ship->setV(2, 1);
}

void Game::spawnEnemyShips(const int amount)
{
	for (int i = 0; i < amount; i++)
	{
		int x;
		int y;
		double vx;
		double vy;
		double r = static_cast<double>(1 + rand() % (GameOptions.enemyShipMaxSpeed * 2)) / 2.0;
		if (i % 2)
		{
			x = width;
			vx = -r;
		}
		else
		{
			x = 0;
			vx = r;
		}
		y = rand() % (height / 2);
		vy = GameOptions.enemyShipMaxSpeed - rand() % (2 * GameOptions.enemyShipMaxSpeed);
		enemyShips.push_back(std::make_unique<Ship>(*this, "badShip", 1, GameOptions.enemyShipHP));
		std::unique_ptr<Ship>& ship = enemyShips.back();
		ship->set(static_cast<double>(x), static_cast<double>(y));
		ship->setV(static_cast<double>(vx), static_cast<double>(vy));
	}
}

void Game::addProjectile(const std::string& type, const int x, const int y)
{
	if (type == "good")
	{
		projectiles.push_back(std::make_unique<Projectile>(*this, "goodLazer", 0, GameOptions.playerProjSpeed, GameOptions.playerProjDamage));
		projectiles.back()->set(x, y);
	}
	else if (type == "bad")
	{
		projectiles.push_back(std::make_unique<Projectile>(*this, "badLazer", 1, GameOptions.enemyProjSpeed, GameOptions.enemyProjDamage));
		projectiles.back()->set(x, y);
	}
	else if (type == "bad2")
	{
		projectiles.push_back(std::make_unique<Projectile>(*this, "badLazer2", 1, GameOptions.enemyProjSpeed2, GameOptions.enemyProjDamage2));
		projectiles.back()->set(x, y);
	}
}

void Game::addBoolTimer(const int maxFrames, bool& ref)
{
	timers.push_back(std::make_unique<BoolTimer>(maxFrames, ref));
}

void Game::modifyScore(const int value)
{
	score += value;
	if (score < 0)
	{
		score = 0;
	}
}

void Game::handleKeyDown(const std::string& key)
{
	//std::cout << "KEYDOWN: " << key << std::endl;
}

void Game::handleKeyUp(const std::string& key)
{
	//std::cout << "KEYDOWN: " << key << std::endl;
}

void Game::handleKeyUpdate()
{
	SDL2Wrapper::Events& events = window.getEvents();
	if (events.isKeyPressed("Left"))
	{
		playerShip->setVx(-GameOptions.playerSpeed);
	}
	else if (events.isKeyPressed("Right"))
	{
		playerShip->setVx(GameOptions.playerSpeed);
	}
	else
	{
		playerShip->setVx(0.0);
	}

	if (events.isKeyPressed("Space"))
	{
		if (playerMayFire)
		{
			playerMayFire = false;
			addProjectile("good", playerShip->x, playerShip->y - GameOptions.spriteSize / 2);
			addBoolTimer(GameOptions.playerFireCooldown, playerMayFire);
			modifyScore(-GameOptions.pointsLostPerShot);
		}
	}
}

bool Game::collidesWith(Actor& a, Actor& b)
{
	float d = distance(a.x, a.y, b.x, b.y);
	if (d < a.r + b.r)
	{
		return true;
	}
	return false;
}

void Game::checkCollisions(Ship& a)
{
	for (auto it = projectiles.begin(); it != projectiles.end(); ++it)
	{
		std::unique_ptr<Projectile>& act = *it;
		if (collidesWith(a, *act))
		{
			a.onCollision(*act);
			act->onCollision(a);
		}
	}
}

void Game::drawUI()
{
	int hpX = GameOptions.spriteSize;
	int hpY = height - GameOptions.spriteSize;
	window.setCurrentFont("default", 20);
	window.drawText("HP ", hpX - 24, hpY, window.makeColor(255, 255, 255));
	for (int i = 0; i < playerShip->hp; i++)
	{
		window.drawSprite("healthBar_0", hpX + i * 12, hpY, false);
	}

	window.drawText("Score: " + std::to_string(score), hpX - 24, 32, window.makeColor(255, 255, 255));
}

bool Game::loop()
{
	if (background.size())
	{
		for (int i = 0; i <= height / GameOptions.spriteSize; ++i)
		{
			for (int j = 0; j < width / GameOptions.spriteSize; ++j)
			{
				int ind = i * width / GameOptions.spriteSize + j;
				window.drawSprite("starsBg_" + std::to_string(background[ind]), j * GameOptions.spriteSize, i * GameOptions.spriteSize, false);
			}
		}
	}

	handleKeyUpdate();

	for (auto it = timers.begin(); it != timers.end(); ++it)
	{
		std::unique_ptr<BoolTimer>& timer = *it;
		timer->update();
		if (timer->shouldRemove())
		{
			timers.erase(it--);
		}
	}

	playerShip->update();
	checkCollisions(*playerShip);
	playerShip->draw();

	unsigned int sz = enemyShips.size();
	for (unsigned int i = 0; i < sz; i++)
	{
		Ship& ship = *enemyShips[i];
		ship.update();
		ship.draw();
		checkCollisions(ship);
		if (rand() % enemyFireRate == 1)
		{
			addProjectile("bad", ship.x, ship.y);
		}

		if (ship.x <= 0.0 || ship.x >= static_cast<double>(width))
		{
			ship.setVx(-ship.vx);
		}
		if (ship.y <= 0.0 || ship.y >= static_cast<double>(height))
		{
			ship.setVy(-ship.vy);
		}
	}

	enemyShips.erase(std::remove_if(enemyShips.begin(), enemyShips.end(), [](const std::unique_ptr<Ship>& ship) {
		return ship->shouldRemove();
	}), enemyShips.end());

	for (auto it = enemyShips.begin(); it != enemyShips.end(); ++it)
	{
		Ship& ship = **it;
		if (ship.shouldRemove())
		{
			enemyShips.erase(it--);
			std::cout << "NEW LENGTH " << enemyShips.size() << std::endl;
		}		
	}

	for (auto it = projectiles.begin(); it != projectiles.end(); ++it)
	{
		Projectile& proj = **it;
		proj.update();
		if (proj.shouldRemove())
		{
			projectiles.erase(it--);
		}
		else
		{
			proj.draw();
		}
	}

	//drawUI();

	return true;
}
