//
// Created by alvaro on 21/9/26.
//

#ifndef PR01_RENDERER_H
#define PR01_RENDERER_H

#include <string>
#include "Listener.h"

// Dentro de el espacio de nombres PAG
namespace PAG {

    class Renderer : public Listener {

    private:
        static Renderer* instancia; // Instancia, patrón singleton
        float colorFondo[4];

        // Identificadores temporales de los shaders y objectos
        GLuint idVS = 0; // Identificador del vertex shader
        GLuint idFS = 0; // Identificador del fragment shader
        GLuint idSP = 0; // Identificador del shader program
        GLuint idVAO = 0; // Identificador del vertex array object
        GLuint idVBO = 0; // Identificador del vertex buffer object
        GLuint idVBOColor = 0; // Id para los colores
        GLuint idIBO = 0; // Identificador del index buffer object

        Renderer();

    public:
        virtual ~Renderer();
        static Renderer& getInstancia();
        void refrescar();
        void inicializar();
        void cambiarTamano(int ancho, int alto);
        void setColorFondo(float r, float g, float b, float a = 1.0f);
        const float* getColorFondo() const;

        void wakeUp(bool enviar, WindowType t, ...) override;

        void creaShaderProgram(std::string nombre);
        void creaModelo();
    };

}




#endif //PR01_RENDERER_H
