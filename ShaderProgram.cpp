//
// Created by alvaro on 5/10/26.
//

#include "ShaderProgram.h"
#include "Shader.h"

#include <stdexcept>

namespace PAG {

    ShaderProgram::ShaderProgram(const std::string& nombre) {
        Shader vs(GL_VERTEX_SHADER, nombre + "-vs.glsl");
        Shader fs(GL_FRAGMENT_SHADER, nombre + "-fs.glsl");

        id = glCreateProgram();
        if (id == 0) {
            throw std::runtime_error("Cannot create shader program.");
        }

        glAttachShader(id, vs.getId());
        glAttachShader(id, fs.getId());
        glLinkProgram(id);

        GLint linkSuccess = GL_FALSE;
        glGetProgramiv(id, GL_LINK_STATUS, &linkSuccess);
        if (linkSuccess == GL_FALSE) {
            GLint logLen = 0;
            std::string logString = "";
            glGetProgramiv(id, GL_INFO_LOG_LENGTH, &logLen);
            if (logLen > 0) {
                char * cLogString = new char[logLen];
                GLint written = 0;
                glGetProgramInfoLog(id, logLen, &written, cLogString);
                logString.assign(cLogString);
                delete[] cLogString;
            }
            glDeleteProgram(id);
            id = 0;
            throw std::runtime_error("Cannot link shader:\n" + logString);
        }
    }

    ShaderProgram::~ShaderProgram() {
        if (id != 0) {
            glDeleteProgram(id);
        }
    }

    void ShaderProgram::usar() const {
        glUseProgram(id);
    }

}