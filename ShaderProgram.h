//
// Created by alvaro on 5/10/26.
//

#ifndef PR04_SHADERPROGRAM_H
#define PR04_SHADERPROGRAM_H


#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>

namespace PAG {

    class ShaderProgram {
        GLuint id = 0;

    public:
        ShaderProgram(const std::string& nombre);
        ~ShaderProgram();
        ShaderProgram(const ShaderProgram&) = delete;
        ShaderProgram& operator=(const ShaderProgram&) = delete;

        void usar() const;
        void setUniform(const std::string& nombre, const glm::mat4& valor) const;
        GLuint getId() const { return id; }
    };

}


#endif //PR04_SHADERPROGRAM_H
