# Solución al problema de reflexión

---

Según la [documentación de glfw](https://www.glfw.org/docs/3.3/window_guide.html), cada ventana tiene un puntero asociado (por defecto `NULL`) que puede modificarse mediante los metodos `glfwSetWindowUserPointer` y se puede recuperar con `glfwGetWindowUserPointer`

De esta manera, cuando se inicializa la ventana, podemos asignarle un objeto PAG::Renderer, y luego dentro del callback `window_refresh`, se puede recuperar con el `get` y podemos llamar a la función `refrescarVentana()`

## Cambios en la práctica 2
He creado una clase Renderer que gestiona todo el rendering de la aplicacion, utilizando el patrón singleton, para que solo pueda haber una instancia durante toda la aplicación
Se ha creado una clase GUI que gestiona las interfaces como la de selección de color (usando ImGui), esta tambien sigue el patrón singleton

También se ha creado una interfaz Listener para implementar el patrón observador, que se usa en Renderer para que la clase GUI avise cuando se cambia el color.