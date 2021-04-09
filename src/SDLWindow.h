#pragma once
#include <SDL.h>

//#define COLOR_0 15, 56, 15
//#define COLOR_1 48, 98, 48
//#define COLOR_2 139, 172, 15
//#define COLOR_3 155, 188, 15

#define COLOR_0 255, 239, 255
#define COLOR_1 206, 231, 222
#define COLOR_2 165, 214, 255
#define COLOR_3 24, 16, 16


// As seen on LazyFoo's SDL tutorials
class SDLWindow
{
public:
	//Intializes internals
	/*SDLWindow();*/

	//Creates window
	bool init(const char* title, int width, int height);

	bool init(const char* title, int width, int height, bool opened);

	//Handles window events
	void handleEvent(SDL_Event& e);

	//Focuses on window
	void focus();

	//Shows windows contents
	void render(uint8_t** matrix, int width, int height);

    void renderWrapping(uint8_t** matrix, uint8_t scx, uint8_t scy);

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

private:
	//Window data
	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
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
};

