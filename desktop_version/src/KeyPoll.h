#ifndef KEYPOLL_H
#define KEYPOLL_H

#include <string>
#include <vector>
#include <map> // FIXME: I should feel very bad for using C++ -flibit

#include "SDL.h"

#ifdef PSP
#include "SDL2_stub.h"
#endif

#include <pspctrl.h>
#include <psptypes.h>

enum Kybrd
{
	KEYBOARD_UP = PSP_CTRL_UP,
	KEYBOARD_DOWN = PSP_CTRL_DOWN,
	KEYBOARD_LEFT = PSP_CTRL_LEFT,
	KEYBOARD_RIGHT = PSP_CTRL_RIGHT,
	KEYBOARD_ENTER = PSP_CTRL_START,
	KEYBOARD_SPACE = PSP_CTRL_CIRCLE,

	KEYBOARD_w = SDLK_w,
	KEYBOARD_s = SDLK_s,
	KEYBOARD_a = SDLK_a,
	KEYBOARD_d = SDLK_d,
	KEYBOARD_m = SDLK_m,

	KEYBOARD_v = SDLK_v,
	KEYBOARD_z = SDLK_z,

	KEYBOARD_BACKSPACE = PSP_CTRL_CROSS
};

static SceCtrlLatch latch;

static SceCtrlData buttonInput;

#define PSP_BUTTONS_LENGTH 11

static int PSP_BUTTONS[PSP_BUTTONS_LENGTH] = {
    PSP_CTRL_START,
    PSP_CTRL_UP,
    PSP_CTRL_DOWN,
    PSP_CTRL_RIGHT,
    PSP_CTRL_LEFT,
    PSP_CTRL_CROSS,
    PSP_CTRL_CIRCLE,
    PSP_CTRL_SQUARE,
    PSP_CTRL_TRIANGLE,
    PSP_CTRL_LTRIGGER,
    PSP_CTRL_RTRIGGER
};

class KeyPoll
{
public:
	std::map<SDL_Keycode, bool> keymap;

	bool isActive;

	bool resetWindow;

	bool escapeWasPressedPreviously;
	bool quitProgram;
	bool toggleFullscreen;

	int sensitivity;

	void setSensitivity(int _value);

	KeyPoll();

	void enabletextentry();

	void disabletextentry();

	void Poll();

	bool isDown(SDL_Keycode key);

	bool isUp(SDL_Keycode key);

	bool isDown(std::vector<SDL_GameControllerButton> buttons);
	bool isDown(SDL_GameControllerButton button);
	bool controllerButtonDown();
	bool controllerWantsLeft(bool includeVert);
	bool controllerWantsRight(bool includeVert);

	int leftbutton, rightbutton, middlebutton;
	int mx, my;

	bool textentrymode;
	int keyentered, keybufferlen;
	bool pressedbackspace;
	std::string keybuffer;

	bool linealreadyemptykludge;

private:
	std::map<SDL_JoystickID, SDL_GameController*> controllers;
	std::map<SDL_GameControllerButton, bool> buttonmap;
	int xVel, yVel;
	bool useFullscreenSpaces;
	Uint32 wasFullscreen;
};

extern KeyPoll key;

#endif /* KEYPOLL_H */
