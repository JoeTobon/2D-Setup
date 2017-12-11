#include "menus.h"

void titleScreen()
{
	Sprite *titleScreen;
	int titleBool = 0;
	SDL_GameController *controller;
	SDL_Event e;

	titleScreen = gf2d_sprite_load_image("images/UI/titleMenu.png");
	controller = SDL_GameControllerOpen(0);

	//title screen
	while(!titleBool)
	{
		while(SDL_PollEvent(&e) != 0)
		{
			gf2d_graphics_clear_screen();// clears drawing buffers

				gf2d_sprite_draw_image(titleScreen,vector2d(0, 0));

			gf2d_grahics_next_frame();// render current draw frame and skip to the next frame

			if(e.cbutton.button == SDL_CONTROLLER_BUTTON_A)
			{
				if(e.type == SDL_CONTROLLERBUTTONUP)
				{
					titleBool = 1;
				}
			}
		}

		slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
	}
}

void main_menu(Level *level)
{
	int menuBool = 0;
	float timer = 0;
	Bool stall, menuSwitch;
	SDL_GameController *controller;
	Button *play, *editor, *controls, *settings;
	SDL_Event e;

	//dead zone for controller
	const int DEAD_ZONE = 8000;

	controller = SDL_GameControllerOpen(0);

	stall = false;
	menuSwitch = false;

	//initialize buttons
	play	 = button_new();
	editor	 = button_new();
	controls = button_new();
	settings = button_new();

	//Set button attributes
	play->position.x = 450;
	play->position.y = 0;
	play->bounds.x = play->position.x;
	play->bounds.y = play->position.y + 50;
	play->hover = true;

	editor->position.x = 450;
	editor->position.y = 150;
	editor->bounds.x = editor->position.x;
	editor->bounds.y = editor->position.y + 50;

	controls->position.x = 450;
	controls->position.y = 300;
	controls->bounds.x = controls->position.x;
	controls->bounds.y = controls->position.y + 50;

	settings->position.x = 450;
	settings->position.y = 450;
	settings->bounds.x = settings->position.x;
	settings->bounds.y = settings->position.y + 50;

	//Menu Screen
	while(!menuBool)
	{
		SDL_PumpEvents();   // update SDL's internal event structures

		//Delays time between controller input
		if(stall == true)
		{
			timer += 0.1;
			
		}

        if (timer >= 2.0) 
		{
			timer = 0;
			stall = false;
		}

		button_hover_all();

		gf2d_graphics_clear_screen();// clears drawing buffers

			button_draw_all();

		gf2d_grahics_next_frame();// render current draw frame and skip to the next frame

		//Deals with controller input when left stick is down
		if(play->hover == true && (SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY) > DEAD_ZONE) && stall == false)
		{
			play->hover = false;
			editor->hover = true;
			stall = true;
		}
		if(editor->hover == true && (SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY) > DEAD_ZONE) && stall == false)
		{
			editor->hover = false;
			controls->hover = true;
			stall = true;
		}
		if(controls->hover == true && (SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY) > DEAD_ZONE) && stall == false)
		{
			controls->hover = false;
			settings->hover = true;
			stall = true;
		}
		if(settings->hover == true && (SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY) > DEAD_ZONE) && stall == false)
		{
			settings->hover = false;
			play->hover = true;
			stall = true;
		}

		//Deals with controller input when left stick is up
		if(play->hover == true && (SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY) < -DEAD_ZONE) && stall == false)
		{
			play->hover = false;
			settings->hover = true;
			stall = true;
		}
		if(editor->hover == true && (SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY) < -DEAD_ZONE) && stall == false)
		{
			editor->hover = false;
			play->hover = true;
			stall = true;
		}
		if(controls->hover == true && (SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY) < -DEAD_ZONE) && stall == false)
		{
			controls->hover = false;
			editor->hover = true;
			stall = true;
		}
		if(settings->hover == true && (SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY) < -DEAD_ZONE) && stall == false)
		{
			settings->hover = false;
			controls->hover = true;
			stall = true;
		}

		//If A is clicked, exit while loop call appropriate function
		if(SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_A))
		{
			menuBool = 1;
		}

		//If B is clicked, exit game entirely
		if(SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_B))
		{
			exit(0);
		}

		slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
	}

	//Functions to depending on button hovered over
	if(play->hover == true)
	{
		button_delete_all();
		level_Screen(level);
	}
	else if(editor->hover == true)
	{
		button_delete_all();

	}
	else if(controls->hover == true)
	{
		button_delete_all();
	}
	else if(settings->hover == true)
	{
		button_delete_all();
	}

}

void level_Screen(Level *level)
{
	Sprite *screen;
	int levelBool = 0;
	SDL_GameController *controller;
	const Uint8 * keys;

	screen = gf2d_sprite_load_image("images/UI/levelSelect.png");
	controller = SDL_GameControllerOpen(0);

	if(!level)
	{
		slog("level is null");
		return;
	}

	

	//title screen
	while(!levelBool)
	{
		SDL_PumpEvents();   // update SDL's internal event structures
		keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame

		gf2d_graphics_clear_screen();// clears drawing buffers

			gf2d_sprite_draw_image(screen,vector2d(0, 0));

		gf2d_grahics_next_frame();// render current draw frame and skip to the next frame

		if(SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_X) || keys[SDL_SCANCODE_SPACE])
		{
			level_load(level, "def/level/level1.level");
			slog("level 1 loaded");
			levelBool = 1;
		}
		else if(SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_B))
		{
			level_load(level, "def/level/level2.level");
			slog("level 1 loaded");
			levelBool = 1;
		}

		slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
	}
}