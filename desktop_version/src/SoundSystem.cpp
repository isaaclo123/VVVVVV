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
