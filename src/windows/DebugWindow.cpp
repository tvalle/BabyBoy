#include <iomanip>
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
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif defined(__APPLE__)
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

static void PushStyleCompact()
{
    ImGuiStyle& style = ImGui::GetStyle();
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(style.FramePadding.x, (float)(int)(style.FramePadding.y * 0.60f)));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(style.ItemSpacing.x, (float)(int)(style.ItemSpacing.y * 0.60f)));
}

static void PopStyleCompact()
{
    ImGui::PopStyleVar(2);
}

void DebugWindow::update()
{
    if (m_MarkedForDeletion)
        return;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    bool closed = false;
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    // ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::Begin("Debugger", &closed, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize);

    // PC Instructions
    ImGui::BeginGroup();
    
        static ImGuiTableFlags flags = ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;

        // PushStyleCompact();
        // ImGui::CheckboxFlags("ImGuiTableFlags_ScrollY", &flags, ImGuiTableFlags_ScrollY);
        // PopStyleCompact();

        // When using ScrollX or ScrollY we need to specify a size for our table container!
        // Otherwise by default the table will fit all available space, like a BeginChild() call.
        
        float firstHalfWidth = m_IO->DisplaySize.x * .60f;
        float secondHalfWidth = m_IO->DisplaySize.x * .40f;

        ImVec2 outer_size = ImVec2(firstHalfWidth, m_IO->DisplaySize.y * .65f);
        if (ImGui::BeginTable("table_scrolly", 3, flags, outer_size))
        {
            ImGui::TableSetupScrollFreeze(0, 1); // Make top row always visible
            ImGui::TableSetupColumn("PC", ImGuiTableColumnFlags_None);
            ImGui::TableSetupColumn("OP Code", ImGuiTableColumnFlags_None);
            ImGui::TableSetupColumn("Instruction", ImGuiTableColumnFlags_None);
            ImGui::TableHeadersRow();

            ImGuiListClipper clipper;
            const int memoryMapWhereROMIsLocated = 0x7FFF - 0x100;
            clipper.Begin(memoryMapWhereROMIsLocated);
            while (clipper.Step())
            {
                for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; row++)
                {
                    bool isCurrentPc = row == m_Soc->cpu.PC;

                    if (isCurrentPc && m_TrackPC) {
                        // ImGui::SetScrollHereY(row * 0.25f);
                    }

                    ImGui::TableNextRow();
                    for (int column = 0; column < 3; column++)
                    {
                        ImGui::TableSetColumnIndex(column);
                        // ImGui::Text("Hello %d,%d", column, row);

                        if (isCurrentPc) {
                            ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg1, ImGui::GetColorU32(ImVec4(0.7f, 0.3f, 0.3f, 0.65f)));
                        }

                        if (column == 0) {
                            ImGui::Text("0x%02X", row);
                        } else if (column == 1) {
                            uint8_t opcode = opcodes.at(m_Soc->ram.read(row)).opcode;
                            ImGui::Text("%02X", opcode);
                        } else if (column == 2) {
                            std::string s = opcodes.at(m_Soc->ram.read(row)).mnemonic;
                            char* c = &*s.begin();
                            ImGui::Text("%s", c);
                        }
                        

                    }
                }
            }
        }
        ImGui::EndTable(); ImGui::SameLine();
        ImGui::BeginGroup();
        if (ImGui::Button(m_Soc->isPaused ? "Continue" : "Pause   ")) {
            m_Soc->isPaused = !m_Soc->isPaused;
        }
        ImGui::SameLine();
        ImGui::PushButtonRepeat(true);
        if (ImGui::ArrowButton("right", ImGuiDir_Right)) {
            m_Soc->step();
        }
        ImGui::PopButtonRepeat();

        ImGui::SetNextItemWidth(secondHalfWidth - 50);
        char pc_ca[3];
        sprintf(pc_ca, "%x", m_Soc->cpu.PC);
        ImGui::InputText("PC", pc_ca, IM_ARRAYSIZE(pc_ca));
        bool filterByHeatmap = false;
        ImGui::Checkbox("Filter by heatmap", &filterByHeatmap);
        ImGui::Checkbox("Track PC", &m_TrackPC);

        ImGui::Spacing();

        ImGui::Text("AF"); ImGui::SameLine();
        char reg_af[5];
        sprintf(reg_af, "%02x%02x", m_Soc->cpu.a, m_Soc->cpu.f);
        ImGui::Text("%s", reg_af);

        ImGui::Text("BC"); ImGui::SameLine();
        char reg_bc[5];
        sprintf(reg_bc, "%02x%02x", m_Soc->cpu.b, m_Soc->cpu.c);
        ImGui::Text("%s", reg_bc);

        ImGui::Text("DE"); ImGui::SameLine();
        char reg_de[5];
        sprintf(reg_de, "%02x%02x", m_Soc->cpu.d, m_Soc->cpu.e);
        ImGui::Text("%s", reg_de);

        ImGui::Text("HL"); ImGui::SameLine();
        char reg_hl[5];
        sprintf(reg_hl, "%02x%02x", m_Soc->cpu.h, m_Soc->cpu.l);
        ImGui::Text("%s", reg_hl);

        ImGui::Text("SP"); ImGui::SameLine();
        char reg_sp[5];
        sprintf(reg_sp, "%04x", m_Soc->cpu.SP);
        ImGui::Text("%s", reg_sp);

        ImGui::EndGroup();
        ImGui::Text("Last instructions list");


    // if (ImGui::BeginListBox("##listbox 1", ImVec2(0, 100)))
    // {
    //     const int memoryMapWhereROMIsLocated = 0x7FFF - 0x100;
    //     for (int n = 0; n < memoryMapWhereROMIsLocated; n++)
    //     {
    //         std::string s = opcodes.at(m_Soc->ram.read(n)).mnemonic;
    //         char* c = &*s.begin();
    //         ImGui::Selectable(c, false);

    //     //     // const bool is_selected = (item_current_idx == n);
    //     //     // if (ImGui::Selectable(items[n], is_selected))
    //     //     //     item_current_idx = n;

    //     //     // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
    //     //     // if (is_selected)
    //     //     // ImGui::SetItemDefaultFocus();
    //     }
    //     ImGui::EndListBox();
    // }

    ImGui::EndGroup();


    ImGui::End();
    // ImGui::PopStyleVar(2);

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