#include <SDL.h>
#include <physfs.h>
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "entity.h"
#include "simple_logger.h"
#include "game.h"
#include "player.h"
#include "enemy.h"
#include "audio.h"
#include "level.h"
#include "window.h"
#include "widgets.h"
#include "menus.h"


int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    const Uint8 * keys;
	int i, in;

    int mx,my;
    float mf = 0;
    Sprite *mouse;
	Level *level;
	Vector2D scale = vector2d(.1, .1);
	Vector2D scale2 = vector2d(3, 3);	
	Vector2D scale3 = vector2d(.1, .1);	

	//Entity Stuff
	Entity *hPotion, *iPotion, *bombE, *bandit, *knifeE, *shieldEnt;

	//Window
	Window *win;
	
    Vector4D mouseColor = {255,100,255,200};

	//Game Controller 1 handler
	SDL_GameController *controller = NULL;

    /*program initializtion*/
    init_logger("gf2d.log");
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "MTS",
        1200,
        720,
        1200,
        720,
        vector4d(0,0,0,255),
        0);
    gf2d_graphics_set_frame_delay(16);
    gf2d_sprite_init(1024);

	//entity system initialized
	entity_system_init(1024);

	//audio system initialized
	audio_system_init(256,16,4,1,1,1);

	//window system initialized
	window_system_init(10);

	//Button system initialized
	button_system_init(100);

    SDL_ShowCursor(SDL_DISABLE);

	//Controller support
	SDL_Init(SDL_INIT_GAMECONTROLLER);
	
	//Check for joysticks
    if( SDL_NumJoysticks() < 1 )
    {
		slog( "No controllers connected!" );
    }
    else
    {
		//Load joystick
		controller = SDL_GameControllerOpen(0);
            
		if(controller == NULL)
        {
			slog( "Unable to open controller! SDL Error: %s\n", SDL_GetError() );
        }
		else
		{
			slog("Controller is attached.");
		}
	}


    /*demo setup*/
	

	//Load all sprites in system
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16);

	//titleScreen
	titleScreen();

	//Load level
	level = level_new();

	//mainMenu
	main_menu(level);


	//Bow Stuff
	knifeE = entity_new();
	knifeE->type = knife;
	knifeE->scale = &scale3;
	
	
	/*Invincibility potion
	iPotion = entity_new();
	iPotion->type = ip;
	iPotion->sprite = gf2d_sprite_load_image("images/Items/Invincibility.png");
	iPotion->position = vector2d(350, 100);
	iPotion->scale = &scale;
	iPotion->spawned = 1;
	iPotion->boundingBox.x = iPotion->position.x;
	iPotion->boundingBox.y = iPotion->position.y;
	iPotion->boundingBox.w = 30;
	iPotion->boundingBox.h = 30;
	//Set timer now*/

	/*Bomb
	bombE = entity_new();
	bombE->type = bomb;
	bombE->sprite = gf2d_sprite_load_image("images/Items/Bomb.png");
	bombE->position= vector2d(250,500);
	bombE->scale = &scale;
	bombE->boundingBox.x = bombE->position.x;
	bombE->boundingBox.y = bombE->position.y;
	bombE->boundingBox.w = 100;
	bombE->boundingBox.h = 100;
	bombE->update = &bomb_update;

	//*/

	/*/Bandit Enemy
	bandit = entity_new();
	bandit->type = banditE;
	bandit->sprite = gf2d_sprite_load_all("images/Enemies/StreetThief.png", 32, 32, 10);
	bandit->position = vector2d(200, 400);
	bandit->scale = &scale2;
	bandit->boundingBox.x = bandit->position.x;
	bandit->boundingBox.y = bandit->position.y;
	bandit->boundingBox.w = 100;
	bandit->boundingBox.h = 100;*/

	//Bandit Knife
	//knife = entity_new();
	//knife->type = banditK;
	//knife->scale = &scale3;

	//Shield weapon
	shieldEnt = entity_new();
	shieldEnt->type = shield;
//	shieldEnt->position = vector2d(500, 200);
	//shieldEnt->sprite = gf2d_sprite_load_image("images/Weapons/shield.png");

	//Window
	win = window_new();
	win->position = vector2d(0, 0);
	win->bgcolor = vector4d(0, 200, 0, 0);
	win->main_window.x = win->position.x;
	win->main_window.y = win->position.y;
	win->main_window.w = 1200;
	win->main_window.h = 720;

	//play level music
	music_play(level->levelMusic);

    /*main game loop*/
    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        /*update things here*/
        SDL_GetMouseState(&mx,&my);
		
        mf+=0.1;

        if (mf >= 16.0)mf = 0;

		entity_update_all();
		entity_collide_all();
		//bandit_a();

		if(enemies_dead() == true)
		{
			slog("win!");
		}
		
        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first
		    //UI elements last

			gf2d_sprite_draw_image(level->background,vector2d(0,0));

           
            /*gf2d_sprite_draw(
                mouse,
                vector2d(mx,my),
                NULL,
                NULL,
                NULL,
                NULL,
                &mouseColor,
                (int)mf);*/

			//window_draw_all();

			entity_draw_all();
			
        gf2d_grahics_next_frame();// render current draw frame and skip to the next frame

		if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
        slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }

	//Close game controller
	SDL_GameControllerClose(controller);
    controller = NULL;


    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/

