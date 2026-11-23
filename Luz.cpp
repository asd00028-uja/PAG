//
// Created by alvaro on 23/11/26.
//

#include "Luz.h"

namespace PAG {

    /**
     * Crea una luz del tipo indicado con valores por defecto para todos los
     * atributos, aunque ese tipo no los use
     */
    Luz::Luz(TipoLuz tipo) : tipo(tipo), ia(0.2f), id(1.0f), is(1.0f),
                             posicion(0.0f, 3.0f, 3.0f), direccion(0.0f, -1.0f, 0.0f),
                             gamma(30.0f), s(0.0f) {
    }

}
