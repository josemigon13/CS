// Nombre: José Miguel, Apellidos: González Cañadas, DNI/pasaporte: 77184606X (IG GIM+GIADE, curso 21-22)
#include "modelo-jer.h"
#include "malla-ind.h"
#include "malla-revol.h"
#include "grafo-escena.h"
#include "ig-aux.h"
#include <math.h>

using namespace std;

unsigned SillaArticulada::leerNumParametros() const{
   return numParametros;
}

void SillaArticulada::actualizarEstadoParametro (const unsigned iParam, const float t_sec){
   assert(iParam < leerNumParametros());
   switch(iParam) {
      case 0:
         *rotacionRespaldo
         = MAT_Rotacion(30*sin(0.5*M_PI*t_sec), 0.0, 0.0, 1.0);
         break;
      case 1:
         *rotacionAsiento
         = MAT_Rotacion(60*t_sec, 0.0, 1.0, 0.0);
         break;
      case 2:
         *traslacionSoporteAsiento
         = MAT_Traslacion(0.0, 0.25+0.25*sin(0.5*M_PI*t_sec), 0.0);
         break;
      case 3:
         /* v(m/s) = w(rad/s)r(m), donde
               - v: velocidad lineal, en m/s
               - w: velocidad angular, en rad/s
               - r: radio de la esfera, en m
            Aplicado a nuestro modelo:
               - w = 2*pi rad/s (360 grados/s) -> theta (ángulo de giro) = 2*pi rad
               (360 grados, un ciclo) cada s
               - r = (0.25*1.2*1.7*0.12) * 1 (producto de todos los escalados
               en el eje de traslación aplicados a la esfera inicial (eje X)
               por el radio original de la esfera)
            Por tanto:
               - v = (2*pi)*(0.25*1.2*1.7*0.12) m/s
            De aquí deducimos:
               - x = (2*pi)*(0.25*1.2*1.7*0.12)*t_sec (traslación en el
               eje X, en m) -> x (m) = theta (rad/s) * r (m)
            Nota: consideramos que Tra(+1.0,0.0,0.0) equivale a trasladar el
            modelo 1m en el eje X
         */
         float theta_gr  = 360*t_sec,
               theta_rad = 2*M_PI*t_sec;
         *rotacionRueda = MAT_Rotacion(theta_gr, 0.0, 0.0, -1.0);
         float x = theta_rad*0.25*1.2*1.7*0.12;
         *traslacionSilla = MAT_Traslacion(x, 0.0 , 0.0);
   }
}

SillaArticulada::SillaArticulada(){
   ponerNombre("Silla articulada");
   numParametros = 4;
   
   unsigned k = agregar(MAT_Traslacion(0.0, 0.0, 0.0));
   /* Matriz de traslación de la silla (GL 3) */
   traslacionSilla = leerPtrMatriz(k);
   agregar(new ConectorPata(rotacionRueda));
   
   SoporteAsiento * soporte = new SoporteAsiento;
   agregar(MAT_Escalado(0.4, 0.65, 0.4));
   agregar(soporte);
   
   k = agregar(MAT_Traslacion(0.0, 0.0, 0.0));
   /* Matriz de traslación del soporte del asiento (GL 2) */
   traslacionSoporteAsiento = leerPtrMatriz(k);
   agregar(soporte);
   
   k = agregar(MAT_Rotacion(0.0, 0.0, 1.0, 0.0));
   /* Matriz de rotación del asiento (GL 1) */
   rotacionAsiento = leerPtrMatriz(k);
   agregar(new Asiento(rotacionRespaldo));
}

Respaldo::Respaldo(){
   Cubo24 * cubo = new Cubo24();
   agregar(MAT_Escalado(0.15, 0.95, 1.0));
   cubo->ponerColor({0.56, 0.74, 0.56});
   agregar(cubo);
}

SoporteRespaldo::SoporteRespaldo(){
   Cubo24 * cubo = new Cubo24();
   agregar(MAT_Escalado(0.7, 8, 0.7));
   agregar(MAT_Traslacion(0.0, 1.075, 0.0));
   cubo->ponerColor({0.98, 0.98, 0.82});
   agregar(cubo);
   
   agregar(MAT_Escalado(11.0, 0.75, 11.0));
   agregar(MAT_Traslacion(0.25, 0.7, 0.0));
   /* Agrego el respaldo */
   Textura * textura = new Textura("../recursos/imgs/cuadricula.jpg");
   agregar ( new Material(textura, 0.5, 0.5, 0.5, 15) );
   agregar(new Respaldo);
}

RuedaConectora::RuedaConectora(Matriz4f * &M){
   Esfera * esfera = new Esfera(40, 60);
   agregar(MAT_Escalado(1.0, 1.4, 1.5));
   agregar(MAT_Traslacion(-1.3, 0.2, 0.0));
   esfera->ponerColor({0.87, 1.0, 1.0});
   unsigned k = agregar(MAT_Rotacion(0.0, 0.0, 1.0, 0.0));
   /* Matriz de inclinación del respaldo (GL 0) */
   M = leerPtrMatriz(k);
   agregar(esfera);
   /* Agrego el soporte del respaldo (junto al respaldo) */
   Textura * textura = new Textura("../recursos/imgs/text-madera.jpg");
   agregar ( new Material(textura, 0.5, 0.5, 0.5, 15) );
   agregar(new SoporteRespaldo);
}

ConectorAsiento::ConectorAsiento(Matriz4f * &M){
   Cubo24 * cubo = new Cubo24();
   agregar(MAT_Escalado(0.13, 0.7, 0.10));
   agregar(MAT_Traslacion(-8.5, 0.25, 0.0));
   cubo->ponerColor({0.98, 0.98, 0.82});
   agregar(cubo);
   /* Agrego la rueda conectora
      (junto al respaldo y su soporte) */
   Textura * textura = new Textura("../recursos/imgs/window-icon.jpg");
   agregar ( new Material(textura, 0.5, 0.5, 0.5, 15) );
   agregar(new RuedaConectora(M));
}

Asiento::Asiento(Matriz4f * &M){
   Cubo24 * cubo = new Cubo24();
   agregar(MAT_Escalado(1.1, 0.1, 1.1));
   agregar(MAT_Traslacion(0.0, 11.0, 0.0));
   cubo->ponerColor({0.94, 0.5, 0.5});
   Textura * textura = new Textura("../recursos/imgs/cuadricula.jpg");
   agregar ( new Material(textura, 0.5, 0.5, 0.5, 15) );
   agregar(cubo);
   /* Agrego el conector del asiento
      (junto a la rueda conectora,
      el soporte del respaldo y el mismo) */
   Textura * textura2 = new Textura("../recursos/imgs/text-madera.jpg");
   agregar ( new Material(textura2, 0.5, 0.5, 0.5, 15) );
   agregar(new ConectorAsiento(M));
}

SoporteAsiento::SoporteAsiento(){
   Cilindro2 * cilindro = new Cilindro2(40, 60);
   cilindro->ponerColor({0.0, 0.0, 0.2});
   Textura * textura = new Textura("../recursos/imgs/text-madera.jpg");
   agregar ( new Material(textura, 0.5, 0.5, 0.5, 15) );
   agregar(cilindro);
}

ConectorPata::ConectorPata(Matriz4f * &M){
   Esfera * esfera = new Esfera(40, 60);
   agregar(MAT_Escalado(0.12, 0.12, 0.12));
   esfera->ponerColor({0.98, 0.98, 0.82});
   agregar ( new Material(0.5, 0.5, 0.5, 15) );
   agregar(esfera);
   
   /* Agrego cada una de las patas
      (junto a las ruedas, una única instancia)
    */
   Pata * pata = new Pata(M);
   Textura * textura = new Textura("../recursos/imgs/text-madera.jpg");
   agregar ( new Material(textura, 0.5, 0.5, 0.5, 15) );
   agregar(MAT_Rotacion(90.0, 1.0, 0.0, 0.0));
   agregar(pata);
   agregar(MAT_Rotacion(90.0, 0.0, 0.0, 1.0));
   agregar(pata);
   agregar(MAT_Rotacion(90.0, 0.0, 0.0, 1.0));
   agregar(pata);
   agregar(MAT_Rotacion(90.0, 0.0, 0.0, 1.0));
   agregar(pata);
}

Pata::Pata(Matriz4f * &M){
   Cilindro2 * cilindro = new Cilindro2(40, 60);
   agregar(MAT_Escalado(1.7, 5.5, 1.7));
   cilindro->ponerColor({0.0, 0.0, 0.2});
   agregar (cilindro);
   
   agregar(MAT_Escalado(1.2, 0.36, 1.2));
   agregar(MAT_Traslacion(0.0, 2.8, 0.0));
   agregar(new Rueda(M));
}

Rueda::Rueda(Matriz4f * &M){
   Esfera * esfera = new Esfera(40, 60);
   agregar(MAT_Escalado(0.25, 0.25, 0.25));
   esfera->ponerColor({0.98, 0.98, 0.82});
   unsigned k = agregar(MAT_Rotacion(0.0, 0.0, 1.0, 0.0));
   /* Matriz de rotación de la rueda (GL 3) */
   M = leerPtrMatriz(k);
   agregar ( new Material(0.5, 0.5, 0.5, 15) );
   agregar(esfera);
}
