#include "window.h"

typedef struct
{
	Uint32 winMax;
	Window *winList;
}Windowmanager;

static Windowmanager window_manager = {0};

void window_close()
{
	if (window_manager.winList != NULL)
    {
		window_delete_all();
        free(window_manager.winList);
    }

	memset(&window_manager, 0, sizeof(Windowmanager));

    slog("Window system closed");
}

void window_system_init(Uint32 maxNum)
{
	if(maxNum <= 0)
	{
		slog("cannot intialize a window manager for zero windows!");
        return;
	}

	memset(&window_manager, 0, sizeof(Windowmanager));
	window_manager.winList = (Window *)malloc(sizeof(Window)*maxNum);

	if(!window_manager.winList)
	{
		slog("Failed to allocate window list");
		window_close();
		return;
	}

	memset(window_manager.winList,0,sizeof(Window)*maxNum);
    window_manager.winMax = maxNum;

	atexit(window_close);

	slog("window system initialized");
}

Window *window_new()
{
	//Search through the window manager for an unused address
	int i;

	for(i = 0; i < window_manager.winMax; i++)
	{
		if(window_manager.winList[i].inuse == 0)
		{
			window_delete(&window_manager.winList[i]);	//clean up old data
			window_manager.winList[i].inuse = 1;		//Set inuse to 1. Address is now in use

			//Initialize various default attributes of window here

			return &window_manager.winList[i];		  //Return address of index in array
		}
	}
	
	slog("error: out of window addresses");
	exit(0);
	return  NULL;
}

void window_draw(Window *win)
{
	if(!win)
	{
		slog("Window is null");
		return;
	}

	//Fill rectangle that will act as window
	gf2d_draw_rect(win->main_window, win->bgcolor, true);
}

void window_update(Window *win);

void window_delete(Window *win)
{
	if(!win)
	{
		return;
	}

	memset(win,0,sizeof(Window));	//clean up all other data
}

void window_draw_all()
{
	int i;

	for(i = 0; i < window_manager.winMax; i++)
	{
		if(window_manager.winList[i].inuse == 0)
		{
			continue;
		}

		window_draw(&window_manager.winList[i]);
	}
}

void window_update_all()
{
	int i; 

	for(i = 0; i < window_manager.winMax; i++)
	{
		if(window_manager.winList[i].inuse == 0)
		{
			continue;
		}

		//if window has no update function go to next entity
		if(!window_manager.winList[i].update)
		{
			continue;
		}

		window_manager.winList[i].update(&window_manager.winList[i]);
	}
}

void window_delete_all()
{
	int i;

	for(i = 0; i < window_manager.winMax; i++)
	{
		window_delete(&window_manager.winList[i]);
	}
}