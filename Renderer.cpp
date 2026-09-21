//
// Created by alvaro on 21/9/26.
//

#include <GL/gl.h>
#include "Renderer.h"
namespace PAG {
    // Instancia inicializada a NULL
    Renderer* Renderer::instancia = nullptr;

    // Constructor
    Renderer::Renderer() {}

    // Destructor
    Renderer::~Renderer() {}

    /**
     * Método que devuelve (o crea si no existe) la instancia del Renderer
     * @return Instancia de la clase renderer
     */
    Renderer& Renderer::getInstancia() {
        if (!instancia)
            instancia = new Renderer();
        return *instancia;
    }

    /**
     * Método para refrescar
     */
    void Renderer::refrescar () {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

}