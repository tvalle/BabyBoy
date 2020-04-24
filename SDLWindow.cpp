#include <stdio.h>
#include <sstream>
#include "SDLWindow.h"
bool SDLWindow::init(const char* title, int width, int height)
{
    return init(title, width, height, true);
}

bool SDLWindow::init(const char* title, int width, int height, bool opened)
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
    }
    else
    {
        printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
    }

    return mWindow != NULL && mRenderer != NULL;
}

void SDLWindow::handleEvent(SDL_Event& e)
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

        //Update window caption with new data
        if (updateCaption)
        {
            std::stringstream caption;
            caption << "SDL Tutorial - ID: " << mWindowID << " MouseFocus:" << ((mMouseFocus) ? "On" : "Off") << " KeyboardFocus:" << ((mKeyboardFocus) ? "On" : "Off");
            SDL_SetWindowTitle(mWindow, caption.str().c_str());
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

void SDLWindow::render(uint8_t** matrix, int width, int height)
{
    if (!mMinimized)
    {
        //Clear screen
        SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
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
                        SDL_SetRenderDrawColor(mRenderer, COLOR_0, 255);
                    else if (color == 0b01)
                        SDL_SetRenderDrawColor(mRenderer, COLOR_1, 255);
                    else if (color == 0b10)
                        SDL_SetRenderDrawColor(mRenderer, COLOR_2, 255);
                    else if (color == 0b11)
                        SDL_SetRenderDrawColor(mRenderer, COLOR_3, 255);

                    SDL_RenderDrawPoint(mRenderer, j, i);
                }
            }
        }

        //Update screen
        //SDL_RenderPresent(mRenderer);
    }
}

void SDLWindow::renderWrapping(uint8_t** matrix, uint8_t scx, uint8_t scy)
{
    if (!mMinimized)
    {
        //Clear screen
        SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(mRenderer);

        if (matrix != nullptr)
        {
            for (int i = 0; i < 144; i++)
            {
                for (int j = 0; j < 160; j++)
                {
                    auto color = matrix[uint8_t(i + scy)][uint8_t(j + scx)];

                    if (color == 0b00)
                        SDL_SetRenderDrawColor(mRenderer, COLOR_0, 255);
                    else if (color == 0b01)
                        SDL_SetRenderDrawColor(mRenderer, COLOR_1, 255);
                    else if (color == 0b10)
                        SDL_SetRenderDrawColor(mRenderer, COLOR_2, 255);
                    else if (color == 0b11)
                        SDL_SetRenderDrawColor(mRenderer, COLOR_3, 255);

                    SDL_RenderDrawPoint(mRenderer, j, i);
                }
            }
        }

        //Update screen
        SDL_RenderPresent(mRenderer);
    }
}

void SDLWindow::free()
{
    SDL_DestroyWindow(mWindow);
}
