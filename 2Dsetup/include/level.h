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

/**
 * @brief will load a level based on the level's filename
 * @param level the level to load
 * @param filename the name of the level file to load
 */
void level_load(Level *level, char *filename);

/**
 * @brief will initialize and allocate address for a new level
 * @return a pointer to the memory address of the new level
 */
Level *level_new();

#endif