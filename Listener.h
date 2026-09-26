//
// Created by azured on 26/9/26.
//

#ifndef PR02_LISTENER_H
#define PR02_LISTENER_H


namespace PAG {

    enum class WindowType {
        Background,
        Messages
    };

    /**
    * @brief Patrón Obsaervador
    */
    class Listener {
    public:
        Listener() = default;
        virtual ~Listener() = default;

        virtual void wakeUp(WindowType t, ...) = 0;
    };

}


#endif //PR02_LISTENER_H
