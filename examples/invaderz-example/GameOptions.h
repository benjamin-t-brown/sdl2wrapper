#pragma once

static struct
{
	const int width = 800;
	const int height = 600;
	const int spriteSize = 32;
	const int playerSpeed = 3;
	const int playerProjDamage = 1;
	const int playerProjSpeed = 5;
	const int playerFireCooldown = 20;
	const int playerShipHP = 20;
	const int enemyShipHP = 2;
	const int enemyFireRate = 3000;
	const int enemyShipMaxSpeed = 4;
	const int enemyProjDamage = 1;
	const int enemyProjSpeed = 3;
	const int enemyProjDamage2 = 2;
	const int enemyProjSpeed2 = 5;
	const int shipCollideDamage = 3;
	const int pointsPerDestroyedShip = 100;
	const int pointsPerDestroyedShip2 = 1000;
	const int pointsLostPerShot = 1;
} GameOptions;
