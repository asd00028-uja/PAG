//
// Created by azured on 26/9/26.
//

#ifndef PR02_LISTENER_H
#define PR02_LISTENER_H


namespace PAG {

    enum class WindowType {
        Background,
        Messages,
        ShaderProgram,
        Camera
    };

    /**
    * @brief Patrón Obsaervador
    */
    class Listener {
    public:
        Listener() = default;
        virtual ~Listener() = default;

        /**
         * @param enviar true: la interfaz envía datos al listener.
         *               false: la interfaz pide datos al listener (este los escribe en los argumentos).
         */
        virtual void wakeUp(bool enviar, WindowType t, ...) = 0;
    };

}


#endif //PR02_LISTENER_H
