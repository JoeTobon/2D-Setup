#include <SDL.h>
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "entity.h"
#include "simple_logger.h"
#include "game.h"
#include "player.h"
#include "enemy.h"
#include "audio.h"

int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    const Uint8 * keys;
    Sprite *sprite, *titleScreen;
	int i, in;
	Sound *testS;
	Music *testM;
	float *counter;
	Bool *spawned;
	int titleBool = 0;

	//Used for Entity assignment
	Entity *entPlayer;
	Entity *enemyEnt;
	Entity *weaponEnt;
	Sprite *bug;
	Sprite *playerS;

    int mx,my;
    float mf = 0;
    Sprite *mouse;
	Vector4D *color;
    Vector4D mouseColor = {255,100,255,200};
	Vector4D skeletonC = {255, 0, 0,200};

	//Game Controller 1 handler
	SDL_GameController *controller = NULL;


	color = &skeletonC;
	counter = 0;
	spawned = false;

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
    sprite = gf2d_sprite_load_image("images/backgrounds/newBackground.png");
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16);
	bug = gf2d_sprite_load_image("images/Enemies/skeleton.png");
	titleScreen = gf2d_sprite_load_image("images/UI/titleMenu.png");
	
	//Load player sprite and define player entity (temp)
	playerS = gf2d_sprite_load_all("images/Soldier/player.png", 128, 128, 16);
	
	entPlayer = entity_new();
	entPlayer->type = player;
	entPlayer->update = &player_update;
	entPlayer->sprite = playerS;
	entPlayer->position = vector2d(200, 200);
	entPlayer->frame = (int)(mf);


	enemyEnt = entity_new();
	enemyEnt->type = enemy;
	//entPlayer->update = &enemy_update;
	enemyEnt->sprite = bug;
	enemyEnt->spawned = 1;
	enemyEnt->position = vector2d(0, 0);
	enemyEnt->frame = (int)(mf);
	enemyEnt->colorShift = color;

	weaponEnt = entity_new();
	weaponEnt->type = weapon;
	weaponEnt->spawned = 0;

	//test sound
	testS = sound_new("audio/swish_2.wav", 1, 1);
	

	//test music
	testM = music_new("audio/little town.mid", 1);
	music_play(testM);

	//titleScreen


	//title screen
	while(!titleBool)
	{
		SDL_PumpEvents();   // update SDL's internal event structures

		gf2d_graphics_clear_screen();// clears drawing buffers

			gf2d_sprite_draw_image(titleScreen,vector2d(0, 0));

		gf2d_grahics_next_frame();// render current draw frame and skip to the next frame

		//slog( "controller input: %i\n", SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_A));

		if(SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_A))
		{
			
			titleBool = 1;
		}

		slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
	}

    /*main game loop*/
    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        /*update things here*/
        SDL_GetMouseState(&mx,&my);
		
        mf+=0.1;

        if (mf >= 16.0)mf = 0;

		enemy_approach(entPlayer, enemyEnt);
		enemy_attack(entPlayer, enemyEnt);
		player_attack(entPlayer, enemyEnt, weaponEnt);
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

			entity_draw_all();
			
        gf2d_grahics_next_frame();// render current draw frame and skip to the next frame

		if(keys[SDL_SCANCODE_SPACE])
		{
			sound_play(testS);
		}

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
