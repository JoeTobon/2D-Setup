#include <SDL.h>
#include "stdio.h"
#include "entity.h"
#include "player.h"
#include "simple_logger.h"

void player_load(Entity *player, char *filename)
{
	FILE *file;

	if(!player)
	{
		slog("player does not exist");
		return;
	}

	if(!filename)
	{
		slog("file does not exist");
		return;
	}

	file = fopen(filename, "r");

	if (!file)
	{
		slog("Failed to open file (%s) for reading", filename);
		return;
	}

	fread(player, sizeof(Entity), 1,file);  //what to read, how big, how many, where to save

	fclose(file);
}

void player_save(Entity *player, char *filename)
{
	FILE *file;

	if(!player)
	{
		slog("player does not exist");
		return;
	}

	if(!filename)
	{
		slog("file does not exist");
		return;
	}

	//Make sure the user can't save a non-player entity to a player file
	if(player->type != player)
	{
		slog("Entity is not a player. Can't save to player file.");
		return;
	}

	file = fopen(filename, "w");

	if (!file)
	{
		slog("Failed to open file (%s) for reading", filename);
		return;
	}

	fwrite(player, sizeof(Entity), 1, file);  //what to write, how big, how many, where to save

	fclose(file);
}

void player_update(Entity *entity)
{
	const Uint8 *keys;
	
	//dead zone for controller
	const int DEAD_ZONE = 8000;

	//Game Controller 1 handler
	SDL_GameController *controller;

	if(!entity)
	{
		return;
	}

	if(!entity->inuse)
	{
		return;
	}

	//used to make player move
	if(entity->type == player)
	{
		//movement with keys
		keys = SDL_GetKeyboardState(NULL);

		if(keys[SDL_SCANCODE_D])
		{
			entity->position.x += 10;
		}
		if(keys[SDL_SCANCODE_A])
		{
			entity->position.x -= 10;
		}
		if(keys[SDL_SCANCODE_W])
		{
			entity->position.y -= 10;
		}
		if(keys[SDL_SCANCODE_S])
		{
			entity->position.y += 10;
		}

		//movement for controller
		controller = SDL_GameControllerOpen(0);

		if(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX) < -DEAD_ZONE)
		{
			entity->position.x -= 10;
		}
		if(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX) > DEAD_ZONE)
		{
			entity->position.x += 10;
		}
		if(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY) > DEAD_ZONE)
		{
			entity->position.y += 10;
		}
		if(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY) < -DEAD_ZONE)
		{
			entity->position.y -= 10;
		}
	}

	//updates bounding box
	entity->boundingBox.x = entity->position.x;
	entity->boundingBox.y = entity->position.y;
	entity->boundingBox.w = 50;
	entity->boundingBox.h = 100;

}

void player_attack(Entity *playerEnt, Entity *enemyEnt, Entity *weaponEnt)
{
	Sprite *weaponS;
	const Uint8 *keys;
	
	//dead zone for controller
	const int DEAD_ZONE = 8000;

	//Game Controller 1 handler
	SDL_GameController *controller;
	weaponS = gf2d_sprite_load_image("images/Weapons/sword2.png");

	if(!playerEnt || !enemyEnt || !weaponEnt)
	{
		return;
	}

	//Delay between attacks
	if(weaponEnt->spawnTime >= 0)
	{
		weaponEnt->spawnTime += .1;
	}

	if(weaponEnt->spawnTime >= 6)
	{
		weaponEnt->sprite = NULL;
		weaponEnt->spawned = 0;
		weaponEnt->spawnTime = -1;
	}

	//Attacking
	if(playerEnt->type == player)
	{
		//attacking with controller
		controller = SDL_GameControllerOpen(0);

		if(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTX) < -DEAD_ZONE && weaponEnt->spawned != 1)
		{
			weaponEnt->sprite = weaponS;

			weaponEnt->position.x = playerEnt->position.x - 10;
			weaponEnt->position.y = playerEnt->position.y;

			weaponEnt->spawned = 1;
			weaponEnt->spawnTime = 0;

		}
		else if(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTX) > DEAD_ZONE && weaponEnt->spawned != 1)
		{
			weaponEnt->sprite = weaponS;

			weaponEnt->position.x = playerEnt->position.x + 80;
			weaponEnt->position.y = playerEnt->position.y;

			weaponEnt->spawned = 1;
			weaponEnt->spawnTime = 0;
		}
		else if(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTY) > DEAD_ZONE && weaponEnt->spawned != 1)
		{
			weaponEnt->sprite = weaponS;

			weaponEnt->position.x = playerEnt->position.x + 30;
			weaponEnt->position.y = playerEnt->position.y + 100;

			weaponEnt->spawned = 1;
			weaponEnt->spawnTime = 0;
		}
		else if(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTY) < -DEAD_ZONE && weaponEnt->spawned != 1)
		{
			weaponEnt->sprite = weaponS;

			weaponEnt->position.x = playerEnt->position.x + 30;
			weaponEnt->position.y = playerEnt->position.y - 100;

			weaponEnt->spawned = 1;
			weaponEnt->spawnTime = 0;
		}
	}

	//Detect collision if weapon is spawned
	if(weaponEnt->spawned == 1)
	{
		//update bounding box
		weaponEnt->boundingBox.x = weaponEnt->position.x;
		weaponEnt->boundingBox.y = weaponEnt->position.y;
		weaponEnt->boundingBox.w = 30;
		weaponEnt->boundingBox.h = 110;

		//detect collision
		if(entity_collsion(weaponEnt, enemyEnt) == true)
		{
			entity_delete(enemyEnt);
		}
	}
}