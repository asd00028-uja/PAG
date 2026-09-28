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
        if ( idVS != 0 )
        { glDeleteShader ( idVS );
        }
        if ( idFS != 0 )
        { glDeleteShader ( idFS );
        }
        if ( idSP != 0 )
        { glDeleteProgram ( idSP );
        }
        if ( idVBO != 0 )
        { glDeleteBuffers ( 1, &idVBO );
        }
        if ( idIBO != 0 )
        { glDeleteBuffers ( 1, &idIBO );
        }
        if ( idVAO != 0 )
        { glDeleteVertexArrays ( 1, &idVAO );
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
     * Método para refrescar
     */
    void Renderer::refrescar () {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glPolygonMode ( GL_FRONT_AND_BACK, GL_FILL );
        if ( idSP != 0 && idVAO != 0 ) {
            glUseProgram ( idSP );
            glBindVertexArray ( idVAO );
            glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, idIBO );
            glDrawElements ( GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr );
        }
    }

    void Renderer::inicializar() {
        glClearColor(colorFondo[0], colorFondo[1], colorFondo[2], colorFondo[3]);
        glEnable ( GL_DEPTH_TEST );
        glEnable ( GL_MULTISAMPLE );
    }

    void Renderer::cambiarTamano(int ancho, int alto) {
        glViewport(0, 0, ancho, alto);
    }

    void Renderer::setColorFondo(float r, float g, float b, float a) {
        colorFondo[0] = r;
        colorFondo[1] = g;
        colorFondo[2] = b;
        colorFondo[3] = a;
        glClearColor(colorFondo[0], colorFondo[1], colorFondo[2], colorFondo[3]);
    }

    void Renderer::wakeUp(WindowType t, ...) {
        switch (t) {
            case WindowType::Background: {
                std::va_list args;
                va_start(args, t);
                float* nuevoColor = va_arg(args, float*);
                setColorFondo(nuevoColor[0], nuevoColor[1], nuevoColor[2], 1.0f);
                va_end(args);
                break;
            }
            default:
                break;
        }
    }

     /**
     * Método para crear, compilar y enlazar el shader program
     * @throws runtime_error si falla algun shader
     */
    void Renderer::creaShaderProgram() {
        std::string miVertexShader =
        "#version 410\n"
        "layout (location = 0) in vec3 posicion;\n"
        "void main ()\n"
        // "{ gl_Position = vec4 ( posicion_error, 1 );\n"
        " { gl_Position = vec4 ( posicion, 1 );\n"
        "}\n";

        std::string miFragmentShader =
        "#version 410\n"
        "out vec4 colorFragmento;\n"
        "void main ()\n"
        "{ colorFragmento = vec4 ( 1.0, .4, .2, 1.0 );\n"
        "}\n";

        idVS = glCreateShader ( GL_VERTEX_SHADER );
        if (idVS == 0) {
            throw std::runtime_error("Cannot create vertex shader object.");
        }

        const GLchar* fuenteVS = miVertexShader.c_str ();
        glShaderSource ( idVS, 1, &fuenteVS, nullptr );
        glCompileShader ( idVS );
        GLint compileResultVS;
        glGetShaderiv ( idVS, GL_COMPILE_STATUS, &compileResultVS );
        if (compileResultVS == GL_FALSE) {
            GLint logLen = 0;
            std::string logString = "";
            glGetShaderiv(idVS, GL_INFO_LOG_LENGTH, &logLen);
            if (logLen > 0) {
                char * cLogString = new char[logLen];
                GLint written = 0;
                glGetShaderInfoLog(idVS, logLen, &written, cLogString);
                logString.assign(cLogString);
                delete[] cLogString;
            }
            throw std::runtime_error("Cannot compile shader " + std::to_string(GL_VERTEX_SHADER) + ":\n" + logString);
        }

        idFS = glCreateShader ( GL_FRAGMENT_SHADER );
        if (idFS == 0) {
            throw std::runtime_error("Cannot create fragment shader object.");
        }

        const GLchar* fuenteFS = miFragmentShader.c_str ();
        glShaderSource ( idFS, 1, &fuenteFS, nullptr );
        glCompileShader ( idFS );
        GLint compileResultFS;
        glGetShaderiv ( idFS, GL_COMPILE_STATUS, &compileResultFS );
        if (compileResultFS == GL_FALSE) {
            GLint logLen = 0;
            std::string logString = "";
            glGetShaderiv(idFS, GL_INFO_LOG_LENGTH, &logLen);
            if (logLen > 0) {
                char * cLogString = new char[logLen];
                GLint written = 0;
                glGetShaderInfoLog(idFS, logLen, &written, cLogString);
                logString.assign(cLogString);
                delete[] cLogString;
            }
            throw std::runtime_error("Cannot compile shader " + std::to_string(GL_FRAGMENT_SHADER) + ":\n" + logString);
        }


        if (idSP <= 0) {
            idSP = glCreateProgram ();
            if (idSP == 0) {
                throw std::runtime_error("Cannot create shader program.");
            }
        }

        glAttachShader ( idSP, idVS );
        glAttachShader ( idSP, idFS );
        glLinkProgram ( idSP );

        GLint linkSuccess = 0;
        glGetProgramiv(idSP, GL_LINK_STATUS, &linkSuccess);
        if (linkSuccess == GL_FALSE) {
            GLint logLen = 0;
            std::string logString = "";
            glGetProgramiv(idSP, GL_INFO_LOG_LENGTH, &logLen);
            if (logLen > 0) {
                char * cLogString = new char[logLen];
                GLint written = 0;
                glGetProgramInfoLog(idSP, logLen, &written, cLogString);
                logString.assign(cLogString);
                delete[] cLogString;
            }
            throw std::runtime_error("Cannot link shader:\n" + logString);
        }

    }

    /**
    * Método para crear el VAO para el modelo a renderizar
    * @note No se incluye ninguna comprobación de errores
    */
    void Renderer::creaModelo() {

        GLfloat vertices[] = { -.5, -.5, 0,
                                .5, -.5, 0,
                                .0, .5, 0 };

        GLuint indices[] = { 0, 1, 2 };
        glGenVertexArrays ( 1, &idVAO );
        glBindVertexArray ( idVAO );
        glGenBuffers ( 1, &idVBO );
        glBindBuffer ( GL_ARRAY_BUFFER, idVBO );
        glBufferData ( GL_ARRAY_BUFFER, 9*sizeof(GLfloat), vertices, GL_STATIC_DRAW );
        glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), nullptr );
        glEnableVertexAttribArray ( 0 );
        glGenBuffers ( 1, &idIBO );
        glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, idIBO );
        glBufferData ( GL_ELEMENT_ARRAY_BUFFER, 3*sizeof(GLuint), indices, GL_STATIC_DRAW );
    }
}
