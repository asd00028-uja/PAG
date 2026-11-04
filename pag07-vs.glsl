#version 410
layout (location = 0) in vec3 posicion;
layout (location = 1) in vec3 normal;

uniform mat4 mModelViewProj;
uniform mat4 mModelView;

out salidaVS
{
    vec3 posicionV;
    vec3 normalV;
} salida;

void main ()
{
    salida.posicionV = vec3 ( mModelView * vec4 ( posicion, 1 ) );
    salida.normalV = normalize ( mat3 ( transpose ( inverse ( mModelView ) ) ) * normal );
    gl_Position = mModelViewProj * vec4 ( posicion, 1 );
}
