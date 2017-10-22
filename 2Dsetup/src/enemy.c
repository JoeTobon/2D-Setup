#include <SDL.h>
#include <math.h>
#include <stdio.h>
#include "simple_logger.h"
#include "entity.h"
#include "enemy.h"
#include "gf2d_sprite.h"


//For skeleton
void enemy_approach(Entity *playerEnt, Entity *enemyEnt)
{
	float deltaX;
	float deltaY;
	float xStep;
	float yStep;
	float pathResult;

	int speed;
	float timeDelta;
	int xMove;
	int yMove;

	timeDelta = .01;
	speed = 2;

	if(!playerEnt || !enemyEnt)
	{
		return;
	}

	if(playerEnt->type != player && enemyEnt->type != enemy)
	{
		slog("Enities not of tyope player or enemy");
		return;
	}

	//calculate deltas
	deltaX = playerEnt->position.x - enemyEnt->position.x;
	deltaY = playerEnt->position.y - enemyEnt->position.y;

	//find unit vector
	pathResult = (deltaX * deltaX) + (deltaY * deltaY);
	pathResult = sqrt(pathResult);

	//normalize deltas
	xStep = deltaX/pathResult;
	yStep = deltaY/pathResult;

	//move
	
	xMove = ((int)deltaX) * (speed) * (timeDelta);
	yMove = ((int)deltaY) * (speed) * (timeDelta);

	if(deltaX == 0 && deltaY == 0)
	{
		return;
	}

	enemyEnt->position.x += xMove;
	enemyEnt->position.y += yMove;

	//updates bounding box
	enemyEnt->boundingBox.x = enemyEnt->position.x;
	enemyEnt->boundingBox.y = enemyEnt->position.y;
	enemyEnt->boundingBox.w = 60;
	enemyEnt->boundingBox.h = 100;
}


