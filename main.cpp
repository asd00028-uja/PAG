#include <iostream>
#include <cstdlib>
#include <string>
// IMPORTANTE: El include de GLAD debe estar siempre ANTES de el de GLFW
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "GUI.h"
#include "Renderer.h"
#include <imgui.h>

// - Esta función callback será llamada cuando GLFW produzca algún error
void error_callback ( int errno, const char* desc )
{ std::string aux (desc);
    PAG::GUI::getInstancia().anadirMensaje("Error de GLFW número " + std::to_string(errno) + ": " + aux);
}

// - Esta función callback será llamada cada vez que el área de dibujo
// OpenGL deba ser redibujada.
void window_refresh_callback ( GLFWwindow *window ) {
    PAG::Renderer::getInstancia().refrescar();
    PAG::GUI::getInstancia().render();

    // - GLFW usa un doble buffer para que no haya parpadeo. Esta orden
    // intercambia el buffer back (que se ha estado dibujando) por el
    // que se mostraba hasta ahora front. Debe ser la última orden de
    // este callback
    glfwSwapBuffers ( window );
}

// - Esta función callback será llamada cada vez que se cambie el tamaño
// del área de dibujo OpenGL.
void framebuffer_size_callback ( GLFWwindow *window, int width, int height )
{
    PAG::Renderer::getInstancia().cambiarTamano(width, height);
    PAG::GUI::getInstancia().anadirMensaje("Resize callback called");
}
// - Esta función callback será llamada cada vez que se pulse una tecla
// dirigida al área de dibujo OpenGL.
void key_callback ( GLFWwindow *window, int key, int scancode, int action, int mods )
{ if ( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS )
{ glfwSetWindowShouldClose(window, GLFW_TRUE);
}
    PAG::GUI::getInstancia().anadirMensaje("Key callback called");
}
// - Esta función callback será llamada cada vez que se pulse algún botón
// del ratón sobre el área de dibujo OpenGL.
void mouse_button_callback ( GLFWwindow *window, int button, int action, int mods )
{ if ( action == GLFW_PRESS )
{ PAG::GUI::getInstancia().anadirMensaje("Pulsado el botón: " + std::to_string(button));
    ImGuiIO& io = ImGui::GetIO ();
    io.AddMouseButtonEvent ( button, true );
}
else if ( action == GLFW_RELEASE )
{ PAG::GUI::getInstancia().anadirMensaje("Soltado el botón: " + std::to_string(button));
    ImGuiIO& io = ImGui::GetIO ();
    io.AddMouseButtonEvent ( button, false );
}
}

float colorActual[4] = {0.6f, 0.6f, 0.6f, 1.0f};

// - Esta función callback será llamada cada vez que se mueva la rueda
// del ratón sobre el área de dibujo OpenGL.
void scroll_callback ( GLFWwindow *window, double xoffset, double yoffset )
{
    PAG::GUI::getInstancia().anadirMensaje("Movida la rueda del ratón " + std::to_string(xoffset)
              + " Unidades en horizontal y " + std::to_string(yoffset)
              + " unidades en vertical");

    // Cambiamos un canal aleatorio entre los 3 primeros (rgb)
    int canal = rand() % 3;

    if ( yoffset > 0 )
    {
        // Si se hace scroll hacia arriba se suma 0.1f, haciendo de tope en 1.0f
        if (colorActual[canal] + 0.1f >= 1.0f)
            colorActual[canal] = 1.0f;
        else colorActual[canal] += 0.1f;
    }
    else if ( yoffset < 0 )
    {
        // Si se hace scroll hacia abajo, lo mismo pero capando en 0.0f
        if (colorActual[canal] - 0.1f <= 0.0f)
            colorActual[canal] = 0.0f;
        else colorActual[canal] -= 0.1f;
    }

    PAG::Renderer::getInstancia().setColorFondo(colorActual[0], colorActual[1], colorActual[2], colorActual[3]);
}


int main()
{
    std::cout << "Starting Application PAG - Prueba 01" << std::endl;

    // - Este callback hay que registrarlo ANTES de llamar a glfwInit
    glfwSetErrorCallback ( (GLFWerrorfun) error_callback );

    // - Inicializa GLFW. Es un proceso que sólo debe realizarse una vez en la aplicación
    if ( glfwInit () != GLFW_TRUE )
    { std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    // - Definimos las características que queremos que tenga el contexto gráfico
    // OpenGL de la ventana que vamos a crear. Por ejemplo, el número de muestras o el
    // modo Core Profile.
    glfwWindowHint ( GLFW_SAMPLES, 4 ); // - Activa antialiasing con 4 muestras.
    glfwWindowHint ( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE ); // - Esta y las 2
    glfwWindowHint ( GLFW_CONTEXT_VERSION_MAJOR, 4 ); // siguientes activan un contexto
    glfwWindowHint ( GLFW_CONTEXT_VERSION_MINOR, 3 ); // OpenGL Core Profile 4.3.
    // - Definimos el puntero para guardar la dirección de la ventana de la aplicación y
    // la creamos
    GLFWwindow *window;
    // - Tamaño, título de la ventana, en ventana y no en pantalla completa,
    // sin compartir recursos con otras ventanas.
    window = glfwCreateWindow ( 1024, 576, "PAG Introduction", nullptr, nullptr );
    // - Comprobamos si la creación de la ventana ha tenido éxito.
    if ( window == nullptr )
    { std::cout << "Failed to open GLFW window" << std::endl;
        glfwTerminate (); // - Liberamos los recursos que ocupaba GLFW.
        return -2;
    }
    // - Hace que el contexto OpenGL asociado a la ventana que acabamos de crear pase a
    // ser el contexto actual de OpenGL para las siguientes llamadas a la biblioteca
    glfwMakeContextCurrent ( window );
    // - Ahora inicializamos GLAD.
    if ( !gladLoadGLLoader ( (GLADloadproc) glfwGetProcAddress ) )
    { std::cout << "GLAD initialization failed" << std::endl;
        glfwDestroyWindow ( window ); // - Liberamos los recursos que ocupaba GLFW.
        window = nullptr;
        glfwTerminate ();
        return -3;
    }

    // - Registramos los callbacks que responderán a los eventos principales
    glfwSetWindowRefreshCallback ( window, window_refresh_callback );
    glfwSetFramebufferSizeCallback ( window, framebuffer_size_callback );
    glfwSetKeyCallback ( window, key_callback );
    glfwSetMouseButtonCallback ( window, mouse_button_callback );
    glfwSetScrollCallback ( window, scroll_callback );

    PAG::Renderer::getInstancia().inicializar();
    PAG::GUI::getInstancia().inicializar(window);
    PAG::GUI::getInstancia().addListener(&PAG::Renderer::getInstancia());
    PAG::GUI::getInstancia().anadirMensaje("Starting Application PAG - Prueba 01");
    // - Ciclo de eventos de la aplicación. La condición de parada es que la
    // ventana principal deba cerrarse. Por ejemplo, si el usuario pulsa el
    // botón de cerrar la ventana (la X).
    while ( !glfwWindowShouldClose ( window ) )
    {
        PAG::Renderer::getInstancia().refrescar();
        PAG::GUI::getInstancia().render();
        glfwSwapBuffers ( window );
        glfwPollEvents ();
    }
    // - Una vez terminado el ciclo de eventos, liberar recursos, etc.
    std::cout << "Finishing application pag prueba" << std::endl;
    PAG::GUI::getInstancia().finalizar();
    glfwDestroyWindow ( window ); // - Cerramos y destruimos la ventana de la aplicación.
    window = nullptr;
    glfwTerminate (); // - Liberamos los recursos que ocupaba GLFW.
}
