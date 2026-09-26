cmake_minimum_required(VERSION 3.27)
project(pr02)
set(CMAKE_CXX_STANDARD 17)

# Define variables agrupando nombres de archivo
file( GLOB MY_FILES *.cpp )
file( GLOB GLAD_FILES GLAD/src/*.c )
file( GLOB IMGUI_FILES imgui/src/*.cpp )

# Indica qué archivos hay que compilar para generar el ejecutable
add_executable( ${PROJECT_NAME} ${MY_FILES}
                                ${GLAD_FILES}
                                ${IMGUI_FILES}
        Listener.h
        GUI.cpp
        GUI.h)

# Indica directorios (adicionales a los por defecto) para buscar includes
target_include_directories( ${PROJECT_NAME} PUBLIC 
    GLAD/include
    imgui/include )

# Indica bibliotecas a enlazar en el ejecutable
target_link_libraries( ${PROJECT_NAME} GL glfw )