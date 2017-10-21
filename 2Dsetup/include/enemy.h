#ifndef __enemy_h__
#define __enemy_h__

#include <SDL.h>
#include "simple_logger.h"
#include "entity.h"

/**
 * @brief  will cause the entity to approach the player at a consistant rate
 * @param  the entity to update (the enemy in this instance)
 */
void enemy_approach(Entity *player, Entity *enemy);

/**
 * @brief  will cause the enemy to attack the player once it reaches their position
 * @param  the player entity to attack, the enemy entity to do the attack
 */
void enemy_attack(Entity *playerEnt, Entity *enemyEnt);

void enemy_attack_all();

#endif