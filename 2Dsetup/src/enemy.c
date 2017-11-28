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
		slog("Enities not of type player or enemy");
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

//Handles movement for the bandit AI
void bandit_movement(Entity *player, Entity *bandit)
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
	speed = 3;

	if(!player || !bandit)
	{
		slog("Player or Bandit entity is null");
		return;
	}

	//calculate deltas
	deltaX = player->position.x - bandit->position.x;
	deltaY = player->position.y - bandit->position.y;

	//find unit vector
	pathResult = (deltaX * deltaX) + (deltaY * deltaY);
	pathResult = sqrt(pathResult);

	//normalize deltas
	xStep = deltaX/pathResult;
	yStep = deltaY/pathResult;

	//calculate move
	xMove = ((int)deltaX) * (speed) * (timeDelta);
	yMove = ((int)deltaY) * (speed) * (timeDelta);

	if(bandit->moveCount >= 3)
	{
		bandit->moveCount = 0;
	}

	if(bandit->moveCount >= 1)
	{
		
		goto move;
	}

	//move

	if(((bandit->boundingBox.x - 150) + (bandit->boundingBox.w + 300)) < player->boundingBox.x)
	{
		slog("not moving");
		return;
	}
	else if((player->boundingBox.x + player->boundingBox.w) < (bandit->boundingBox.x - 150))
	{
		return;
	}
	else if(((bandit->boundingBox.y - 150) + (bandit->boundingBox.h + 300)) < player->boundingBox.y)
	{
		return;
	}
	else if((player->boundingBox.y + player->boundingBox.h) < (bandit->boundingBox.y - 150))
	{
		return;
	}
	else
	{
		move: 

		bandit->moveCount++;
		slog("moving");
		bandit->position.x -= xMove;
		bandit->position.y -= yMove;

		//update bounding box
		bandit->boundingBox.x = bandit->position.x;
		bandit->boundingBox.y = bandit->position.y;
		bandit->boundingBox.w = 100;
		bandit->boundingBox.h = 100;
	}	
}

void bandit_attack(Entity *bandit, Entity *knife, Entity *player)
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
	speed = 4;

	if(!player || !bandit || !knife)
	{
		slog("Player or Bandit entity is null");
		return;
	}

	//Set knife starting point to bandit
	if(knife->spawnTime == -1)
	{
		knife->position.x = bandit->position.x;
		knife->position.y = bandit->position.y;
		knife->spawnTime = 0;

		//calculate deltas
		deltaX = player->position.x - knife->position.x;
		deltaY = player->position.y - knife->position.y;

		//find unit vector
		pathResult = (deltaX * deltaX) + (deltaY * deltaY);
		pathResult = sqrt(pathResult);

		//normalize deltas
		xStep = deltaX/pathResult;
		yStep = deltaY/pathResult;

		//calculate move
		xMove = ((int)deltaX) * (speed) * (timeDelta);
		yMove = ((int)deltaY) * (speed) * (timeDelta);

		bandit->movex = xMove;
		bandit->movey = yMove;
	}
	
	//Timer for knife
	if(knife->spawnTime >= 0)
	{
		knife->spawnTime += .1;

		knife->spawned = true;
		knife->sprite = gf2d_sprite_load_image("images/Weapons/knife.png");

		//Throw knife towards player
		knife->position.x += bandit->movex;
		knife->position.y += bandit->movey;
	}
	
	//reset timer, delete knife
	if(knife->spawnTime >= 12)
	{
		knife->spawnTime = -1;
		knife->spawned = false;
		knife->sprite = NULL;
	}

	//update bounding box
	knife->boundingBox.x = knife->position.x;
	knife->boundingBox.y = knife->position.y;
	knife->boundingBox.w = 10;
	knife->boundingBox.h = 10;

	
}