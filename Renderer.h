//
// Created by alvaro on 21/9/26.
//

#ifndef PR01_RENDERER_H
#define PR01_RENDERER_H

// Dentro de el espacio de nombres PAG
namespace PAG {

    class Renderer {

    private:
        static Renderer* instancia; // Instancia, patrón singleton
        Renderer();

    public:
        virtual ~Renderer();
        static Renderer& getInstancia();
        void refrescar();
        void inicializar();
        void cambiarTamano(int ancho, int alto);
        void setColorFondo(float r, float g, float b, float a = 1.0f);
    };

}




#endif //PR01_RENDERER_H
