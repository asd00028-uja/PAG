#version 410
in vec3 vColor;
out vec4 colorFragmento;
void main ()
{
    colorFragmento = vec4 ( vColor, 1.0 );
}
