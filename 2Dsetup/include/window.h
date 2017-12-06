#ifndef __window_h__
#define __window_h__

#include <SDL.h>
#include <stdlib.h>
#include "gf2d_draw.h"
#include "gf2d_sprite.h"
#include "gf2d_types.h"
#include "gf2d_vector.h"
#include "gf2d_text.h"
#include "simple_logger.h"

typedef struct Window_S
{
	//window specific sdl objects
	SDL_Rect main_window;

	Bool inuse;

    
    Vector4D bgcolor;
    Bool fullscreen;

	Vector2D position; 
	Vector2D scale;
	Vector2D scaleCenter;
	Vector2D rotation;
	Vector2D flip;


	void (*update)(struct Window_S *self);  //used to update the enity's current states

}Window;

/**
 * @brief  initializes the window manager/system
 * @param  maxNum the maximum number of windows the system will handle
 */
void window_system_init(Uint32 maxNum);

/**
 * @brief   initialize a new window
 * @return pointer to window or null on error
 */
Window *window_new();

/**
 * @brief draws window to the screen
 * @param win the window to draw
 */
void window_draw(Window *win);

/**
 * @brief update the window passed as a parameter
 * @param win the window to update
 */
void window_update(Window *win);

/**
 * @brief  deletes/removes an window from the window system
 * @param  win a pointer to the window that is to be deleted
 */
void window_delete(Window *win);

/**
 * @brief draws all windows in use to the screen
 */
void window_draw_all();

/**
 * @brief updates all the window
 */
void window_update_all();

/**
 * @brief deletes all loaded window from memory does not close the window system
 */
void window_delete_all();

#endif