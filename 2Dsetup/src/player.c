#include <SDL.h>
#include "stdio.h"
#include "entity.h"
#include "player.h"
#include "simple_logger.h"
#include "audio.h"

void player_update(Entity *entity)
{
	const Uint8 *keys;
	Vector4D invinColor = vector4d(0, 255, 0, 0);
	
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

	if(entity->health <= 0)
	{
		//call game over
		entity->sword = false; 
		entity->shield = false;
		entity->knife = false;
	}

	//Equips sword to start
	if(entity->sword == false && entity->shield == false && entity->knife == false)
	{
		entity->sword = true;
	}

	//checks invincibility status
	if(entity->invincible == true)
	{
		entity->invinceT += .1;
		//entity->colorShift = &invinColor;
	}

	if(entity->invinceT >= 18)
	{
		entity->invincible = false;
		entity->invinceT = 0;
	}

	//used to make player move
	if(entity->type == player && entity->health > 0)
	{
		//movement with controller
		controller = SDL_GameControllerOpen(0);

		if(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX) > DEAD_ZONE && entity->position.x != 1100)
		{

			entity->position.x += 10;
		}
		if(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX) < -DEAD_ZONE && entity->position.x != -10)
		{
			entity->position.x -= 10;
		}
		if(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY) < -DEAD_ZONE && entity->position.y != -10)
		{
			entity->position.y -= 10;
		}
		if(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY) > DEAD_ZONE && entity->position.y != 600)
		{
			entity->position.y += 10;
		}

		//Switch weapons
		if(entity->health > 0)
		{
			if(SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_LEFTSHOULDER) && entity->sword == true)
			{
				entity->sword  = false;
				entity->shield = true;
			}
			else if(SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_LEFTSHOULDER) && entity->shield == true)
			{
				entity->shield = false;
				entity->knife = true;
			}
			else if(SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_LEFTSHOULDER) && entity->knife == true)
			{
				entity->knife  = false;
				entity->sword = true;
			}
		}
	}

	//updates bounding box
	entity->boundingBox.x = entity->position.x +20;
	entity->boundingBox.y = entity->position.y;
	entity->boundingBox.w = 80;
	entity->boundingBox.h = 120;
}

void player_attack(Entity *playerEnt, Entity *weaponEnt)
{
	Sprite *weaponS;
	const Uint8 *keys;
	
	//dead zone for controller
	const int DEAD_ZONE = 8000;

	//Game Controller 1 handler
	SDL_GameController *controller;
	weaponS = gf2d_sprite_load_image("images/Weapons/sword2.png");

	if(!playerEnt || !weaponEnt)
	{
		return;
	}

	//Delay between attacks
	if(weaponEnt->spawnTime >= 0)
	{
		weaponEnt->spawnTime += .1;
	}

	if(weaponEnt->spawnTime >= 3)
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

			sound_play(weaponEnt->entSound);
			weaponEnt->spawned = 1;
			weaponEnt->spawnTime = 0;

		}
		else if(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTX) > DEAD_ZONE && weaponEnt->spawned != 1)
		{
			weaponEnt->sprite = weaponS;

			weaponEnt->position.x = playerEnt->position.x + 80;
			weaponEnt->position.y = playerEnt->position.y;

			sound_play(weaponEnt->entSound);
			weaponEnt->spawned = 1;
			weaponEnt->spawnTime = 0;
		}
		else if(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTY) > DEAD_ZONE && weaponEnt->spawned != 1)
		{
			weaponEnt->sprite = weaponS;

			weaponEnt->position.x = playerEnt->position.x + 30;
			weaponEnt->position.y = playerEnt->position.y + 100;

			sound_play(weaponEnt->entSound);
			weaponEnt->spawned = 1;
			weaponEnt->spawnTime = 0;
		}
		else if(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTY) < -DEAD_ZONE && weaponEnt->spawned != 1)
		{
			weaponEnt->sprite = weaponS;

			weaponEnt->position.x = playerEnt->position.x + 30;
			weaponEnt->position.y = playerEnt->position.y - 100;

			sound_play(weaponEnt->entSound);
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
	}
}

void knife_Attack(Entity * playerEnt, Entity *knife)
{
	Sprite *knifeS;
	
	//dead zone for controller
	const int DEAD_ZONE = 8000;

	//Game Controller 1 handler
	SDL_GameController *controller;

	knifeS = gf2d_sprite_load_image("images/Weapons/knife.png");

	if(!knife)
	{
		slog("Knife does not exist");
		return;
	}

	//Delay between arrow spawn
	if(knife->spawnTime >= 0)
	{
		knife->spawnTime += .1;
	}

	if(knife->spawnTime >= 4.5)
	{
		knife->sprite = NULL;
		knife->spawned = 0;
		knife->spawnTime = -1;
	}


	//Spawn arrow in direction of right stick from player position
	if(playerEnt->type == player)
	{
		//attacking with controller
		controller = SDL_GameControllerOpen(0);

		if(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTX) < -DEAD_ZONE && knife->spawned != 1)
		{
			knife->sprite = knifeS;

			knife->position.x = playerEnt->position.x + 10;
			knife->position.y = playerEnt->position.y + 30;

			knife->spawned = 1;
			knife->spawnTime = 0;

			knife->direct = 'l';

		}
		else if(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTX) > DEAD_ZONE && knife->spawned != 1)
		{
			knife->sprite = knifeS;

			knife->position.x = playerEnt->position.x + 90;
			knife->position.y = playerEnt->position.y + 30;

			knife->spawned = 1;
			knife->spawnTime = 0;

			knife->direct = 'r';
		}
		else if(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTY) > DEAD_ZONE && knife->spawned != 1)
		{
			knife->sprite = knifeS;

			knife->position.x = playerEnt->position.x + 55;
			knife->position.y = playerEnt->position.y + 100;

			knife->spawned = 1;
			knife->spawnTime = 0;

			knife->direct = 'd';
		}
		else if(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTY) < -DEAD_ZONE && knife->spawned != 1)
		{
			knife->sprite = knifeS;

			knife->position.x = playerEnt->position.x + 55;
			knife->position.y = playerEnt->position.y - 10;

			knife->spawned = 1;
			knife->spawnTime = 0;

			knife->direct = 'u';
		}
	}

	//Make knife move depending on right stick press
	if(knife->direct == 'l')
	{
		knife->position.x -= 10;
	}
	else if(knife->direct == 'r')
	{
		knife->position.x += 10;
	}
	else if(knife->direct == 'd')
	{
		knife->position.y += 10;
	}
	else if(knife->direct == 'u')
	{
		knife->position.y -= 10;
	}

	//Detect collision if knife is spawned
	if(knife->spawned == 1)
	{
		//update bounding box
		knife->boundingBox.x = knife->position.x;
		knife->boundingBox.y = knife->position.y;
		knife->boundingBox.w = 30;
		knife->boundingBox.h = 30;
	}
}

void shield_Attack(Entity *playerEnt, Entity *shieldEnt)
{
	Sprite *shieldS;
	
	//dead zone for controller
	const int DEAD_ZONE = 8000;

	//Game Controller 1 handler
	SDL_GameController *controller;
	shieldS = gf2d_sprite_load_image("images/Weapons/shield.png");

	if(!playerEnt || !shieldEnt)
	{
		return;
	}

	//Delay between attacks
	if(shieldEnt->spawnTime >= 0)
	{
		shieldEnt->spawnTime += .1;
	}

	if(shieldEnt->spawnTime >= 3)
	{
		shieldEnt->sprite = NULL;
		shieldEnt->spawned = 0;
		shieldEnt->spawnTime = -1;
	}

	//Attacking
	if(playerEnt->type == player)
	{
		//attacking with controller
		controller = SDL_GameControllerOpen(0);

		if(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTX) < -DEAD_ZONE && shieldEnt->spawned != 1)
		{
			shieldEnt->sprite = shieldS;

			shieldEnt->position.x = playerEnt->position.x - 15;
			shieldEnt->position.y = playerEnt->position.y + 30;

			
		
			//sound_play(weaponEnt->entSound);
			shieldEnt->spawned = 1;
			shieldEnt->spawnTime = 0;

		}
		else if(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTX) > DEAD_ZONE && shieldEnt->spawned != 1)
		{
			shieldEnt->sprite = shieldS;

			shieldEnt->position.x = playerEnt->position.x + 80;
			shieldEnt->position.y = playerEnt->position.y + 30;

			//sound_play(weaponEnt->entSound);
			shieldEnt->spawned = 1;
			shieldEnt->spawnTime = 0;
		}
		else if(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTY) > DEAD_ZONE && shieldEnt->spawned != 1)
		{
			shieldEnt->sprite = shieldS;

			shieldEnt->position.x = playerEnt->position.x + 30;
			shieldEnt->position.y = playerEnt->position.y + 100;

			//sound_play(weaponEnt->entSound);
			shieldEnt->spawned = 1;
			shieldEnt->spawnTime = 0;
		}
		else if(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTY) < -DEAD_ZONE && shieldEnt->spawned != 1)
		{
			shieldEnt->sprite = shieldS;

			shieldEnt->position.x = playerEnt->position.x + 30;
			shieldEnt->position.y = playerEnt->position.y - 30;

			//sound_play(weaponEnt->entSound);
			shieldEnt->spawned = 1;
			shieldEnt->spawnTime = 0;
		}
	}

	//Detect collision if weapon is spawned
	if(shieldEnt->spawned == 1)
	{
		//update bounding box
		shieldEnt->boundingBox.x = shieldEnt->position.x;
		shieldEnt->boundingBox.y = shieldEnt->position.y;
		shieldEnt->boundingBox.w = 30;
		shieldEnt->boundingBox.h = 30;
	}
}