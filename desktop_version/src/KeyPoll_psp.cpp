#include "KeyPoll.h"
#include <stdio.h>
#include <string.h>
#include <utf8/unchecked.h>

#include <pspctrl.h>
#include <psptypes.h>

void KeyPoll::setSensitivity(int _value)
{
	switch (_value)
	{
		case 0:
			sensitivity = 28000;
			break;
		case 1:
			sensitivity = 16000;
			break;
		case 2:
			sensitivity = 8000;
			break;
		case 3:
			sensitivity = 4000;
			break;
		case 4:
			sensitivity = 2000;
			break;
	}

}

KeyPoll::KeyPoll()
{
	xVel = 0;
	yVel = 0;
	setSensitivity(2);

	quitProgram = 0;
	textentrymode=true;
	keybuffer="";
	leftbutton=0; rightbutton=0; middlebutton=0;
	mx=0; my=0;
	resetWindow = 0;
	toggleFullscreen = false;
	pressedbackspace=false;

	useFullscreenSpaces = false;
  // GUSARBA: Disabled
  /*
	if (strcmp(SDL_GetPlatform(), "Mac OS X") == 0)
	{
		useFullscreenSpaces = true;
		const char *hint = SDL_GetHint(SDL_HINT_VIDEO_MAC_FULLSCREEN_SPACES);
		if (hint != NULL)
		{
			useFullscreenSpaces = (strcmp(hint, "1") == 0);
		}
	}
  */

	linealreadyemptykludge = false;
}


// Keypoll for PSP buttons
static inline bool pspKeyIsDown(int key) {
	return (latch.uiMake & key);
}

static inline bool pspKeyIsUp(int key) {
	return (latch.uiBreak & key);
}

void KeyPoll::enabletextentry()
{
	keybuffer="";
	textentrymode = true;
  // GUSARBA: Disabled at the moment
	//SDL_StartTextInput();
}

void KeyPoll::disabletextentry()
{
	textentrymode = false;
  // GUSARBA: Disabled at the moment
	//SDL_StopTextInput();
}

static inline void pollLatch() {
	sceCtrlReadLatch(&latch);
}

static inline void pollPad() {
	sceCtrlReadBufferPositive(&buttonInput, 1);
}

void KeyPoll::Poll()
{
	sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

        pollPad();
        pollLatch();

        for (int i=0; i < PSP_BUTTONS_LENGTH; i++) {
            int button = PSP_BUTTONS[i];
            if (pspKeyIsDown(button)) {
                // printf("button %i down\n", button);
                keymap[button] = true;
            }
            if (pspKeyIsUp(button)) {
                // printf("button %i up\n", button);
                keymap[button] = false;
            }
        }
}

bool KeyPoll::isDown(SDL_Keycode key)
{
	return keymap[key];
}

bool KeyPoll::isUp(SDL_Keycode key)
{
	return !keymap[key];
}

bool KeyPoll::isDown(std::vector<SDL_GameControllerButton> buttons)
{
	for (size_t i = 0; i < buttons.size(); i += 1)
	{
		if (buttonmap[buttons[i]])
		{
			return true;
		}
	}
	return false;
}

bool KeyPoll::isDown(SDL_GameControllerButton button)
{
	return buttonmap[button];
}

bool KeyPoll::controllerButtonDown()
{
	for (
		SDL_GameControllerButton button = SDL_CONTROLLER_BUTTON_A;
		button < SDL_CONTROLLER_BUTTON_DPAD_UP;
		button = (SDL_GameControllerButton) (button + 1)
	) {
		if (isDown(button))
		{
			return true;
		}
	}
	return false;
}

bool KeyPoll::controllerWantsLeft(bool includeVert)
{
	return (	buttonmap[SDL_CONTROLLER_BUTTON_DPAD_LEFT] ||
			xVel < 0 ||
			(	includeVert &&
				(	buttonmap[SDL_CONTROLLER_BUTTON_DPAD_UP] ||
					yVel < 0	)	)	);
}

bool KeyPoll::controllerWantsRight(bool includeVert)
{
	return (	buttonmap[SDL_CONTROLLER_BUTTON_DPAD_RIGHT] ||
			xVel > 0 ||
			(	includeVert &&
				(	buttonmap[SDL_CONTROLLER_BUTTON_DPAD_DOWN] ||
					yVel > 0	)	)	);
}
