//
// Created by alvaro on 21/9/26.
//
#include <glad/glad.h>
#include <GL/gl.h>
#include "Renderer.h"

#include <cstdarg>
#include <iostream>
#include <string>
#include <stdexcept>
#include <fstream>
#include <sstream>

namespace PAG {
    // Instancia inicializada a NULL
    Renderer* Renderer::instancia = nullptr;

    // Constructor
    Renderer::Renderer() {
        colorFondo[0] = 0.6f;
        colorFondo[1] = 0.6f;
        colorFondo[2] = 0.6f;
        colorFondo[3] = 1.0f;
    }

    // Destructor
    Renderer::~Renderer() {
        for ( Modelo* modelo : modelos ) {
            delete modelo;
        }

        delete shaderProgram;
    }

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
        glPolygonMode ( GL_FRONT_AND_BACK, GL_FILL );
        if ( shaderProgram ) {
            shaderProgram->usar();
            shaderProgram->setUniform("mModelViewProj",
                                      camara.getProyeccion() * camara.getVision());
            for ( Modelo* modelo : modelos ) {
                modelo->dibujar();
            }
        }
    }

    void Renderer::inicializar() {
        glClearColor(colorFondo[0], colorFondo[1], colorFondo[2], colorFondo[3]);
        glEnable ( GL_DEPTH_TEST );
        glEnable ( GL_MULTISAMPLE );
    }

    void Renderer::cambiarTamano(int ancho, int alto) {
        glViewport(0, 0, ancho, alto);
        camara.setTamano(ancho, alto);
    }

    void Renderer::setColorFondo(float r, float g, float b, float a) {
        colorFondo[0] = r;
        colorFondo[1] = g;
        colorFondo[2] = b;
        colorFondo[3] = a;
        glClearColor(colorFondo[0], colorFondo[1], colorFondo[2], colorFondo[3]);
    }

    const float* Renderer::getColorFondo() const {
        return colorFondo;
    }

    void Renderer::wakeUp(bool enviar, WindowType t, ...) {
        switch (t) {
            case WindowType::Background: {
                std::va_list args;
                va_start(args, t);
                float* color = va_arg(args, float*);
                va_end(args);
                if (enviar) {
                    // La interfaz nos manda el color
                    setColorFondo(color[0], color[1], color[2], 1.0f);
                } else {
                    // La interfaz nos pide el color
                    color[0] = colorFondo[0];
                    color[1] = colorFondo[1];
                    color[2] = colorFondo[2];
                }
                break;
            }
            case WindowType::ShaderProgram: {
                std::va_list args;
                va_start(args, t);
                const char* nombre = va_arg(args, const char*);
                va_end(args);
                if (enviar) {
                    // La interfaz nos manda el nombre base de los shaders
                    // Si falla, la excepción llega hasta la interfaz
                    creaShaderProgram(nombre);
                }
                break;
            }
            case WindowType::Camera: {
                std::va_list args;
                va_start(args, t);
                if (enviar) {
                    TipoMovimiento tipo = static_cast<TipoMovimiento>(va_arg(args, int));
                    float a = static_cast<float>(va_arg(args, double));
                    float b = static_cast<float>(va_arg(args, double));
                    moverCamara(tipo, a, b);
                } else {
                    // La interfaz nos pide el ángulo de visión actual
                    float* fov = va_arg(args, float*);
                    *fov = camara.getFovX();
                }
                va_end(args);
                break;
            }
            default:
                break;
        }
    }

    /**
     * Aplica un movimiento a la cámara
     * @param a Primer parámetro del movimiento (ángulo, distancia o longitud)
     * @param b Segundo parámetro, solo para dolly (dz) y orbit (latitud)
     */
    void Renderer::moverCamara(TipoMovimiento tipo, float a, float b) {
        switch (tipo) {
            case TipoMovimiento::Zoom:  camara.zoom(a); break;
            case TipoMovimiento::Pan:   camara.pan(a); break;
            case TipoMovimiento::Tilt:  camara.tilt(a); break;
            case TipoMovimiento::Dolly: camara.dolly(a, b); break;
            case TipoMovimiento::Crane: camara.crane(a); break;
            case TipoMovimiento::Orbit: camara.orbit(a, b); break;
        }
    }

     /**
     * Método para crear, compilar y enlazar el shader program
     * @throws runtime_error si falla algun shader
     */
    void Renderer::creaShaderProgram(const std::string& nombre) {
        ShaderProgram* nuevo = new ShaderProgram(nombre);
        delete shaderProgram;
        shaderProgram = nuevo;
    }

    /**
     * Método para cargar un modelo desde un fichero y añadirlo a la escena
     * @throws runtime_error si no se puede cargar el modelo
     */
    void Renderer::cargaModelo(const std::string& ruta) {
        modelos.push_back(new Modelo(ruta));
    }
}
