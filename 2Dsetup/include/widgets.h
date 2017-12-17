#ifndef __widgets_h__
#define __widgets_h__

#include <SDL.h>
#include "gf2d_sprite.h"
#include "gf2d_types.h"
#include "gf2d_vector.h"
#include "gf2d_text.h"
#include "gf2d_draw.h"
#include "simple_logger.h"

typedef struct Button_S
{
	SDL_Rect bounds;

	Bool inuse;
	Bool hover;
	Bool clicked;

	Sprite *image;
	Sprite *label;

	Vector2D *scale;
	Vector3D *rotation;
	Vector2D *scaleCenter;
	Vector2D *flip;
	Vector4D *colorShift;
	Uint32	 frame;
	Vector2D position;

	void (*update)(struct Button_S *self);
}Button;

/**
 * @brief  initializes the button manager/system
 * @param  maxNum the maximum number of buttons the system will handle
 */
void button_system_init(Uint32 maxNum);

/**
 * @brief  initialize a new button
 * @return pointer to button or null on error
 */
Button *button_new();

/**
 * @brief draws button to the screen
 * @param button the button to draw
 */
void button_draw(Button *button);

/**
 * @brief  deletes/removes a button from the button system
 * @param  button a pointer to the button that is to be deleted
 */
void button_delete(Button *button);

/**
 * @brief draws all buttons in use to the screen
 */
void button_draw_all();

/**
 * @brief updates all the buttons
 */
void button_update_all();

/**
 * @brief deletes all loaded buttons from memory does not close the button system
 */
void button_delete_all();

/**
 * @brief   Checks to see if a button is hovered over
 * @param   button the button that is checked to see if hovered
 * @return  True if button is hovered false if it is not
 */
void button_hover(Button *button);

/**
 * @brief   Will cycle through all buttons in the system to see which ones are hovered over
 */
void button_hover_all();

/**
 * @brief   Checks to see if a button is clicked
 * @param   button the button that is checked to see if clicked
 * @return  True if button is clicked false if it is not
 */
void button_clicked(Button *button);

#endif