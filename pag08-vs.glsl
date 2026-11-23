#version 410
layout (location = 0) in vec3 posicion;
layout (location = 1) in vec3 normal;

uniform mat4 mModelViewProj;
uniform mat4 mModelView;
uniform mat4 mModelViewIT;

out salidaVS
{
    vec3 posicionV;
    vec3 normalV;
} salida;

void main ()
{
    salida.posicionV = vec3 ( mModelView * vec4 ( posicion, 1 ) );
    salida.normalV = vec3 ( mModelViewIT * vec4 ( normal, 0 ) );
    gl_Position = mModelViewProj * vec4 ( posicion, 1 );
}
