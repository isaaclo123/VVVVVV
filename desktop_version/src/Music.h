#ifndef MUSIC_H
#define MUSIC_H

#include "SoundSystem.h"

#include <vector>

#define musicroom(rx, ry) ((rx) + ((ry) * 20))

    static const char SOUND_TRACK_PATHS[28][29] = {
        "sounds/jump.wav",
        "sounds/jump2.wav",
        "sounds/hurt.wav",
        "sounds/souleyeminijingle.wav",
        "sounds/coin.wav",
        "sounds/save.wav",
        "sounds/crumble.wav",
        "sounds/vanish.wav",
        "sounds/blip.wav",
        "sounds/preteleport.wav",
        "sounds/teleport.wav",
        "sounds/crew1.wav",
        "sounds/crew2.wav",
        "sounds/crew3.wav",
        "sounds/crew4.wav",
        "sounds/crew5.wav",
        "sounds/crew6.wav",
        "sounds/terminal.wav",
        "sounds/gamesaved.wav",
        "sounds/crashing.wav",
        "sounds/blip2.wav",
        "sounds/countdown.wav",
        "sounds/go.wav",
        "sounds/crash.wav",
        "sounds/combine.wav",
        "sounds/newrecord.wav",
        "sounds/trophy.wav",
        "sounds/rescue.wav"
    };


#ifdef VVV_COMPILEMUSIC
    static const char MUSIC_TRACK_PATHS[16][41] = {
        "data/music/0levelcomplete.ogg",
        "data/music/1pushingonwards.ogg",
        "data/music/2positiveforce.ogg",
        "data/music/3potentialforanything.ogg",
        "data/music/4passionforexploring.ogg",
        "data/music/5intermission.ogg",
        "data/music/6presentingvvvvvv.ogg",
        "data/music/7gamecomplete.ogg",
        "data/music/8predestinedfate.ogg",
        "data/music/9positiveforcereversed.ogg",
        "data/music/10popularpotpourri.ogg",
        "data/music/11pipedream.ogg",
        "data/music/12pressurecooker.ogg",
        "data/music/13pacedenergy.ogg",
        "data/music/14piercingthesky.ogg",
        "data/music/predestinedfatefinallevel.ogg"
    };
#endif

class musicclass
{
public:
        int playMusic(int t, int loop, int ms);
        // void freeMusic();

	void init();

	void play(int t);
	void haltdasmusik();
	void silencedasmusik();
	void fadeMusicVolumeIn(int ms);
	void fadeout();
	void processmusicfadein();
	void processmusic();
	void niceplay(int t);

	void changemusicarea(int x, int y);

        int currentsong;
	int musicfade, musicfadein;
	int resumesong;

	void playef(int t);

	// std::vector<SoundTrack> soundTracks;
	SoundTrack soundTracks[28];
	// std::vector<MusicTrack> musicTracks;
	MusicTrack musicTracks[16];

	SoundSystem soundSystem;
	bool safeToProcessMusic;

	int nicechange;
	int nicefade;

	bool m_doFadeInVol;
	int FadeVolAmountPerFrame;
	int musicVolume;

	float volume;

	bool custompd;

	int fadeoutqueuesong; // -1 if no song queued
	bool dontquickfade;

	// MMMMMM mod settings
	bool mmmmmm;
	bool usingmmmmmm;
};

extern musicclass music;

#endif /* MUSIC_H */
