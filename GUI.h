//
// Created by azured on 26/9/26.
//

#ifndef PR02_GUI_H
#define PR02_GUI_H


#include <vector>
#include <string>
#include <GLFW/glfw3.h>
#include "Listener.h"

namespace PAG {

    class GUI {
    private:
        static GUI* instancia;

        std::vector<Listener*> listeners;

        float colorFondo[3];
        std::vector<std::string> mensajes;

        GUI();

    public:
        virtual ~GUI();
        static GUI& getInstancia();
        void inicializar(GLFWwindow* window);
        void render();
        void finalizar();

        void addListener(Listener* listener);
        void warnListeners();

        void anadirMensaje(const std::string& mensaje);
    };

}

#endif //PR02_GUI_H
