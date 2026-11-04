//
// Created by alvaro on 28/10/26.
//

#ifndef PR07_MATERIAL_H
#define PR07_MATERIAL_H

#include <glm/glm.hpp>

namespace PAG {

    /**
     * Material de un modelo: colores ambiente, difuso y especular, y exponente de brillo
     */
    class Material {
        glm::vec3 ambiente;
        glm::vec3 difuso;
        glm::vec3 especular;
        float brillo;

    public:
        Material();
        Material(const glm::vec3& ambiente, const glm::vec3& difuso,
                 const glm::vec3& especular, float brillo);

        const glm::vec3& getAmbiente() const { return ambiente; }
        const glm::vec3& getDifuso() const { return difuso; }
        const glm::vec3& getEspecular() const { return especular; }
        float getBrillo() const { return brillo; }

        void setAmbiente(const glm::vec3& valor) { ambiente = valor; }
        void setDifuso(const glm::vec3& valor) { difuso = valor; }
        void setEspecular(const glm::vec3& valor) { especular = valor; }
        void setBrillo(float valor) { brillo = valor; }
    };

}

#endif //PR07_MATERIAL_H
