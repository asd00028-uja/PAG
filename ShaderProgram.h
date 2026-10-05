//
// Created by alvaro on 5/10/26.
//

#ifndef PR04_SHADERPROGRAM_H
#define PR04_SHADERPROGRAM_H


#include <string>
#include <glad/glad.h>

namespace PAG {

    class ShaderProgram {
        GLuint id = 0;

    public:
        ShaderProgram(const std::string& nombre);
        ~ShaderProgram();

        void usar() const;
        GLuint getId() const { return id; }
    };

}


#endif //PR04_SHADERPROGRAM_H
