//
// Created by alvaro on 5/10/26.
//

#ifndef PR04_SHADER_H
#define PR04_SHADER_H
#include <string>

#include "glad/glad.h"

namespace PAG {
    class Shader {
        GLuint id = 0;
        static std::string leerFichero(const std::string &fichero);

    public:
        Shader(GLenum tipo, const std::string& fichero);
        ~Shader();
        Shader(const Shader&) = delete;
        Shader& operator=(const Shader&) = delete;

        GLuint getId() const { return id; }
    };
}


#endif //PR04_SHADER_H
