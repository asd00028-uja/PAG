#version 410

in salidaVS
{
    vec3 posicionV;
    vec3 normalV;
} entrada;

uniform vec3 uDifuso;

out vec4 colorFragmento;

subroutine vec4 fCalcularColor ();

subroutine uniform fCalcularColor uMetodoColorElegido;

subroutine ( fCalcularColor )
vec4 colorAlambre ()
{
    return vec4 ( 1.0, 0.0, 0.0, 1.0 );
}

subroutine ( fCalcularColor )
vec4 colorSolido ()
{
    // Provisional: luz en la cámara (en espacio de visión la cámara está en el
    // origen) para comprobar que posición y normal llegan bien transformadas
    vec3 n = normalize ( entrada.normalV );
    vec3 l = normalize ( -entrada.posicionV );
    return vec4 ( uDifuso * max ( dot ( n, l ), 0.0 ), 1.0 );
}

void main ()
{
    colorFragmento = uMetodoColorElegido ();
}
