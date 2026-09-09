# Solución al problema de reflexión

---

Según la [documentación de glfw](https://www.glfw.org/docs/3.3/window_guide.html), cada ventana tiene un puntero asociado (por defecto `NULL`) que puede modificarse mediante los metodos `glfwSetWindowUserPointer` y se puede recuperar con `glfwGetWindowUserPointer`

De esta manera, cuando se inicializa la ventana, podemos asignarle un objeto PAG::Renderer, y luego dentro del callback `window_refresh`, se puede recuperar con el `get` y podemos llamar a la función `refrescarVentana()`