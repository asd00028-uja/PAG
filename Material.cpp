//
// Created by alvaro on 28/10/26.
//

#include "Material.h"

namespace PAG {

    /**
     * Constructor por defecto: material gris, sin brillo especular
     */
    Material::Material() : ambiente(0.2f), difuso(0.8f), especular(0.0f), brillo(1.0f) {
    }

    Material::Material(const glm::vec3& ambiente, const glm::vec3& difuso,
                       const glm::vec3& especular, float brillo)
        : ambiente(ambiente), difuso(difuso), especular(especular), brillo(brillo) {
    }

}
