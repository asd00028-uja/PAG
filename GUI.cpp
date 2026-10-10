//
// Created by azured on 26/9/26.
//

#include "GUI.h"

#include <stdexcept>

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
            listeners[i]->wakeUp(true, WindowType::Background, colorFondo);
        }
    }

    /* Pide a los listeners los datos actuales para mostrarlos en la interfaz */
    void GUI::pedirDatos() {
        for (size_t i = 0; i < listeners.size(); i++) {
            listeners[i]->wakeUp(false, WindowType::Background, colorFondo);
        }
    }

    /* Pide a los listeners que muevan la cámara con el movimiento seleccionado */
    void GUI::moverCamara(float a, float b) {
        for (size_t i = 0; i < listeners.size(); i++) {
            listeners[i]->wakeUp(true, WindowType::Camera, static_cast<int>(tipoMovimiento), a, b);
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
            pedirDatos();
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

        ImGui::SetNextWindowPos(ImVec2(10, 300), ImGuiCond_Once);
        if (ImGui::Begin("Shader Program")) {
            ImGui::InputText("##nombre", nombreShader, sizeof(nombreShader),
                             ImGuiInputTextFlags_AutoSelectAll);
            ImGui::SameLine();
            if (ImGui::Button("Cargar")) {
                for (size_t i = 0; i < listeners.size(); i++) {
                    try {
                        listeners[i]->wakeUp(true, WindowType::ShaderProgram, nombreShader);
                        anadirMensaje(std::string("Shader program cargado: ") + nombreShader);
                    } catch (const std::exception& e) {
                        anadirMensaje(e.what());
                    }
                }
            }
        }

        ImGui::End();

        ImGui::SetNextWindowPos(ImVec2(10, 400), ImGuiCond_Once);
        if (ImGui::Begin("Camera")) {
            const char* movimientos[] = { "Zoom", "Pan", "Tilt", "Dolly", "Crane", "Orbit" };
            int actual = static_cast<int>(tipoMovimiento);
            ImGui::Text("Movement");
            if (ImGui::Combo("##movimiento", &actual, movimientos, IM_ARRAYSIZE(movimientos))) {
                tipoMovimiento = static_cast<TipoMovimiento>(actual);
            }

            const float grados = 5.0f; // Grados que se mueve
            const float distancia = 0.1f;
            switch (tipoMovimiento) {
                case TipoMovimiento::Zoom: {
                    // Pedimos el ángulo actual (el ratón también puede cambiarlo)
                    for (size_t i = 0; i < listeners.size(); i++) {
                        listeners[i]->wakeUp(false, WindowType::Camera, &anguloZoom);
                    }
                    float anterior = anguloZoom;
                    if (ImGui::SliderFloat("Angle", &anguloZoom, 10.0f, 120.0f)) {
                        moverCamara(anguloZoom - anterior);
                    }
                    break;
                }
                case TipoMovimiento::Pan:
                    ImGui::Text("Direction");
                    if (ImGui::Button("<- Left")) moverCamara(-grados);
                    ImGui::SameLine();
                    if (ImGui::Button("Right ->")) moverCamara(grados);
                    break;
                case TipoMovimiento::Tilt:
                    ImGui::Text("Direction");
                    if (ImGui::Button("^ Up ^")) moverCamara(grados);
                    ImGui::SameLine();
                    if (ImGui::Button("v Down v")) moverCamara(-grados);
                    break;
                case TipoMovimiento::Dolly:
                    // Ejes de la escena: hacia el triángulo es -Z
                    ImGui::Text("Direction");
                    if (ImGui::Button("^ Forward ^")) moverCamara(0.0f, -distancia);
                    if (ImGui::Button("<- Left")) moverCamara(-distancia, 0.0f);
                    ImGui::SameLine();
                    if (ImGui::Button("Right ->")) moverCamara(distancia, 0.0f);
                    if (ImGui::Button("v Back v")) moverCamara(0.0f, distancia);
                    break;
                case TipoMovimiento::Crane:
                    ImGui::Text("Direction");
                    if (ImGui::Button("^ Up ^")) moverCamara(distancia);
                    ImGui::SameLine();
                    if (ImGui::Button("v Down v")) moverCamara(-distancia);
                    break;
                case TipoMovimiento::Orbit:
                    ImGui::Text("Latitude");
                    if (ImGui::Button("^ North ^")) moverCamara(0.0f, grados);
                    ImGui::SameLine();
                    if (ImGui::Button("v South v")) moverCamara(0.0f, -grados);
                    ImGui::Text("Longitude");
                    if (ImGui::Button("<- West")) moverCamara(-grados, 0.0f);
                    ImGui::SameLine();
                    if (ImGui::Button("East ->")) moverCamara(grados, 0.0f);
                    break;
            }
        }
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

}
