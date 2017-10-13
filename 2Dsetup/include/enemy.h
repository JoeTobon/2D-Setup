#ifndef __enemy_h__
#define __enemy_h__

#include <SDL.h>
#include "simple_logger.h"
#include "entity.h"

/**
 * @brief  will load a enemy entity and its various attributes from a file
 * @param  enemhy entity to load to, name of file to load from
 */
void enemy_load(Entity *enemy, char *filename);

/**
 * @brief  will save a enemy entity and its various attributes to a file
 * @param  enemy entity to save, name of file to save to 
 */
void enemy_save(Entity *enemy, char *filename);

/**
 * @brief  will update various attributes of the enemy
 * @param  the entity to update (the enemy in this instance)
 */
void enemy_update(Entity *enemy);

/**
 * @brief  will update various attributes of the enemy
 * @param  the entity to update (the enemy in this instance)
 */
void enemy_think(Entity *enemy);

/**
 * @brief  will update various attributes of the enemy
 * @param  the entity to update (the enemy in this instance)
 */
void enemy_approach(Entity *player, Entity *enemy);

void enemy_attack(Entity *playerEnt, Entity *enemyEnt);

#endif