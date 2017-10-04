#include "simple_logger.h"
#include "entity.h"
#include "gf2d_types.h"
#include "gf2d_sprite.h"

typedef struct
{
	Uint32 maxEnt;
	Entity *entList;
} Entitymanager; 

static Entitymanager entity_manager = {0};

void entity_close()
{
	if (entity_manager.entList != NULL)
    {
		entity_clear_all;
        free(entity_manager.entList);
    }

	memset(&entity_manager, 0, sizeof(Entitymanager));

    slog("entity system closed");
}

void entity_system_init(Uint32 maxNum)
{
	if(maxNum == 0)
	{
		slog("cannot intialize an entity manager for zero entities!");
        return;
	}

	memset(&entity_manager, 0, sizeof(Entitymanager));
	entity_manager.entList = (Entity *)malloc(sizeof(Entity)*maxNum);

	if(!entity_manager.entList)
	{
		slog("Failed to allocate entity list");
		entity_close();
		return;
	}

	memset(entity_manager.entList,0,sizeof(Entity)*maxNum);
    entity_manager.maxEnt = maxNum;

	atexit(entity_close);

	slog("entity system initialized");
}

void entity_delete(Entity *entity)
{
	if(!entity)
	{
		return;
	}

	memset(entity,0,sizeof(Entity));//clean up all other data
}

void entity_free(Entity *entity)
{
	if(!entity)
	{
		return;	
	}

	entity->inuse = 0;
}

void entity_clear_all()
{
	int i;

	for(i = 0; i < entity_manager.maxEnt; i++)
	{
		entity_delete(&entity_manager.entList[i]);// cleans up the data
	}
}

Entity *entity_new()
{
	//Search through the entity manager for an unused address
	int i;

	for(i = 0; i < entity_manager.maxEnt; i++)
	{
		if(entity_manager.entList[i].inuse == 0)
		{
			entity_delete(&entity_manager.entList[i]); //clean up old data
			entity_manager.entList[i].inuse = 1; //Set ref count to 1. Address is now in use

			//Initialize various default attributes of entity here
			entity_manager.entList[i].type = -1;

			return &entity_manager.entList[i];		  //Return address of index in array
		}
	}
	
	slog("error: out of entity addresses");
	exit(0);
	return  NULL;
}

void entity_update(Entity *entity)
{
	const Uint8 *keys;
	
	//Event handler
    SDL_Event e;

	//dead zone for controller
	const int DEAD_ZONE = 8000;

    //Normalized direction
    int xDir = 0;
    int yDir = 0;

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

		//Handle events on queue
        while( SDL_PollEvent( &e ) != 0 )
        {	
            if( e.type == SDL_JOYAXISMOTION )
            {
				//Motion on controller 0
                if( e.jaxis.which == 0 )
                {                        
                     //X axis motion
                     if( e.jaxis.axis == 0 )
                     {
						//Left of dead zone
                        if( e.jaxis.value < -DEAD_ZONE )
                        {
							entity->position.x -= 10;
                        }
                        //Right of dead zone
                        else if( e.jaxis.value > DEAD_ZONE )
                        {
							entity->position.x += 10;
                        }
					}
					//Y axis motion
					else if( e.jaxis.axis == 1 )
					{
						//Below of dead zone
						if( e.jaxis.value < -DEAD_ZONE )
						{
						   entity->position.y -= 10;
						}
						//Above of dead zone
						else if( e.jaxis.value > DEAD_ZONE )
						{
							entity->position.y += 10;
						}
					 }
			    }
		     }
	     }
	}
}

void entity_update_all()
{
	int i;

	for(i = 0; i < entity_manager.maxEnt; i++)
	{
		if(entity_manager.entList[i].inuse == 0)
		{
			continue;
		}

		entity_update(&entity_manager.entList[i]);
	}
}

void entity_draw(Entity *entity)
{
	if(!entity)
	{
		return;
	}


	gf2d_sprite_draw(entity->sprite, entity->position, entity->scale, entity->scaleCenter, 
					    entity->rotation, entity->flip, entity->colorShift, entity->frame);
}

void entity_draw_all()
{
	int i;

	for(i = 0; i < entity_manager.maxEnt; i++)
	{
		if(entity_manager.entList[i].inuse == 0)
		{
			continue;
		}

		entity_draw(&entity_manager.entList[i]);
	}
}