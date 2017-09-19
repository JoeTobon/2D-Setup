#include "simple_logger.h"
#include "entity.h"
#include "gf2d_types.h"
#include "gf2d_sprite.h"

typedef struct
{
	Uint32 maxEnt;
	Entity *entList;
} Entitymanager; 

static Entitymanager entity_manager;

void entity_close()
{
	entity_clear_all;
	if (entity_manager.entList != NULL)
    {
        free(entity_manager.entList);
    }
	entity_manager.entList = NULL;
	entity_manager.maxEnt = 0;
    slog("entity system closed");
}

void entity_system_init(Uint32 maxNum)
{
	if(maxNum == 0)
	{
		slog("cannot intialize an entity manager for zero entities!");
        return;
	}

	entity_manager.maxEnt = maxNum;
	entity_manager.entList = (Entity *)malloc(sizeof(Entity)*maxNum);
	memset(entity_manager.entList,0,sizeof(Entity)*maxNum);

	 slog("entity system initialized");

	atexit(entity_close);
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

	entity->ref_count--;
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
		if(entity_manager.entList[i].ref_count == 0)
		{
			entity_delete(&entity_manager.entList[i]); //clean up old data
			entity_manager.entList[i].ref_count == 1; //Set ref count to 1. Address is now in use

			//Initialize various attributes of entity here

			return &entity_manager.entList[i];		  //Return address of index in array
		}
	}

	slog("error: out of entity addresses");
    return NULL;
}

void entity_draw(Entity *entity)
{
	gf2d_sprite_draw(entity->sprite, entity->position, entity->scale, entity->scaleCenter, 
					    entity->rotation, entity->flip, entity->colorShift, entity->frame);
}