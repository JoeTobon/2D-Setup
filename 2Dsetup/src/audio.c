#include <SDL.h>
#include <string.h>
#include "stdio.h"
#include "simple_logger.h"
#include "audio.h"


//Used to manage all the sounds in the game
typedef struct 
{
	Uint32 maxSounds;
	Sound *soundList;
}SoundManager;

static SoundManager sound_manager = {0};

void audio_system_close()
{
	if (sound_manager.soundList != NULL)
    {
		sound_clear_all();
        free(sound_manager.soundList);
    }

	memset(&sound_manager, 0, sizeof(SoundManager));
	
	slog("Audio system is closed.");
}

void audio_system_init(Uint32 maxSounds, Uint32 channels, Uint32 channelGroups, Uint32 maxMusic, Uint8  enableMP3, Uint8  enableOgg)
{
	int flags = 0;

    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024)==-1)
    {
        slog("Failed to open audio: %s\n", SDL_GetError());
        return;
    }

	if (enableMP3)
    {
        flags |= MIX_INIT_MP3;
    }

    if (enableOgg)
    {
        flags |= MIX_INIT_OGG;
    }

    if (!(Mix_Init(flags) & flags))
    {
        slog("failed to initialize some audio support: %s",SDL_GetError());
    }

	//Initializes list for sounds
	if(maxSounds <= 0 || maxMusic <= 0)
	{
		slog("Can't initialize a list of zero sounds or musics");
		return;
	}

	memset(&sound_manager, 0, sizeof(SoundManager));
	sound_manager.soundList = (Sound *)malloc(sizeof(Sound)*maxSounds);

	if(!sound_manager.soundList)
	{
		slog("Failed to allocate sound list");
		audio_system_close();
		return;
	}

	memset(sound_manager.soundList, 0, sizeof(Sound)*maxSounds);
    sound_manager.maxSounds = maxSounds;

	atexit(audio_system_close); 
	atexit(Mix_CloseAudio);
	atexit(Mix_Quit); 

	slog("audio system initialized");
}

void sound_free(Sound *sound)
{
	if(!sound)
	{
		return;
	}

	sound->inuse = 0;
	memset(sound,0,sizeof(Sound));//clean up all other data
}
void sound_clear_all()
{
	int i;

	for(i = 0; i < sound_manager.maxSounds; i++)
	{
		sound_free(&sound_manager.soundList[i]);
	}
}

void sound_play(Sound *sound)
{
	if(!sound)
	{
		return;
	}

	Mix_PlayChannel(sound->channel, sound->chunk, sound->loop);
}

Sound *sound_new(char filename[100], int loop, int channel)
{
	int i;

	//search through the sound list for an unused address
	for(i = 0; i < sound_manager.maxSounds; i++)
	{
		if(sound_manager.soundList[i].inuse == 0)
		{
			sound_free(&sound_manager.soundList[i]); //cleans up old data
			sound_manager.soundList[i].inuse = 1; //Set ref count to 1. Address is now in use

			//Initialize various attributes of audio here
			strcpy(sound_manager.soundList[i].filename, filename);
			sound_manager.soundList[i].loop = loop;
			sound_manager.soundList[i].channel = channel;

			sound_manager.soundList[i].chunk = Mix_LoadWAV(sound_manager.soundList[i].filename);
			
			return &sound_manager.soundList[i];
		}
	}

	slog("error: out of addresses for sounds");
	exit(0);
	return NULL;
}