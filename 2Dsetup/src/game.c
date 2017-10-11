#include <SDL.h>
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "entity.h"
#include "simple_logger.h"
#include "game.h"
#include "player.h"
#include "audio.h"

int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    const Uint8 * keys;
    Sprite *sprite;
	int i, in;

	//Used for Entity assignment
	Entity *entPlayer;
	Entity *enemyEnt;
	Sprite *bug;
	Sprite *playerS;

    int mx,my;
    float mf = 0;
    Sprite *mouse;
    Vector4D mouseColor = {255,100,255,200};

	//Game Controller 1 handler
	SDL_GameController *controller = NULL;

    /*program initializtion*/
    init_logger("gf2d.log");
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "gf2d",
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
    sprite = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16);
	bug = gf2d_sprite_load_all("images/space_bug.png",128,128,16);
	
	//Load player sprite and define player entity (temp)
	playerS = gf2d_sprite_load_image("images/idle/0000.png");
	
	entPlayer = entity_new();
	entPlayer->type = player;
	entPlayer->update = &player_update;
	entPlayer->sprite = playerS;
	entPlayer->position = vector2d(100, 100);
	entPlayer->frame = (int)(mf);


	enemyEnt = entity_new();
	enemyEnt->type = enemy;
	enemyEnt->sprite = bug;
	enemyEnt->position = vector2d(0, 0);
	enemyEnt->frame = (int)(mf);

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
		        
        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first
            gf2d_sprite_draw_image(sprite,vector2d(0,0));

            //UI elements last
            gf2d_sprite_draw(
                mouse,
                vector2d(mx,my),
                NULL,
                NULL,
                NULL,
                NULL,
                &mouseColor,
                (int)mf);

			
			enemyEnt->frame = (int)mf;
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
