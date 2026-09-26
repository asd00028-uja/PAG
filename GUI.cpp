//
// Created by azured on 26/9/26.
//

#include "GUI.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace PAG {

    GUI* GUI::instancia = nullptr;

    GUI::GUI() {
        colorFondo[0] = 0.6f;
        colorFondo[1] = 0.6f;
        colorFondo[2] = 0.6f;
    }

    GUI::~GUI() {}

    GUI& GUI::getInstancia() {
        if (!instancia) {
            instancia = new GUI();
        }
        return *instancia;
    }

    /* Inicializa Dear ImGui */
    void GUI::inicializar(GLFWwindow* window) {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init();

        anadirMensaje("Dear ImGui inicializado");
    }

    /* Destruye los recursos */
    void GUI::finalizar() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void GUI::addListener(Listener* listener) {
        listeners.push_back(listener);
    }

    /* Despierta un listener */
    void GUI::warnListeners() {
        for (size_t i = 0; i < listeners.size(); i++) {
            listeners[i]->wakeUp(WindowType::Background, colorFondo);
        }
    }

    /* Añade un mensaje a la ventana de mensajes */
    void GUI::anadirMensaje(const std::string& mensaje) {
        mensajes.push_back(mensaje);
    }

    void GUI::render() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Once);
        if (ImGui::Begin("Color de Fondo")) {
            ImGui::SetWindowFontScale(1.0f);
            if (ImGui::ColorEdit3("Seleccionar color", colorFondo)) {
                warnListeners();
            }
        }
        ImGui::End();

        ImGui::SetNextWindowPos(ImVec2(10, 100), ImGuiCond_Once);
        if (ImGui::Begin("Mensajes")) {
            ImGui::SetWindowFontScale(1.0f);
            for (size_t i = 0; i < mensajes.size(); i++) {
                ImGui::TextUnformatted(mensajes[i].c_str());
            }
        }
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

}
