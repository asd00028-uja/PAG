//
// Created by alvaro on 21/9/26.
//

#ifndef PR01_RENDERER_H
#define PR01_RENDERER_H

#include <string>
#include <vector>
#include "Listener.h"
#include "ShaderProgram.h"
#include "Camara.h"
#include "Modelo.h"

// Dentro de el espacio de nombres PAG
namespace PAG {

    class Renderer : public Listener {

    private:
        static Renderer* instancia; // Instancia, patrón singleton
        float colorFondo[4];

        ShaderProgram* shaderProgram = nullptr;
        Camara camara;
        std::vector<Modelo*> modelos; // Modelos de la escena, el renderer es su propietario

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

        void moverCamara(TipoMovimiento tipo, float a, float b = 0.0f);
        void creaShaderProgram(const std::string& nombre);
        void cargaModelo(const std::string& ruta);
    };

}




#endif //PR01_RENDERER_H
