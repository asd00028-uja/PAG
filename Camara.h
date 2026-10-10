//
// Created by alvaro on 10/10/26.
//

#ifndef PR05_CAMARA_H
#define PR05_CAMARA_H

#include <glm/glm.hpp>

namespace PAG {

    enum class TipoMovimiento { Zoom, Pan, Tilt, Dolly, Crane, Orbit };

    class Camara {
        glm::vec3 posicion {0.0f, 0.0f, 3.0f};
        glm::vec3 puntoMira {0.0f, 0.0f, 0.0f};
        glm::vec3 arriba {0.0f, 1.0f, 0.0f};
        float fovX = 40.0f; // Ángulo de visión horizontal, en grados
        float zNear = 0.1f;
        float zFar = 100.0f;
        int ancho = 1024;
        int alto = 576;

        void calcularUVN(glm::vec3& u, glm::vec3& v, glm::vec3& n) const;
        static glm::vec3 rotar(const glm::vec3& dir, float grados, const glm::vec3& eje);

    public:
        glm::mat4 getVision() const;
        glm::mat4 getProyeccion() const;
        void setTamano(int ancho, int alto);
        float getFovX() const { return fovX; }

        void zoom(float grados);
        void pan(float grados);                    // positivo = derecha
        void tilt(float grados);                   // positivo = arriba
        void dolly(float dx, float dz);            // en los ejes X y Z de la escena
        void crane(float dy);                      // positivo = arriba
        void orbit(float longitud, float latitud); // positivos = este / norte
    };

}

#endif //PR05_CAMARA_H
