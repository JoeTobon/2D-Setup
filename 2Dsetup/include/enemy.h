#ifndef __enemy_h__
#define __enemy_h__

#include <SDL.h>
#include "simple_logger.h"
#include "entity.h"

/**
 * @brief  will update various attributes of an enemy update
 * @param  enemy the entity to update (the enemy in this instance)
 */
void enemy_update(Entity *player, Entity *enemy);

/**
 * @brief  will handle the dropping of an item/weapon if an enemy has something to drop
 * @param  enemy the entity that has the item/weapon to drop
 */
void enemy_drop(Entity *enemy);

/**
 * @brief  will cause the entity to approach the player at a consistant rate
 * @param  player the entity to approach (the player in this instance)
 * @param  enemy the entity to move (the enemy in this instance)
 */
void skeleton_approach(Entity *player, Entity *enemy);

/**
 * @brief  handles the bandit entity's movement. Will stay in one spot and attack player until player approaches. 
 * @param  player the player entity to run from 
 * @param  bandit the bandit entity to move
 */
void bandit_movement(Entity *player, Entity *bandit);

/**
 * @brief  handles the bandit entity's attack
 * @param  bandit the entity attacking
 * @param  knife the entity that the bandit is attacking with
 * @param  player the entity the bandit is trying to attack
 */
void bandit_attack(Entity *bandit, Entity *knife, Entity *player);

#endif