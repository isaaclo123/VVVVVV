#ifndef SOUNDSYSTEM_H
#define SOUNDSYSTEM_H

#include <SDL_mixer.h>
// #include <dc/sound/sfxmgr.h>
//#include <AL/al.h>
//#include <AL/alc.h>

class MusicTrack
{
public:
	MusicTrack(const char* fileName);
// #ifndef PSP
        MusicTrack(SDL_RWops *rw);
        Mix_Music *m_music;
// #else
	// int m_music;
// #endif
	bool m_isValid;
  static int s_num_tracks;
};

// TODO
#define snd_stream_hnd_t int

class SoundTrack
{
public:
	SoundTrack(const char* fileName);
// #ifndef PSP
    Mix_Chunk *sound;
//   int size;
// #else
  // unsigned char *sound;
  //ALuint sound;
  //ALsizei size;
  // sfxhnd_t sound;
// #endif
};

class SoundSystem
{
public:
	SoundSystem();
	void playMusic(MusicTrack* music);
};

#endif /* SOUNDSYSTEM_H */
