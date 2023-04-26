#include "Input.h"

Input* Input::instance = nullptr;;

Input::Input()
{
    m_keyState = 0xFF;
}

Input* Input::GetInstance()
{
    if (instance == nullptr) {
        instance = new Input();
    }
    
    return instance;
}

void Input::handleKeyPress(SDL_Keycode key, bool isKeyDown)
{
    // m_keyState = 0xCF;

    // if (isKeyDown)
    // {
    //     if (isAction) {
    //         if (key == SDLK_z) m_keyState &= 0b11011110;
    //         if (key == SDLK_x) m_keyState &= 0b11011101;
    //         if (key == SDLK_SPACE) m_keyState &= 0b11011011;
    //         if (key == SDLK_RETURN) m_keyState &= 0b11010111;
    //     }

    //     if (isDirection) {
    //         if (key == SDLK_RIGHT) m_keyState &= 0b11101110;
    //         if (key == SDLK_LEFT) m_keyState &= 0b11101101;
    //         if (key == SDLK_UP) m_keyState &= 0b11101011;
    //         if (key == SDLK_DOWN) m_keyState &= 0b11100111;
    //     }
    // } else {
    //     if (isAction) {
    //         if (key == SDLK_z) m_keyState |= ~0b11111110;
    //         if (key == SDLK_x) m_keyState |= ~0b11111101;
    //         if (key == SDLK_SPACE) m_keyState |= ~0b11111011;
    //         if (key == SDLK_RETURN) m_keyState |= ~0b11110111;
    //     }

    //     if (isDirection) {
    //         if (key == SDLK_RIGHT) m_keyState |= ~0b11111110;
    //         if (key == SDLK_LEFT) m_keyState |= ~0b11111101;
    //         if (key == SDLK_UP) m_keyState |= ~0b11111011;
    //         if (key == SDLK_DOWN) m_keyState |= ~0b11110111;
    //     }
    // }
}

uint8_t Input::getState()
{
    m_keyState = 0xCF;

    const Uint8 *keyboard_state_array = SDL_GetKeyboardState(NULL);
    // SDL_Event event;
    // SDL_PollEvent(&event);

        // if (keyboard_state_array[SDL_SCANCODE_UP])
        // {
    // if(event.type == SDL_KEYDOWN)
    {
        if (isDirection) {
            if (keyboard_state_array[SDL_SCANCODE_Z]) m_keyState &= 0b11111110;
            if (keyboard_state_array[SDL_SCANCODE_X]) m_keyState &= 0b11111101;
            if (keyboard_state_array[SDL_SCANCODE_SPACE]) m_keyState &= 0b11111011;
            if (keyboard_state_array[SDL_SCANCODE_RETURN]) m_keyState &= 0b11110111;
        }

        if (isAction) {
            if (keyboard_state_array[SDL_SCANCODE_RIGHT]) m_keyState &= 0b11111110;
            if (keyboard_state_array[SDL_SCANCODE_LEFT]) m_keyState &= 0b11111101;
            if (keyboard_state_array[SDL_SCANCODE_UP]) m_keyState &= 0b11111011;
            if (keyboard_state_array[SDL_SCANCODE_DOWN]) m_keyState &= 0b11110111;
        }
    } 
    // else if (event.type == SDL_KEYUP) {
    //     if (isAction) {
    //         if (keyboard_state_array[SDL_SCANCODE_Z]) m_keyState |= ~0b11111110;
    //         if (keyboard_state_array[SDL_SCANCODE_X]) m_keyState |= ~0b11111101;
    //         if (keyboard_state_array[SDL_SCANCODE_SPACE]) m_keyState |= ~0b11111011;
    //         if (keyboard_state_array[SDL_SCANCODE_RETURN]) m_keyState |= ~0b11110111;
    //     }

    //     if (isDirection) {
    //         if (keyboard_state_array[SDL_SCANCODE_RIGHT]) m_keyState |= ~0b11111110;
    //         if (keyboard_state_array[SDL_SCANCODE_LEFT]) m_keyState |= ~0b11111101;
    //         if (keyboard_state_array[SDL_SCANCODE_UP]) m_keyState |= ~0b11111011;
    //         if (keyboard_state_array[SDL_SCANCODE_DOWN]) m_keyState |= ~0b11110111;
    //     }
    // }

    return m_keyState;
}