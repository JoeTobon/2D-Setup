#ifndef __level_h__
#define __level_h__

#include <stdio.h>
#include "gf2d_sprite.h"
#include "simple_logger.h"
#include "audio.h"

typedef struct level_S
{
	Sprite *background;
	Music  *levelMusic;
}Level;

//Dynamically load level from file
void level_load(Level *level, char *filename);

Level *level_new();

#endif