//
// Created by alvaro on 10/10/26.
//

#include "Camara.h"

#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/epsilon.hpp>

namespace PAG {

    /**
     * Rota una dirección un ángulo (en grados) alrededor de un eje
     */
    glm::vec3 Camara::rotar(const glm::vec3& dir, float grados, const glm::vec3& eje) {
        glm::mat4 r = glm::rotate(glm::mat4(1.0f), glm::radians(grados), eje);
        return glm::vec3(r * glm::vec4(dir, 0.0f));
    }

    /**
     * Matriz de visión: pasa de coordenadas de la escena a coordenadas de la cámara
     */
    glm::mat4 Camara::getVision() const {
        glm::vec3 u, v, n;
        calcularUVN(u, v, n);
        // Se usa v como "up": el eje Y no sirve si la cámara mira en vertical
        return glm::lookAt(posicion, puntoMira, v);
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
        const glm::vec3 ejeZ(0.0f, 0.0f, 1.0f);
        const float umbral = 0.001f;

        n = glm::normalize(posicion - puntoMira);
        // Si n e Y son colineales, Y x n = (0,0,0): se usa Z en su lugar
        if (glm::all(glm::epsilonEqual(n, -arriba, umbral))) {
            u = glm::normalize(glm::cross(-ejeZ, n));
        } else if (glm::all(glm::epsilonEqual(n, arriba, umbral))) {
            u = glm::normalize(glm::cross(ejeZ, n));
        } else {
            u = glm::normalize(glm::cross(arriba, n));
        }
        v = glm::cross(n, u);
    }

    /**
     * Cambia el ángulo de visión, sin mover la cámara
     */
    void Camara::zoom(float grados) {
        fovX = glm::clamp(fovX + grados, 10.0f, 120.0f);
    }

    /**
     * Gira el punto al que se mira alrededor del eje v de la cámara
     */
    void Camara::pan(float grados) {
        glm::vec3 u, v, n;
        calcularUVN(u, v, n);
        puntoMira = posicion + rotar(puntoMira - posicion, -grados, v);
    }

    /**
     * Gira el punto al que se mira alrededor del eje u de la cámara
     */
    void Camara::tilt(float grados) {
        glm::vec3 u, v, n;
        calcularUVN(u, v, n);
        puntoMira = posicion + rotar(puntoMira - posicion, grados, u);
    }

    /**
     * Traslada la cámara y el punto al que mira en el plano XZ de la escena
     */
    void Camara::dolly(float dx, float dz) {
        glm::vec3 t(dx, 0.0f, dz);
        posicion += t;
        puntoMira += t;
    }

    /**
     * Traslada la cámara y el punto al que mira en el eje Y
     */
    void Camara::crane(float dy) {
        glm::vec3 t = arriba * dy;
        posicion += t;
        puntoMira += t;
    }

    /**
     * Gira la cámara en latitud y longitud alrededor del punto al que mira
     */
    void Camara::orbit(float longitud, float latitud) {
        glm::vec3 u, v, n;
        calcularUVN(u, v, n);
        glm::vec3 desdeCentro = rotar(posicion - puntoMira, -latitud, u);
        posicion = puntoMira + rotar(desdeCentro, longitud, arriba);
    }

}
