#include "DebugWindow.h"
#include "../Opcode.h"
#include "../OpcodesArray.h"

DebugWindow::DebugWindow(SoC *soc, WindowManager *windowManager)
{
    m_Soc = soc;
    m_WindowManager = windowManager;
    m_Window = nullptr;
    m_GLContext = nullptr;
    m_IO = nullptr;
}

void DebugWindow::init()
{
#if defined(__APPLE__)
    // GL 3.2 Core + GLSL 150
    const char *glsl_version = "#version 150";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
    // GL 3.0 + GLSL 130
    const char *glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

    m_Window = SDL_CreateWindow("Debug Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, window_flags);

    // SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    m_GLContext = SDL_GL_CreateContext(m_Window);

    SDL_GL_MakeCurrent(m_Window, m_GLContext);
    SDL_GL_SetSwapInterval(1); // Enable vsync

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    // ImGuiIO& io = ImGui::GetIO(); (void)io;
    m_IO = &(ImGui::GetIO());
    (void)m_IO;

    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(m_Window, m_GLContext);
    ImGui_ImplOpenGL3_Init(glsl_version);
}

void DebugWindow::update()
{
    if (m_MarkedForDeletion)
        return;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    //ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.

    if (ImGui::BeginListBox("##listbox 1", ImVec2(0, 100)))
    {
        const int memoryMapWhereROMIsLocated = 0x7FFF - 0x100;
         for (int n = 0; n < memoryMapWhereROMIsLocated; n++)
         {
            std::string s = opcodes.at(m_Soc->ram.read(n)).mnemonic;
             char* c = &*s.begin();
            ImGui::Selectable(c, false);

        //     // const bool is_selected = (item_current_idx == n);
        //     // if (ImGui::Selectable(items[n], is_selected))
        //     //     item_current_idx = n;

        //     // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
        //     // if (is_selected)
        //     // ImGui::SetItemDefaultFocus();
        }
        ImGui::EndListBox();
    }

    //ImGui::End();

    ImGui::Render();
    glViewport(0, 0, (int)m_IO->DisplaySize.x, (int)m_IO->DisplaySize.y);
    glClearColor(m_BGColor.x * m_BGColor.w, m_BGColor.y * m_BGColor.w, m_BGColor.z * m_BGColor.w, m_BGColor.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(m_Window);
}

void DebugWindow::updateEvent(SDL_Event e)
{
    ImGui_ImplSDL2_ProcessEvent(&e);
    if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE && e.window.windowID == SDL_GetWindowID(m_Window))
    {
        m_WindowManager->remove(this);
        destroy();
        return;
    }
}

void DebugWindow::destroy()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(m_GLContext);
    SDL_DestroyWindow(m_Window);

    m_MarkedForDeletion = true;
}

bool DebugWindow::markedForDeletion()
{
    return m_MarkedForDeletion;
}