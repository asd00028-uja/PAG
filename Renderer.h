//
// Created by alvaro on 21/9/26.
//

#ifndef PR01_RENDERER_H
#define PR01_RENDERER_H

#include "Listener.h"

// Dentro de el espacio de nombres PAG
namespace PAG {

    class Renderer : public Listener {

    private:
        static Renderer* instancia; // Instancia, patrón singleton
        float colorFondo[4];

        Renderer();

    public:
        virtual ~Renderer();
        static Renderer& getInstancia();
        void refrescar();
        void inicializar();
        void cambiarTamano(int ancho, int alto);
        void setColorFondo(float r, float g, float b, float a = 1.0f);

        void wakeUp(WindowType t, ...) override;
    };

}




#endif //PR01_RENDERER_H
