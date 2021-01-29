#include <SDL.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include "SoundSystem.h"
#include "FileSystemUtils.h"

#include "SDL2_stub.h"

MusicTrack::MusicTrack(const char* fileName)
{
	m_music = Mix_LoadMUS(fileName);
	m_isValid = true;
	if(m_music == NULL)
	{
		printf("Unable to load Ogg Music file: %s\n", Mix_GetError());;
		m_isValid = false;
	}
}

MusicTrack::MusicTrack(SDL_RWops *rw)
{
	m_music = Mix_LoadMUS_RW(rw);
	m_isValid = true;
	if(m_music == NULL)
	{
		printf("Unable to load Magic Binary Music file: %s\n", Mix_GetError());
		m_isValid = false;
	}
}

SoundTrack::SoundTrack(const char* fileName)
{
	sound = NULL;

	unsigned char *mem;
	size_t length = 0;
	FILESYSTEM_loadFileToMemory(fileName, &mem, &length);
	SDL_RWops *fileIn = SDL_RWFromMem(mem, length);
	sound = Mix_LoadWAV_RW(fileIn, 1);
	if (length)
	{
		FILESYSTEM_freeMemory(&mem);
	}

	if (sound == NULL)
	{
		printf("Unable to load WAV file: %s\n", Mix_GetError());
	}
}

SoundSystem::SoundSystem()
{
	int audio_rate = 44100;
        uint16_t audio_format = AUDIO_S16LSB;
	int audio_channels = 1;
	int audio_buffers = 2048;

        int flags=MIX_INIT_OGG|MIX_INIT_MOD;
        int initted;

        printf("SoundSystem Setup\n");

        if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
        {
                printf("Unable to initialize audio subsystem: %s\n", Mix_GetError());
		SDL_assert(0 && "Unable to initialize audio!");
        }

        initted=Mix_Init(flags);
        if(initted&flags != flags) {
            printf("Mix_Init: Failed to init required ogg support!\n");
            printf("Mix_Init: %s\n", Mix_GetError());
        }

	if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0)
	{
                printf("Unable to open audio: %s\n", Mix_GetError());
		SDL_assert(0 && "Unable to initialize audio!");
	}

        Mix_AllocateChannels(4);
}

void SoundSystem::playMusic(MusicTrack* music)
{
	if(!music->m_isValid)
	{
		printf("Invalid mix specified: %s\n", Mix_GetError());
	}
	if(Mix_PlayMusic(music->m_music, 0) == -1)
	{
		printf("Unable to play Ogg file: %s\n", Mix_GetError());
	}
}
