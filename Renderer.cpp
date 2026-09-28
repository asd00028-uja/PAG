//
// Created by alvaro on 21/9/26.
//
#include <glad/glad.h>
#include <GL/gl.h>
#include "Renderer.h"

#include <cstdarg>
#include <iostream>
#include <string>

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

    void Renderer::inicializar() {
        glClearColor(colorFondo[0], colorFondo[1], colorFondo[2], colorFondo[3]);
        glEnable ( GL_DEPTH_TEST );
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
     * @note No se incluye ninguna comprobación de errores
     * @return 1 si éxito, 0 si no.
     */
    int Renderer::creaShaderProgram() {
        std::string miVertexShader =
        "#version 410\n"
        "layout (location = 0) in vec3 posicion;\n"
        "void main ()\n"
        "{ gl_Position = vec4 ( posicion, 1 );\n"
        "}\n";

        std::string miFragmentShader =
        "#version 410\n"
        "out vec4 colorFragmento;\n"
        "void main ()\n"
        "{ colorFragmento = vec4 ( 1.0, .4, .2, 1.0 );\n"
        "}\n";

        idVS = glCreateShader ( GL_VERTEX_SHADER );
        if (idVS == 0) {
            std::cout << "Cannot create shader object." << std::endl;
            return 0;
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
                std::cout << "Cannot compile shader " << GL_VERTEX_SHADER << std::endl;
                std::cout << logString << std::endl;
            }
            return 0;
        }

        idFS = glCreateShader ( GL_FRAGMENT_SHADER );
        if (idFS == 0) {
            std::cout << "Cannot create shader object." << std::endl;
            return 0;
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
                std::cout << "Cannot compile shader " << GL_FRAGMENT_SHADER << std::endl;
                std::cout << logString << std::endl;
            }
            return 0;
        }


        if (idSP <= 0) {
            idSP = glCreateProgram ();
            if (idSP == 0) {
                std::cout << "Cannot create shader program." << std::endl;
                return 0;
            }
        }

        glAttachShader ( idSP, idVS );
        glAttachShader ( idSP, idFS );
        glLinkProgram ( idSP );

        GLint linkSuccess = 0;
        std::string logString = "";
        glGetProgramiv(idSP, GL_LINK_STATUS, &linkSuccess);
        if (linkSuccess == GL_FALSE) {
            GLint logLen = 0;
            glGetProgramiv(idSP, GL_INFO_LOG_LENGTH, &logLen);
            if (logLen > 0) {
                char * cLogString = new char[logLen];
                GLint written = 0;
                glGetProgramInfoLog(idSP, logLen, &written, cLogString);
                logString.assign(cLogString);
                delete[] cLogString;
                std::cout << "Cannot link shader " << std::endl;
                std::cout << logString << std::endl;
            }
            return 0;
        }

        return 1;
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
