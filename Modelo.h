//
// Created by alvaro on 21/10/26.
//

#ifndef PR06_MODELO_H
#define PR06_MODELO_H

#include <string>
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <assimp/scene.h>
#include "ShaderProgram.h"
#include "Material.h"

namespace PAG {

    enum class TipoTransformacion { Translate, RotateX, RotateY, RotateZ, Scale };

    enum class ModoVisualizacion { Wireframe, Solid };

    class Modelo {
        std::string nombre; // Nombre del fichero del que se ha cargado
        ShaderProgram* shaderProgram = nullptr; // Shader program con el que se dibuja, no es suyo
        std::string nombreShader; // Nombre con el que se cargó ese shader program
        Material* material = nullptr; // Material con el que se dibuja
        std::string nombreMaterial; // Nombre con el que está guardado ese material
        ModoVisualizacion modo = ModoVisualizacion::Wireframe;
        std::vector<GLfloat> vertices; // Entrelazado: posición (3) y normal (3)
        std::vector<GLuint> indices;

        GLuint idVAO = 0; // Identificador del vertex array object
        GLuint idVBO = 0; // Identificador del vertex buffer object
        GLuint idIBO = 0; // Identificador del index buffer object

        glm::mat4 transformacion = glm::mat4(1.0f); // Transformación de modelado

        void procesarNodo(const aiNode* nodo, const aiScene* escena);
        void procesarMalla(const aiMesh* malla);
        void creaVAO();

    public:
        Modelo(const std::string& ruta);
        ~Modelo();
        Modelo(const Modelo&) = delete;
        Modelo& operator=(const Modelo&) = delete;

        void dibujar() const;
        const std::string& getNombre() const { return nombre; }
        const glm::mat4& getTransformacion() const { return transformacion; }

        void setShaderProgram(ShaderProgram* programa, const std::string& nombre);
        ShaderProgram* getShaderProgram() const { return shaderProgram; }
        const std::string& getNombreShader() const { return nombreShader; }

        void setMaterial(Material* nuevoMaterial, const std::string& nombre);
        Material* getMaterial() const { return material; }
        const std::string& getNombreMaterial() const { return nombreMaterial; }

        void setModo(ModoVisualizacion nuevoModo) { modo = nuevoModo; }
        ModoVisualizacion getModo() const { return modo; }

        void trasladar(const glm::vec3& desplazamiento);
        void rotar(float grados, const glm::vec3& eje);
        void escalar(const glm::vec3& factores);
    };

}

#endif //PR06_MODELO_H
