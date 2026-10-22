//
// Created by alvaro on 21/9/26.
//
#include <glad/glad.h>
#include <GL/gl.h>
#include "Renderer.h"

#include <algorithm>
#include <cstdarg>
#include <filesystem>
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

        // Delete de los punteros de los shader programs (.second es el puntero)
        for ( auto& par : shaderPrograms ) {
            delete par.second;
        }
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
     * Método que destruye la instancia del Renderer, liberando los modelos y
     * los shader programs. Hay que llamarlo con el contexto OpenGL todavía activo
     */
    void Renderer::liberarInstancia() {
        delete instancia;
        instancia = nullptr;
    }

    /**
     * Método para refrescar
     */
    void Renderer::refrescar () {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glPolygonMode ( GL_FRONT_AND_BACK, GL_LINE );
        glm::mat4 proyeccionVision = camara.getProyeccion() * camara.getVision();
        for ( Modelo* modelo : modelos ) {
            ShaderProgram* programa = modelo->getShaderProgram();
            programa->usar();
            programa->setUniform("mModelViewProj",
                                 proyeccionVision * modelo->getTransformacion());
            modelo->dibujar();
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
            case WindowType::FileLoading: {
                std::va_list args;
                va_start(args, t);
                const char* ruta = va_arg(args, const char*);
                const char* nombreShader = va_arg(args, const char*);
                va_end(args);
                if (enviar) {
                    // La interfaz manda el fichero obj y el nombre base de los shaders
                    cargaModelo(ruta, nombreShader);
                }
                break;
            }
            case WindowType::Models: {
                std::va_list args;
                va_start(args, t);
                if (enviar) {
                    // La interfaz nos manda el índice del modelo a eliminar
                    eliminaModelo(va_arg(args, int));
                } else {
                    // La interfaz nos pide la lista de modelos, con el formato modelo/shader
                    std::vector<std::string>* nombres = va_arg(args, std::vector<std::string>*);
                    nombres->clear();
                    for ( Modelo* modelo : modelos ) {
                        nombres->push_back(modelo->getNombre() + "/" +
                                std::filesystem::path(modelo->getNombreShader()).filename().string());
                    }
                }
                va_end(args);
                break;
            }
            case WindowType::Transformation: {
                std::va_list args;
                va_start(args, t);
                if (enviar) {
                    // La interfaz manda el modelo, la transformación y sus valores
                    int indice = va_arg(args, int);
                    TipoTransformacion tipo = static_cast<TipoTransformacion>(va_arg(args, int));
                    float x = static_cast<float>(va_arg(args, double));
                    float y = static_cast<float>(va_arg(args, double));
                    float z = static_cast<float>(va_arg(args, double));
                    transformaModelo(indice, tipo, x, y, z);
                }
                va_end(args);
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
     * Devuelve el shader program con ese nombre, y si todavía no está cargado
     * lo crea, compila y enlaza
     * @throws runtime_error si falla algun shader
     */
    ShaderProgram* Renderer::obtenerShaderProgram(const std::string& nombre) {
        auto encontrado = shaderPrograms.find(nombre);
        if ( encontrado != shaderPrograms.end() ) {
            return encontrado->second;
        }
        ShaderProgram* nuevo = new ShaderProgram(nombre);
        shaderPrograms[nombre] = nuevo;
        return nuevo;
    }

    /**
     * Método para cargar un modelo desde un fichero y añadirlo a la escena
     * @param nombreShader Nombre base del shader program con el que se dibujará
     * @throws runtime_error si no se puede cargar el shader program o el modelo
     */
    void Renderer::cargaModelo(const std::string& ruta, const std::string& nombreShader) {
        ShaderProgram* programa = obtenerShaderProgram(nombreShader);
        Modelo* nuevo = new Modelo(ruta);
        nuevo->setShaderProgram(programa, nombreShader);

        // Para que no aparezcan en el mismo sitio: primera casilla de la rejilla libre
        int casilla = 0;
        while ( std::find(casillas.begin(), casillas.end(), casilla) != casillas.end() ) {
            casilla++;
        }
        float x = 2.0f * (casilla % 3);
        float z = -2.0f * (casilla / 3);
        nuevo->trasladar(glm::vec3(x, 0.0f, z));

        modelos.push_back(nuevo);
        casillas.push_back(casilla);
    }

    /**
     * Método para eliminar un modelo de la escena y liberar sus buffers
     * @param indice Posición del modelo en la lista de modelos
     */
    void Renderer::eliminaModelo(int indice) {
        delete modelos[indice];
        modelos.erase(modelos.begin() + indice);
        casillas.erase(casillas.begin() + indice);
    }

    /**
     * Aplica una transformación de modelado a un modelo, en sus propios ejes
     * @param indice Posición del modelo en la lista de modelos
     * @param x, y, z Desplazamiento o factores de escala; en las rotaciones, x son los grados
     */
    void Renderer::transformaModelo(int indice, TipoTransformacion tipo, float x, float y, float z) {
        Modelo* modelo = modelos[indice];
        switch (tipo) {
            case TipoTransformacion::Translate: modelo->trasladar(glm::vec3(x, y, z)); break;
            case TipoTransformacion::RotateX:   modelo->rotar(x, glm::vec3(1.0f, 0.0f, 0.0f)); break;
            case TipoTransformacion::RotateY:   modelo->rotar(x, glm::vec3(0.0f, 1.0f, 0.0f)); break;
            case TipoTransformacion::RotateZ:   modelo->rotar(x, glm::vec3(0.0f, 0.0f, 1.0f)); break;
            case TipoTransformacion::Scale:     modelo->escalar(glm::vec3(x, y, z)); break;
        }
    }
}
