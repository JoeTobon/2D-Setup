#include "level.h"
#include "simple_logger.h"
#include "entity.h"
#include <physfs.h>

typedef struct
{
	Uint32 maxLevel;
	Level *levelList;
} Levelmanager; 

static Levelmanager level_manager = {0};
static int count = 0;
static Bool playM = true;

void level_close()
{
	if (level_manager.levelList != NULL)
    {
		level_delete_all();
        free(level_manager.levelList);
    }

	memset(&level_manager, 0, sizeof(Levelmanager));

    slog("level system closed");
}


void level_system_init(Uint32 maxNum)
{
	if(maxNum <= 0)
	{
		slog("cannot intialize an entity manager for zero entities!");
        return;
	}

	memset(&level_manager, 0, sizeof(Levelmanager));
	level_manager.levelList = (Level *)malloc(sizeof(Level)*maxNum);;

	if(!level_manager.levelList)
	{
		slog("Failed to allocate level list");
		level_close();
		return;
	}

	memset(level_manager.levelList,0,sizeof(Level)*maxNum);
    level_manager.maxLevel = maxNum;

	atexit(level_close);

	slog("level system initialized");
}

Level *level_new()
{
	//Search through the entity manager for an unused address
	int i;

	for(i = 0; i < level_manager.maxLevel; i++)
	{
		if(level_manager.levelList[i].inuse == 0)
		{
			level_delete(&level_manager.levelList[i]); //clean up old data
			level_manager.levelList[i].inuse = 1; //Set ref count to 1. Address is now in use

			return &level_manager.levelList[i];		  //Return address of index in array
		}
	}
	
	slog("error: out of level addresses");
	exit(0);
	return  NULL;
}

void level_load(Level *level, char *filename)
{
	PHYSFS_File *file;
	char *pBuf;
	char buffer[512];
	char hold[512];
	int loop = 0;
	int incr = 0;

	if(!level)
	{
		slog("Level is null");
		return;
	}

	if(!filename)
	{
		slog("Filename is null");
		return;
	}

	if(!PHYSFS_exists(filename))
	{
		slog("Failed to load file: %s", filename);
		return;
	}

	file = PHYSFS_openRead(filename);

	pBuf = (char *)malloc(PHYSFS_fileLength(file));

	if(!pBuf)
	{
		slog("Failed to allocate pBuf");
		return;
	}

	memset(pBuf, 0, PHYSFS_fileLength(file));

	PHYSFS_readBytes(file, pBuf, PHYSFS_fileLength(file));
	PHYSFS_close(file);

	//slog("pBUF: %s", pBuf);

	while(sscanf(pBuf, " %s\n%n", buffer, &incr) == 1)
	{
	    if(pBuf[0] == '~')
		{
			return;
		}

		pBuf += incr;

		if(strcmp(buffer, "background:") == 0)
		{
			sscanf(pBuf, " %s\n%n", buffer, &incr);
			pBuf += incr;
			level->background = gf2d_sprite_load_image(buffer);

			slog("Level background: %s", buffer);
			//continue;
		}
		if(strcmp(buffer, "entFile:") == 0)
		{
			sscanf(pBuf, " %s\n%n", buffer, &incr);
			pBuf += incr;
			entity_load_all(buffer);
			slog("Entity File: %s", buffer);
			//continue;
		}
	}
}

void level_draw(Level *level)
{
	if(!level)
	{
		return;
	}

	gf2d_sprite_draw_image(level->background,vector2d(0,0));
}

void level_delete(Level *level)
{
	if(!level)
	{
		return;
	}

	memset(level,0,sizeof(Level));//clean up all other data
}

void level_draw_all()
{
	int i;

	for(i = 0; i < level_manager.maxLevel; i++)
	{
		if(level_manager.levelList[i].inuse == 0)
		{
			continue;
		}

		level_draw(&level_manager.levelList[i]);
	}
}

void level_delete_all()
{
	int i;

	for(i = 0; i < level_manager.maxLevel; i++)
	{
		level_delete(&level_manager.levelList[i]);// cleans up the data
	}
}

void next_level()
{
	Level *level = NULL;

	if(enemies_dead() == true && count == 0)
	{
		button_delete_all();
		window_delete_all();
		level_delete_all();

		level = level_new();

		slog("win!");
		count = 1;
		level_load(level, "assets/def/level/level2.level");
	}
	if(enemies_dead() == true && count == 1)
	{
		button_delete_all();
		window_delete_all();
		level_delete_all();

		level = level_new();

		slog("win!");
		count = 2;
		level_load(level, "assets/def/level/level3.level");
	}
	if(enemies_dead() == true && count == 2)
	{
		//Call win screen
		win_screen();
	}

}

void level_over(Bool status)
{
	if(status == true)
	{
		count = 0;
	}
}