#pragma once

#include <SDL.h>
#include <SDL_opengl.h>

#include "../Window.h"
#include "../SoC.h"

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_sdl.h"
#include "../imgui/imgui_impl_opengl3.h"

class DebugWindow : public Window
{
public:
    DebugWindow(SoC *soc);

    void init() override;
    void update() override;
    void destroy() override;

private:
    SoC *m_Soc;

    SDL_Window *m_Window;
    SDL_GLContext m_GLContext;
    ImGuiIO *m_IO;

    ImVec4 m_BGColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
};