#include <SDL.h>
#include "gf2d_sprite.h"
#include "gf2d_types.h"
#include "gf2d_vector.h"
#include "gf2d_text.h"

typedef struct Entity_S
{
	Bool inuse;							     //used to keep track of entity's use in system

	Sprite   *sprite;						 //sprite associated with entity 
	Vector2D position;						 //where the entity appears on the screen (using x and y coordinates)
	Vector2D *scale;
	Vector3D *rotation;
	Vector2D *scaleCenter;
	Vector2D *flip;
	Vector4D *colorShift;
	Uint32	 frame;
	
	Vector2D velocity;						 //how fast the enity is moving in a given direction
	int health;								 //keeps track of entity's health

	void (*update)(struct Entity_S *self);  //used to update the enity's current states

}Entity;

/**
 * @brief  initializes the enity manager/system
 * @param  maximum number of enities the system can handle
 */
void entity_system_init(Uint32 maxNum);

/**
 * @brief   initialize a new entity
 * @param   none
 * @returns pointer to entity or null on error
 */
Entity *entity_new();

/**
 * @brief free an entity back to the entity manager
 * Stays in memory until the space is needed
 * @param the entity to free
 */
void entity_free(Entity *entity);

/**
 * @brief  deletes/removes an entity from the entity system
 * @param  a pointer to the entity that is to be deleted
 */
void entity_delete(Entity *entity);

/**
 * @brief deletes all loaded entities from memory
 * does not close the entity system
 */
void entity_clear_all();

/**
 * @brief  updates the entity
 * @param  
 */
void entity_update();

/**
 * @brief  updates all the entity
 * @param  
 */
void entity_update_all();

/**
 * @brief  draws entity to the screen
 * @param  the entity to draw
 */
void entity_draw(Entity *entity);

/**
 * @brief  draws all entities in use to the screen
 */
void entity_draw_all();

