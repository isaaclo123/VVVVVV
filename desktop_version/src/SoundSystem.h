#ifndef SOUNDSYSTEM_H
#define SOUNDSYSTEM_H

#include <SDL_mixer.h>

class MusicTrack
{
public:
    MusicTrack(const char* fileName);
    MusicTrack(SDL_RWops *rw);
    Mix_Music *m_music;
    bool m_isValid;
    static int s_num_tracks;
};

class SoundTrack
{
public:
    SoundTrack(const char* fileName);
    Mix_Chunk *sound;
    int size;
};

class SoundSystem
{
public:
	SoundSystem();
	void playMusic(MusicTrack* music);
};

#endif /* SOUNDSYSTEM_H */
