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

    /* Destruye la instancia de la GUI */
    void GUI::liberarInstancia() {
        delete instancia;
        instancia = nullptr;
    }

    /* Inicializa Dear ImGui */
    void GUI::inicializar(GLFWwindow* window) {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init();

        navegador.SetTitle("Cargar OBJ");
        navegador.SetTypeFilters({ ".obj" });

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
        if (ImGui::Begin("Color de Fondo", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::SetWindowFontScale(1.0f);
            pedirDatos();
            if (ImGui::ColorEdit3("Seleccionar color", colorFondo)) {
                warnListeners();
            }
        }
        ImGui::End();

        ImGui::SetNextWindowPos(ImVec2(10, 100), ImGuiCond_Once);
        if (ImGui::Begin("Mensajes", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::SetWindowFontScale(1.0f);
            for (size_t i = 0; i < mensajes.size(); i++) {
                ImGui::TextUnformatted(mensajes[i].c_str());
            }
        }
        ImGui::End();

        ImGui::SetNextWindowPos(ImVec2(10, 240), ImGuiCond_Once);
        if (ImGui::Begin("File loading", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            if (ImGui::Button("Load OBJ...")) {
                navegador.Open();
            }
            if (!nombreModelo.empty()) {
                ImGui::SameLine();
                ImGui::TextUnformatted(nombreModelo.c_str());
            }
            ImGui::Text("Shader program");
            ImGui::InputText("##nombre", nombreShader, sizeof(nombreShader),
                             ImGuiInputTextFlags_AutoSelectAll);
            if (ImGui::Button("Load") && !rutaModelo.empty()) {
                // La interfaz manda el OBJ elegido y el shader program escrito
                for (size_t i = 0; i < listeners.size(); i++) {
                    try {
                        listeners[i]->wakeUp(true, WindowType::FileLoading,
                                             rutaModelo.c_str(), nombreShader);
                        anadirMensaje("Modelo cargado: " + rutaModelo);
                    } catch (const std::exception& e) {
                        anadirMensaje(e.what());
                    }
                }
            }

            ImGui::Separator();
            ImGui::Text("Models");
            for (size_t i = 0; i < listeners.size(); i++) {
                listeners[i]->wakeUp(false, WindowType::Models, &nombresModelos);
            }
            for (size_t i = 0; i < nombresModelos.size(); i++) {
                ImGui::PushID(static_cast<int>(i));
                if (ImGui::Selectable(nombresModelos[i].c_str(), modeloSeleccionado == static_cast<int>(i))) {
                    modeloSeleccionado = static_cast<int>(i);
                }
                ImGui::PopID();
            }
            if (ImGui::Button("Delete") && modeloSeleccionado >= 0) {
                for (size_t i = 0; i < listeners.size(); i++) {
                    listeners[i]->wakeUp(true, WindowType::Models, modeloSeleccionado);
                }
                modeloSeleccionado = -1;
            }
        }
        ImGui::End();

        navegador.Display();
        if (navegador.HasSelected()) {
            rutaModelo = navegador.GetSelected().string();
            nombreModelo = navegador.GetSelected().filename().string();
            navegador.ClearSelected();
        }

        ImGui::SetNextWindowPos(ImVec2(750, 10), ImGuiCond_Once);
        if (ImGui::Begin("Transformation", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            for (size_t i = 0; i < listeners.size(); i++) {
                listeners[i]->wakeUp(false, WindowType::Models, &nombresModelos);
            }
            if (nombresModelos.empty()) {
                ImGui::Text("No models loaded");
            } else {
                if (objetoTransformado >= static_cast<int>(nombresModelos.size())) {
                    objetoTransformado = 0;
                }
                ImGui::Text("Select object");
                if (ImGui::BeginCombo("##objeto", nombresModelos[objetoTransformado].c_str())) {
                    for (size_t i = 0; i < nombresModelos.size(); i++) {
                        ImGui::PushID(static_cast<int>(i));
                        if (ImGui::Selectable(nombresModelos[i].c_str(), objetoTransformado == static_cast<int>(i))) {
                            objetoTransformado = static_cast<int>(i);
                        }
                        ImGui::PopID();
                    }
                    ImGui::EndCombo();
                }

                const char* transformaciones[] = { "Translate", "RotateX", "RotateY", "RotateZ", "Scale" };
                int actual = static_cast<int>(tipoTransformacion);
                ImGui::Text("Select transformation");
                if (ImGui::Combo("##transformacion", &actual, transformaciones, IM_ARRAYSIZE(transformaciones))) {
                    tipoTransformacion = static_cast<TipoTransformacion>(actual);
                    float inicial = (tipoTransformacion == TipoTransformacion::Scale) ? 1.0f : 0.0f;
                    valoresTransformacion[0] = inicial;
                    valoresTransformacion[1] = inicial;
                    valoresTransformacion[2] = inicial;
                }

                if (tipoTransformacion == TipoTransformacion::Translate ||
                    tipoTransformacion == TipoTransformacion::Scale) {
                    ImGui::DragFloat3("##valores", valoresTransformacion, 0.05f);
                } else {
                    ImGui::DragFloat("Degrees", &valoresTransformacion[0], 0.5f);
                }

                if (ImGui::Button("Apply")) {
                    for (size_t i = 0; i < listeners.size(); i++) {
                        listeners[i]->wakeUp(true, WindowType::Transformation, objetoTransformado,
                                             static_cast<int>(tipoTransformacion),
                                             valoresTransformacion[0], valoresTransformacion[1],
                                             valoresTransformacion[2]);
                    }
                }
            }
        }
        ImGui::End();

        ImGui::SetNextWindowPos(ImVec2(750, 260), ImGuiCond_Once);
        if (ImGui::Begin("Visualization", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            for (size_t i = 0; i < listeners.size(); i++) {
                listeners[i]->wakeUp(false, WindowType::Models, &nombresModelos);
            }
            if (nombresModelos.empty()) {
                ImGui::Text("No models loaded");
            } else {
                if (objetoVisualizado >= static_cast<int>(nombresModelos.size())) {
                    objetoVisualizado = 0;
                }
                ImGui::Text("Select object");
                if (ImGui::BeginCombo("##objetoVisualizado", nombresModelos[objetoVisualizado].c_str())) {
                    for (size_t i = 0; i < nombresModelos.size(); i++) {
                        ImGui::PushID(static_cast<int>(i));
                        if (ImGui::Selectable(nombresModelos[i].c_str(), objetoVisualizado == static_cast<int>(i))) {
                            objetoVisualizado = static_cast<int>(i);
                        }
                        ImGui::PopID();
                    }
                    ImGui::EndCombo();
                }

                // Pedimos el modo actual del modelo para mostrarlo en el desplegable
                int modo = 0;
                for (size_t i = 0; i < listeners.size(); i++) {
                    listeners[i]->wakeUp(false, WindowType::Visualization, objetoVisualizado, &modo);
                }
                const char* modos[] = { "Wireframe", "Solid" }; // Mismo orden que ModoVisualizacion
                ImGui::Text("Visualization mode");
                if (ImGui::Combo("##modo", &modo, modos, IM_ARRAYSIZE(modos))) {
                    for (size_t i = 0; i < listeners.size(); i++) {
                        listeners[i]->wakeUp(true, WindowType::Visualization, objetoVisualizado, modo);
                    }
                }
            }
        }
        ImGui::End();

        ImGui::SetNextWindowPos(ImVec2(750, 400), ImGuiCond_Once);
        if (ImGui::Begin("Camera", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
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
