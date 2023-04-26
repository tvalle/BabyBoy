#include "DebugWindow.h"
#include "../Debug.h"
#include <iomanip>
#include <sstream>

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

    m_Window = SDL_CreateWindow("Debug Window", 50, SDL_WINDOWPOS_CENTERED, 800, 600, window_flags);

    m_GLContext = SDL_GL_CreateContext(m_Window);

    SDL_GL_MakeCurrent(m_Window, m_GLContext);
    SDL_GL_SetSwapInterval(1); // Enable vsync

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    m_IO = &(ImGui::GetIO());
    (void)m_IO;

    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(m_Window, m_GLContext);
    ImGui_ImplOpenGL3_Init(glsl_version);

    createInstructionsList();
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
    ImGui::Begin("Debugger", &closed, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize);

    // PC Instructions
    ImGui::BeginGroup();
    
        static ImGuiTableFlags flags = ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;

        float firstHalfWidth = m_IO->DisplaySize.x * .60f;
        float secondHalfWidth = m_IO->DisplaySize.x * .40f;

        ImVec2 outer_size = ImVec2(firstHalfWidth, m_IO->DisplaySize.y * .65f);
        if (ImGui::BeginTable("table_scrolly", 4, flags, outer_size))
        {
            ImGui::TableSetupScrollFreeze(0, 1); // Make top row always visible
            ImGui::TableSetupColumn("", ImGuiTableColumnFlags_None);
            ImGui::TableSetupColumn("PC", ImGuiTableColumnFlags_None);
            ImGui::TableSetupColumn("OP Code", ImGuiTableColumnFlags_None);
            ImGui::TableSetupColumn("Instruction", ImGuiTableColumnFlags_None);
            ImGui::TableHeadersRow();

            ImGuiListClipper clipper;
            clipper.Begin(instructions.size());
            while (clipper.Step())
            {
                for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; row++)
                {
                    bool isCurrentPc = instructions[row].address == m_Soc->cpu.PC;

                    ImGui::TableNextRow();
                    for (int column = 0; column < 4; column++)
                    {
                        ImGui::TableSetColumnIndex(column);

                        if (isCurrentPc) {
                            ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg1, ImGui::GetColorU32(ImVec4(0.7f, 0.3f, 0.3f, 0.65f)));
                        }

                        if (column == 0) {
                            bool isSelected = Debug::GetInstance()->breakpoints[instructions[row].address];
                            char buf[7];
                            sprintf(buf, "##%04X", instructions[row].address);
                            if (ImGui::Selectable(buf, isSelected, 0, ImVec2(0,0))) {
                                Debug::GetInstance()->breakpoints[instructions[row].address] = !isSelected;
                            }
                            if (isSelected) {
                                ImGui::SameLine();
                                ImGui::Bullet();
                            }
                        }
                        else if (column == 1) {
                            ImGui::Text("0x%04X", instructions[row].address);
                        } else if (column == 2) {
                            uint8_t opcode = instructions[row].opcode;
                            ImGui::Text("%02X", opcode);
                        } else if (column == 3) {
                            std::string s = instructions[row].mnemonic;
                            char* c = &*s.begin();
                            ImGui::Text("%s", c);
                        }

                    }
                }
            }
        }
        ImGui::EndTable(); ImGui::SameLine();
        ImGui::BeginGroup();
        if (ImGui::Button(Debug::GetInstance()->isPaused ? "Continue" : "Pause   ")) {
            Debug::GetInstance()->isPaused = !Debug::GetInstance()->isPaused;
        }
        ImGui::SameLine();
        ImGui::PushButtonRepeat(true);
        if (ImGui::ArrowButton("right", ImGuiDir_Right)) {
            m_Soc->step();
        }
        ImGui::PopButtonRepeat();

        ImGui::SetNextItemWidth(secondHalfWidth - 50);
        char pc_ca[5];
        sprintf(pc_ca, "%x", m_Soc->cpu.PC);
        ImGui::InputText("PC", pc_ca, IM_ARRAYSIZE(pc_ca));
        bool filterByHeatmap = false;
        ImGui::Checkbox("Filter by heatmap", &filterByHeatmap);

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
        ImGui::Text("%s", reg_hl); ImGui::SameLine();
        char reg_addrhl[3];
        sprintf(reg_addrhl, "%02x", m_Soc->ram.read((m_Soc->cpu.h << 8 | 0x00ff) & (m_Soc->cpu.l | 0xff00)));
        ImGui::Text("(%s)", reg_addrhl);

        ImGui::Text("SP"); ImGui::SameLine();
        char reg_sp[5];
        sprintf(reg_sp, "%04x", m_Soc->cpu.SP);
        ImGui::Text("%s", reg_sp); ImGui::SameLine();
        char reg_addrsp[3];
        sprintf(reg_addrsp, "%02x", m_Soc->ram.read(m_Soc->cpu.SP));
        ImGui::Text("(%s)", reg_addrsp);

        bool isZ = m_Soc->cpu.isFlagSet(Z);
        ImGui::Checkbox("Z", &isZ); ImGui::SameLine();

        bool isN = m_Soc->cpu.isFlagSet(N);
        ImGui::Checkbox("N", &isN); ImGui::SameLine();

        bool isH = m_Soc->cpu.isFlagSet(H);
        ImGui::Checkbox("H", &isH); ImGui::SameLine();

        bool isC = m_Soc->cpu.isFlagSet(C);
        ImGui::Checkbox("C", &isC);

        ImGui::Text("Watchlist");
        static char memory_addr_input[5] = "";
        ImGui::SetNextItemWidth(secondHalfWidth - 100);
        ImGui::InputText("##address", memory_addr_input, IM_ARRAYSIZE(memory_addr_input)); ImGui::SameLine();
        if (ImGui::Button("Add")) {
            std::stringstream ss;
            ss << std::hex << memory_addr_input;
            int addr;
            ss >> addr;
            Debug::GetInstance()->addrWatchlist.insert(Debug::GetInstance()->addrWatchlist.begin(), addr);
        }

        static ImGuiTableFlags watchlist_flags = ImGuiTableFlags_ScrollY;

        if (ImGui::BeginTable("table1", 2, watchlist_flags, ImVec2(0, 100)))
        {
            for (int row = 0; row < Debug::GetInstance()->addrWatchlist.size(); row++)
            {
                ImGui::TableNextRow();
                for (int column = 0; column < 2; column++)
                {
                    ImGui::TableSetColumnIndex(column);
                    if (column == 0)
                    {
                        std::stringstream ss;
                        ss << std::hex << Debug::GetInstance()->addrWatchlist.at(row);
                        ImGui::TextUnformatted(ss.str().c_str());
                    } else {
                        char watchlist_addr[3];
                        sprintf(watchlist_addr, "%02x", m_Soc->ram.read(Debug::GetInstance()->addrWatchlist.at(row)));
                        ImGui::Text("(%s)", watchlist_addr);
                    }
                }
            }
            ImGui::EndTable();
        }


        ImGui::EndGroup();

        // Last Instructions
        ImGui::BeginChild("InsList", ImVec2(0, 0), true, ImGuiWindowFlags_None);
        // for (int n = 0; n < 1000; n++)
        //     ImGui::Text("%d", n);
        for (auto const& i : m_Soc->cpu.lastInstructions) {
           ImGui::Text("PC %04X Opcode 0x%02X A:%02x B:%02x C:%02x D:%02x E:%02x F:%02x H:%02x L:%02x SP:%04x", 
           i.PC, i.opcode, i.a, i.b, i.c, i.d, i.e, i.f, i.h, i.l, i.SP);
        }

        ImGui::EndChild();

    ImGui::EndGroup();


    ImGui::End();

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

void DebugWindow::createInstructionsList()
{
    instructions = std::vector<Opcode>();

    const int memoryMapWhereROMIsLocated = 0xFFFF;
    for (int n = 0; n < memoryMapWhereROMIsLocated;)
    {
        Opcode instruction;
        switch (m_Soc->ram.read(n))
        {
            #include "../unprefixed.h"

            case 0xCB: {
                uint8_t nextOpcode = m_Soc->ram.read(n+1);
                switch (nextOpcode)
                {
                    #include "../cb_prefixed.h"
                }
            }
                break;
            default:
                instruction = Opcode("Illegal", m_Soc->ram.read(n), 1, n);
                break;
        }

        instructions.push_back(instruction);

        n += instruction.bytes;
    }
}