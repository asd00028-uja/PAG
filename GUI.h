//
// Created by azured on 26/9/26.
//

#ifndef PR02_GUI_H
#define PR02_GUI_H


#include <vector>
#include <string>
#include "Modelo.h" // Antes que GLFW, porque incluye glad
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imfilebrowser.h"
#include "Listener.h"
#include "Camara.h"

namespace PAG {

    class GUI {
    private:
        static GUI* instancia;

        std::vector<Listener*> listeners;
        char nombreShader[128] = "../pag07";
        ImGui::FileBrowser navegador; // Ventana para elegir el fichero
        std::string rutaModelo; // Ruta del fichero OBJ elegido, pendiente de cargar
        std::string nombreModelo; // Nombre de ese fichero, para mostrarlo
        std::vector<std::string> nombresModelos; // Modelos de la escena, con el formato modelo/shader
        int modeloSeleccionado = -1; // Posición del modelo seleccionado en la lista, -1 si ninguno

        int objetoTransformado = 0; // Modelo elegido en la ventana Transformation
        TipoTransformacion tipoTransformacion = TipoTransformacion::Translate;
        float valoresTransformacion[3] = { 0.0f, 0.0f, 0.0f }; // Desplazamiento, escala o grados (el primero)

        int objetoVisualizado = 0; // Modelo elegido en la ventana Visualization

        float colorFondo[3];
        std::vector<std::string> mensajes;

        TipoMovimiento tipoMovimiento = TipoMovimiento::Orbit;
        float anguloZoom = 40.0f;

        GUI();
        void moverCamara(float a, float b = 0.0f);

    public:
        virtual ~GUI();
        static GUI& getInstancia();
        static void liberarInstancia();
        void inicializar(GLFWwindow* window);
        void render();
        void finalizar();

        void addListener(Listener* listener);
        void warnListeners();
        void pedirDatos();

        void anadirMensaje(const std::string& mensaje);

        TipoMovimiento getTipoMovimiento() const { return tipoMovimiento; }
    };

}

#endif //PR02_GUI_H
