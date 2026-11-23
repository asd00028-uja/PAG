#version 410

in salidaVS
{
    vec3 posicionV;
    vec3 normalV;
} entrada;

// Material
uniform vec3 uAmbiente;
uniform vec3 uDifuso;
uniform vec3 uEspecular;
uniform float uBrillo;

// Luz (posición en espacio de visión)
uniform vec3 uIa;
uniform vec3 uId;
uniform vec3 uIs;
uniform vec3 uPosicionLuz;
uniform vec3 uDireccionLuz;   // Normalizada, desde la luz hacia la escena
uniform float uAnguloFoco;    // Gamma, en grados
uniform float uExponenteFoco; // s, para los bordes suaves

out vec4 colorFragmento;

subroutine vec4 fCalcularColor ();

subroutine uniform fCalcularColor uMetodoColorElegido;

subroutine ( fCalcularColor )
vec4 colorAlambre ()
{
    return vec4 ( 1.0, 0.0, 0.0, 1.0 );
}

subroutine ( fCalcularColor )
vec4 luzAmbiente ()
{
    vec3 ambiente = uIa * uAmbiente;
    return vec4 ( ambiente, 1.0 );
}

subroutine ( fCalcularColor )
vec4 luzPuntual ()
{
    vec3 n = normalize ( entrada.normalV );
    vec3 l = normalize ( uPosicionLuz - entrada.posicionV );
    vec3 v = normalize ( -entrada.posicionV );
    vec3 r = reflect ( -l, n );
    vec3 difusa = uId * uDifuso * max ( dot ( l, n ), 0.0 );
    vec3 especular = uIs * uEspecular * pow ( max ( dot ( r, v ), 0.0 ), uBrillo );
    return vec4 ( difusa + especular, 1.0 );
}

subroutine ( fCalcularColor )
vec4 luzDireccional ()
{
    vec3 n = normalize ( entrada.normalV );
    vec3 l = -uDireccionLuz;
    vec3 v = normalize ( -entrada.posicionV );
    vec3 r = reflect ( -l, n );
    vec3 difusa = uId * uDifuso * max ( dot ( l, n ), 0.0 );
    vec3 especular = uIs * uEspecular * pow ( max ( dot ( r, v ), 0.0 ), uBrillo );
    return vec4 ( difusa + especular, 1.0 );
}

subroutine ( fCalcularColor )
vec4 luzFoco ()
{
    vec3 l = normalize ( uPosicionLuz - entrada.posicionV );
    vec3 d = uDireccionLuz;
    float cosGamma = cos ( radians ( uAnguloFoco ) );
    float cosDelta = dot ( -l, d );
    float spotFactor = 0.0;
    if ( cosDelta >= cosGamma ) { spotFactor = pow ( cosDelta, uExponenteFoco ); }
    vec3 n = normalize ( entrada.normalV );
    vec3 v = normalize ( -entrada.posicionV );
    vec3 r = reflect ( -l, n );
    vec3 difusa = uId * uDifuso * max ( dot ( l, n ), 0.0 );
    vec3 especular = uIs * uEspecular * pow ( max ( dot ( r, v ), 0.0 ), uBrillo );
    return vec4 ( spotFactor * ( difusa + especular ), 1.0 );
}

void main ()
{
    colorFragmento = uMetodoColorElegido ();
}
