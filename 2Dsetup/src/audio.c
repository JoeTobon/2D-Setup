#include <SDL.h>
#include "stdio.h"
#include "simple_logger.h"
#include "audio.h"

//Used to manage all the sounds in the game
typedef struct 
{
	Uint32 maxSounds;
	Sound *soundList;
}SoundManager;

void gf2d_audio_init(Uint32 maxSounds, Uint32 channels, Uint32 channelGroups, Uint32 maxMusic, Uint8  enableMP3, Uint8  enableOgg)
{
}