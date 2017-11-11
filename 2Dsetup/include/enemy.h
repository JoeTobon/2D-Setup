#ifndef __enemy_h__
#define __enemy_h__

#include <SDL.h>
#include "simple_logger.h"
#include "entity.h"

/**
 * @brief  will cause the entity to approach the player at a consistant rate
 * @param  player the entity to approach (the player in this instance)
 * @param  enemy the entity to update (the enemy in this instance)
 */
void enemy_approach(Entity *player, Entity *enemy);

#endif