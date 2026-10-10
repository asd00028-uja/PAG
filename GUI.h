//
// Created by azured on 26/9/26.
//

#ifndef PR02_GUI_H
#define PR02_GUI_H


#include <vector>
#include <string>
#include <GLFW/glfw3.h>
#include "Listener.h"
#include "Camara.h"

namespace PAG {

    class GUI {
    private:
        static GUI* instancia;

        std::vector<Listener*> listeners;
        char nombreShader[128] = "";

        float colorFondo[3];
        std::vector<std::string> mensajes;

        TipoMovimiento tipoMovimiento = TipoMovimiento::Zoom;
        float anguloZoom = 40.0f;

        GUI();
        void moverCamara(float a, float b = 0.0f);

    public:
        virtual ~GUI();
        static GUI& getInstancia();
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
