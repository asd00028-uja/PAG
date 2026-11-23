//
// Created by alvaro on 23/11/26.
//

#ifndef PR08_LUZ_H
#define PR08_LUZ_H

#include <glm/glm.hpp>

namespace PAG {

    /**
     * Tipos de luz vistos en teoría
     */
    enum class TipoLuz {
        Ambiente,
        Puntual,
        Direccional,
        Foco
    };

    /**
     * Fuente luminosa. Guarda los datos de todos los tipos de luz, así al cambiar
     * de tipo no se pierden los valores que ya tenía (por ejemplo, la posición al
     * pasar de puntual a direccional y volver a puntual)
     */
    class Luz {
        TipoLuz tipo;
        bool encendida = true;

        glm::vec3 ia;        // Color de la luz ambiente
        glm::vec3 id;        // Color para la reflexión difusa
        glm::vec3 is;        // Color para la reflexión especular
        glm::vec3 posicion;  // Posición (puntual y foco), en coordenadas de mundo
        glm::vec3 direccion; // Dirección desde la luz hacia la escena (direccional y foco)
        float gamma;         // Ángulo de apertura del foco, en grados (0 <= gamma <= 90)
        float s;             // Exponente para los bordes suaves del foco

    public:
        Luz(TipoLuz tipo);

        TipoLuz getTipo() const { return tipo; }
        bool estaEncendida() const { return encendida; }
        const glm::vec3& getIa() const { return ia; }
        const glm::vec3& getId() const { return id; }
        const glm::vec3& getIs() const { return is; }
        const glm::vec3& getPosicion() const { return posicion; }
        const glm::vec3& getDireccion() const { return direccion; }
        float getGamma() const { return gamma; }
        float getS() const { return s; }

        void setTipo(TipoLuz valor) { tipo = valor; }
        void encenderApagar() { encendida = !encendida; }
        void setIa(const glm::vec3& valor) { ia = valor; }
        void setId(const glm::vec3& valor) { id = valor; }
        void setIs(const glm::vec3& valor) { is = valor; }
        void setPosicion(const glm::vec3& valor) { posicion = valor; }
        void setDireccion(const glm::vec3& valor) { direccion = valor; }
        void setGamma(float valor) { gamma = valor; }
        void setS(float valor) { s = valor; }
    };

}

#endif //PR08_LUZ_H
