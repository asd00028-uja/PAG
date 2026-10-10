//
// Created by alvaro on 10/10/26.
//

#include "Camara.h"

#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

namespace PAG {

    /**
     * Matriz de visión: pasa de coordenadas de la escena a coordenadas de la cámara
     */
    glm::mat4 Camara::getVision() const {
        return glm::lookAt(posicion, puntoMira, arriba);
    }

    /**
     * Matriz de proyección perspectiva
     */
    glm::mat4 Camara::getProyeccion() const {
        float aspecto = static_cast<float>(ancho) / alto;
        // glm::perspective pide el ángulo vertical: lo obtenemos del horizontal
        float fovY = 2.0f * std::atan(std::tan(glm::radians(fovX) / 2.0f) / aspecto);
        return glm::perspective(fovY, aspecto, zNear, zFar);
    }

    void Camara::setTamano(int ancho, int alto) {
        if (ancho > 0 && alto > 0) {
            this->ancho = ancho;
            this->alto = alto;
        }
    }

    /**
     * Calcula el sistema de coordenadas local de la cámara
     */
    void Camara::calcularUVN(glm::vec3& u, glm::vec3& v, glm::vec3& n) const {
        n = glm::normalize(posicion - puntoMira);
        u = glm::normalize(glm::cross(arriba, n));
        v = glm::cross(n, u);
    }

}
