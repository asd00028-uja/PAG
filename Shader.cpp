//
// Created by alvaro on 5/10/26.
//

#include "Shader.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

namespace PAG {
    std::string Shader::leerFichero(const std::string& fichero) {
        std::ifstream shaderSourceFile;
        shaderSourceFile.open(fichero);
        if (!shaderSourceFile) {
            throw std::runtime_error("Cannot open shader source file: " + fichero);
        }
        std::stringstream shaderSourceStream;
        shaderSourceStream << shaderSourceFile.rdbuf();
        std::string miVertexShader = shaderSourceStream.str();
        shaderSourceFile.close();

        return miVertexShader;
    }

    Shader::Shader(GLenum tipo, const std::string& ruta) {
        std::string fuente = leerFichero(ruta);

        id = glCreateShader(tipo);
        if (id == 0) {
            throw std::runtime_error("Cannot create shader object.");
        }

        const GLchar* codigo = fuente.c_str();
        glShaderSource(id, 1, &codigo, nullptr);
        glCompileShader(id);

        GLint compileResult = GL_FALSE;
        glGetShaderiv(id, GL_COMPILE_STATUS, &compileResult);
        if (compileResult == GL_FALSE) {
            GLint logLen = 0;
            std::string logString = "";
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLen);
            if (logLen > 0) {
                char * cLogString = new char[logLen];
                GLint written = 0;
                glGetShaderInfoLog(id, logLen, &written, cLogString);
                logString.assign(cLogString);
                delete[] cLogString;
            }
            glDeleteShader(id);
            id = 0;
            throw std::runtime_error("Cannot compile shader " + ruta + ":\n" + logString);
        }
    }

    Shader::~Shader() {
        if (id != 0) {
            glDeleteShader(id);
        }
    }
}
