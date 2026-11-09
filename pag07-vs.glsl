#version 410
layout (location = 0) in vec3 posicion;
layout (location = 1) in vec3 normal;

uniform mat4 mModelViewProj;

out salidaVS
{
    vec3 posicionV;
    vec3 normalV;
} salida;

void main ()
{
    salida.posicionV = posicion;
    salida.normalV = normal;
    gl_Position = mModelViewProj * vec4 ( posicion, 1 );
}
