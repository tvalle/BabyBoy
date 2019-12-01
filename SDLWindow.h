#pragma once
#include <SDL.h>

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
	void render(uint8_t* matrix, int width, int height);

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

