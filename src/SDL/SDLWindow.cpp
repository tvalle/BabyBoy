#include <stdio.h>
#include <sstream>
#include "SDLWindow.h"

bool SDLWindow::init(const char *title, int width, int height)
{
    return init(title, width, height, true);
}

bool SDLWindow::init(const char *title, int width, int height, bool opened)
{
    //Create window
    mWindow = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, (opened ? SDL_WINDOW_SHOWN : SDL_WINDOW_HIDDEN) | SDL_WINDOW_RESIZABLE);
    if (mWindow != NULL)
    {
        mMouseFocus = true;
        mKeyboardFocus = true;
        mWidth = width;
        mHeight = height;

        //Create renderer for window
        mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
        if (mRenderer == NULL)
        {
            printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
            SDL_DestroyWindow(mWindow);
            mWindow = NULL;
        }
        else
        {
            //Initialize renderer color
            SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

            //Grab window identifier
            mWindowID = SDL_GetWindowID(mWindow);

            //Flag as opened
            mShown = opened;
        }

        // Create texture to draw pixels on
        mScreenTexture = SDL_CreateTexture(mRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, 160, 144);
        initializePixels();
    }
    else
    {
        printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
    }

    return mWindow != NULL && mRenderer != NULL;
}

void SDLWindow::handleEvent(SDL_Event &e)
{
    //If an event was detected for this window
    if (e.type == SDL_WINDOWEVENT && e.window.windowID == mWindowID)
    {
        //Caption update flag
        bool updateCaption = false;

        switch (e.window.event)
        {
            //Window appeared
        case SDL_WINDOWEVENT_SHOWN:
            mShown = true;
            break;

            //Window disappeared
        case SDL_WINDOWEVENT_HIDDEN:
            mShown = false;
            break;

            //Get new dimensions and repaint
        case SDL_WINDOWEVENT_SIZE_CHANGED:
            mWidth = e.window.data1;
            mHeight = e.window.data2;
            SDL_RenderPresent(mRenderer);
            break;

            //Repaint on expose
        case SDL_WINDOWEVENT_EXPOSED:
            SDL_RenderPresent(mRenderer);
            break;

            //Mouse enter
        case SDL_WINDOWEVENT_ENTER:
            mMouseFocus = true;
            updateCaption = true;
            break;

            //Mouse exit
        case SDL_WINDOWEVENT_LEAVE:
            mMouseFocus = false;
            updateCaption = true;
            break;

            //Keyboard focus gained
        case SDL_WINDOWEVENT_FOCUS_GAINED:
            mKeyboardFocus = true;
            updateCaption = true;
            break;

            //Keyboard focus lost
        case SDL_WINDOWEVENT_FOCUS_LOST:
            mKeyboardFocus = false;
            updateCaption = true;
            break;

            //Window minimized
        case SDL_WINDOWEVENT_MINIMIZED:
            mMinimized = true;
            break;

            //Window maxized
        case SDL_WINDOWEVENT_MAXIMIZED:
            mMinimized = false;
            break;

            //Window restored
        case SDL_WINDOWEVENT_RESTORED:
            mMinimized = false;
            break;
            //Hide on close
        case SDL_WINDOWEVENT_CLOSE:
            SDL_HideWindow(mWindow);
            break;
        }
    }
}

void SDLWindow::focus()
{
    //Restore window if needed
    if (!mShown)
    {
        SDL_ShowWindow(mWindow);
    }

    //Move window forward
    SDL_RaiseWindow(mWindow);
}

void SDLWindow::render(uint8_t **matrix, int width, int height)
{
    if (!mMinimized)
    {
        //Clear screen
        SDL_RenderClear(mRenderer);

        if (matrix != nullptr)
        {
            for (int i = 0; i < height; i++)
            {
                for (int j = 0; j < width; j++)
                {
                    //auto color = *((matrix + i * width) + j);
                    auto color = matrix[i][j];

                    if (color == 0b00)
                        pixels[i * 160 + j] = COLOR_0;
                    else if (color == 0b01)
                        pixels[i * 160 + j] = COLOR_1;
                    else if (color == 0b10)
                        pixels[i * 160 + j] = COLOR_2;
                    else if (color == 0b11)
                        pixels[i * 160 + j] = COLOR_3;
                }
            }
        }

        //Update screen
        SDL_UpdateTexture(mScreenTexture, nullptr, pixels, 160 * sizeof(int));
        SDL_RenderCopy(mRenderer, mScreenTexture, nullptr, nullptr);
        SDL_RenderPresent(mRenderer);
    }
}

void SDLWindow::free()
{
    SDL_DestroyWindow(mWindow);
}

void SDLWindow::initializePixels()
{
    pixels = new Uint32[160 * 144];
    memset(pixels, 255, 160 * 144 * sizeof(Uint32));
}

bool SDLWindow::hasKeyboardFocus()
{
    return mKeyboardFocus;
}

int SDLWindow::getWindowId()
{
    return mWindowID;
}
