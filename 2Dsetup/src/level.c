#include "level.h"
#include "simple_logger.h"
#include "entity.h"
#include <physfs.h>

Level *level_new()
{
	Level *defaultL;

	defaultL = (Level *)malloc(sizeof(Level));

	if(!defaultL)
	{
		slog("Failed to allocate level");
		return;
	}

	memset(defaultL,0,sizeof(Level));

	return defaultL;
}

void level_load(Level *level, char *filename)
{
	PHYSFS_File *file;
	char *pBuf;
	char buffer[512];
	char hold[512];
	int incr = 0;
	int loop = 0;

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

	slog("pBUF: %s", pBuf);
	
	

	while(sscanf(pBuf, " %s\n%n", buffer, &incr) == 1)
	{
		pBuf += incr;

		if(pBuf[0] == '~')
		{
			return;
		}

		if(strcmp(buffer, "background:") == 0)
		{
			sscanf(pBuf, " %s\n%n", buffer, &incr);
			pBuf += incr;
			level->background = gf2d_sprite_load_image(buffer);

			slog("Level background: %s", buffer);
			//continue;
		}
		if(strcmp(buffer, "levelMusic:") == 0)
		{
			sscanf(pBuf, " %s\n%n", buffer, &incr);
			pBuf += incr;
			slog("Level music: %s", hold);
			//continue;
		}
		if(strcmp(buffer, "loop:") == 0)
		{
			sscanf(pBuf, " %s\n%n", buffer, &incr);
			pBuf += incr;
			slog("Music loop: %i", loop);

			//initialize music
			level->levelMusic = music_new(hold, loop);

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