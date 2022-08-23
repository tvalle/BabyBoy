#pragma once
#include <SDL.h>

//#define COLOR_0 15, 56, 15
//#define COLOR_1 48, 98, 48
//#define COLOR_2 139, 172, 15
//#define COLOR_3 155, 188, 15

#define COLOR_0 0xffefffff
#define COLOR_1 0xcee7deff
#define COLOR_2 0xa5d6ffff
#define COLOR_3 0x181010ff

class SDLWindow
{
public:
	//Intializes internals
	/*SDLWindow();*/

	//Creates window
	bool init(const char *title, int width, int height);

	bool init(const char *title, int width, int height, bool opened);

	//Handles window events
	void handleEvent(SDL_Event &e);

	//Focuses on window
	void focus();

	//Shows windows contents
	void render(uint8_t **matrix, int width, int height);

	void renderWrapping(uint8_t **matrix, uint8_t scx, uint8_t scy);

	//Deallocates internals
	void free();

	//Window dimensions
	int getWidth();
	int getHeight();

	//Window focii
	bool hasMouseFocus();
	bool hasKeyboardFocus();
	bool isMinimized();
	bool isShown();

	int getWindowId();

private:
	//Window data
	SDL_Window *mWindow;
	SDL_Renderer *mRenderer;
	SDL_Texture *mScreenTexture;
	int mWindowID;

	//Window dimensions
	int mWidth;
	int mHeight;

	//Window focus
	bool mMouseFocus;
	bool mKeyboardFocus;
	bool mFullScreen;
	bool mMinimized;
	bool mShown;

	Uint32 *pixels;
	void initializePixels();
};
