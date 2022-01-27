// Nombre: José Miguel, Apellidos: González Cañadas, DNI/pasaporte: 77184606X (IG GIM+GIADE, curso 21-22)
// *********************************************************************
// **
// ** Gestión de una grafo de escena (implementación)
// ** Copyright (C) 2016 Carlos Ureña
// **
// ** This program is free software: you can redistribute it and/or modify
// ** it under the terms of the GNU General Public License as published by
// ** the Free Software Foundation, either version 3 of the License, or
// ** (at your option) any later version.
// **
// ** This program is distributed in the hope that it will be useful,
// ** but WITHOUT ANY WARRANTY; without even the implied warranty of
// ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// ** GNU General Public License for more details.
// **
// ** You should have received a copy of the GNU General Public License
// ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
// **
// *********************************************************************

#include "ig-aux.h"
#include "matrices-tr.h"
#include "malla-ind.h"
#include "malla-revol.h"
#include "grafo-escena.h"

using namespace std ;

// *********************************************************************
// Entrada del nodo del Grafo de Escena

// ---------------------------------------------------------------------
// Constructor para entrada de tipo sub-objeto

EntradaNGE::EntradaNGE( Objeto3D * pObjeto )
{
   assert( pObjeto != NULL );
   tipo   = TipoEntNGE::objeto ;
   objeto = pObjeto ;
}
// ---------------------------------------------------------------------
// Constructor para entrada de tipo "matriz de transformación"

EntradaNGE::EntradaNGE( const Matriz4f & pMatriz )
{
   tipo    = TipoEntNGE::transformacion ;
   matriz  = new Matriz4f() ; // matriz en el heap, puntero propietario
   *matriz = pMatriz ;
}

// ---------------------------------------------------------------------
// Constructor para entrada de tipo "matriz de transformación"

EntradaNGE::EntradaNGE( Material * pMaterial )
{
   assert( pMaterial != NULL );
   tipo     = TipoEntNGE::material ;
   material = pMaterial ;
}

// -----------------------------------------------------------------------------
// Destructor de una entrada

EntradaNGE::~EntradaNGE()
{
   /**  no fnciona debido a que se hacen copias (duplicados) de punteros
   if ( tipo == TipoEntNGE::transformacion )
   {
      assert( matriz != NULL );
      delete matriz ;
      matriz = NULL ;
   }
   * **/
}

// *****************************************************************************
// Nodo del grafo de escena: contiene una lista de entradas
// *****************************************************************************

// -----------------------------------------------------------------------------
// Visualiza usando OpenGL

void NodoGrafoEscena::visualizarGL( ContextoVis & cv )
{
   // COMPLETAR: práctica 3: recorrer las entradas y visualizar cada nodo.
   // ........

   // Salvar modelview actual
   cv.cauce_act->pushMM();
   // Salvar color anterior
   const Tupla4f color_previo = leerFijarColVertsCauce(cv);
   
   Material * cvMaterial = nullptr;

   if (cv.iluminacion){
      cvMaterial = cv.material_act;
   }

   // Se recorren todas las entradas del array que hay en el nodo
   for (unsigned i = 0; i < entradas.size(); i++){
      switch (entradas[i].tipo){
         case TipoEntNGE::objeto: // entrada objeto
            entradas[i].objeto->visualizarGL(cv); // visualizar objeto
            break;
         case TipoEntNGE::transformacion: // entrada transformación
            cv.cauce_act->compMM(*(entradas[i].matriz)); // componer matriz
            break;
         case TipoEntNGE::material:
            if (cv.iluminacion && !cv.modo_seleccion){
               cv.material_act = entradas[i].material;
               cv.material_act->activar(cv);
            }
         default:
            break;
      }
   }
   // Restaurar color previo
   glColor4fv( color_previo );

   if (cvMaterial != nullptr){
      cv.material_act = cvMaterial;
      cv.material_act->activar(cv);
   }

   // Restaura modelview guardada
   cv.cauce_act->popMM();

   // COMPLETAR: práctica 4: en la práctica 4, si 'cv.iluminacion' es 'true',
   // se deben de gestionar los materiales:
   //   1. guardar puntero al material activo al inicio (está en cv.material_act)
   //   2. si una entrada des de tipo material, activarlo y actualizar 'cv.material_act'
   //   3. al finalizar, restaurar el material activo al inicio (si es distinto del actual)
}
// *****************************************************************************
// visualizar pura y simplemente la geometría, sin colores, normales, coord. text. etc...
// ignora el colo o identificador del nodo, ignora las entradas de tipo material
// (se supone que el estado de OpenGL está fijado antes de esta llamada de alguna forma adecuada)




// -----------------------------------------------------------------------------

NodoGrafoEscena::NodoGrafoEscena()
{

}

// -----------------------------------------------------------------------------
// Añadir una entrada (al final).
// genérica (de cualqiuer tipo de entrada)

unsigned NodoGrafoEscena::agregar( const EntradaNGE & entrada )
{
   // COMPLETAR: práctica 3: agregar la entrada al nodo, devolver índice de la entrada agregada
   // ........
   entradas.push_back(entrada);

   return entradas.size()-1 ; // sustituir por lo que corresponda ....

}
// -----------------------------------------------------------------------------
// construir una entrada y añadirla (al final)
// objeto (copia solo puntero)

unsigned NodoGrafoEscena::agregar( Objeto3D * pObjeto )
{
   return agregar( EntradaNGE( pObjeto ) );
}
// ---------------------------------------------------------------------
// construir una entrada y añadirla (al final)
// matriz (copia objeto)

unsigned NodoGrafoEscena::agregar( const Matriz4f & pMatriz )
{
   return agregar( EntradaNGE( pMatriz ) );
}
// ---------------------------------------------------------------------
// material (copia solo puntero)
unsigned NodoGrafoEscena::agregar( Material * pMaterial )
{
   return agregar( EntradaNGE( pMaterial ) );
}

// devuelve el puntero a la matriz en la i-ésima entrada
Matriz4f * NodoGrafoEscena::leerPtrMatriz( unsigned indice )
{
   // COMPLETAR: práctica 3: devolver puntero la matriz en ese índice
   //   (debe de dar error y abortar si no hay una matriz en esa entrada)
   // ........(sustituir 'return nullptr' por lo que corresponda)
   assert (indice < entradas.size());
   assert (entradas[indice].tipo == TipoEntNGE::transformacion);
   assert (entradas[indice].matriz != nullptr);
   
   return entradas[indice].matriz;


}
// -----------------------------------------------------------------------------
// si 'centro_calculado' es 'false', recalcula el centro usando los centros
// de los hijos (el punto medio de la caja englobante de los centros de hijos)

void NodoGrafoEscena::calcularCentroOC()
{
   // COMPLETAR: práctica 5: calcular y guardar el centro del nodo
   //    en coordenadas de objeto (hay que hacerlo recursivamente)
   //   (si el centro ya ha sido calculado, no volver a hacerlo)
   // ........
   if (!centro_calculado){
      Matriz4f matriz = MAT_Ident();
      Tupla3f suma = {0.0, 0.0, 0.0};
      float num_centros = 0.0;

      for (unsigned int i = 0; i < entradas.size(); ++i){
         if (entradas[i].tipo == TipoEntNGE::objeto){
            entradas[i].objeto->calcularCentroOC();
            suma = suma + (matriz * entradas[i].objeto->leerCentroOC());
            num_centros++;
         }
         else if ( entradas[i].tipo == TipoEntNGE::transformacion)
            matriz = matriz * (*entradas[i].matriz);
      }
      Tupla3f centro = suma/num_centros;

      ponerCentroOC(centro);
      centro_calculado = true;
   }
}
// -----------------------------------------------------------------------------
// método para buscar un objeto con un identificador y devolver un puntero al mismo

bool NodoGrafoEscena::buscarObjeto
(
   const int         ident_busc, // identificador a buscar
   const Matriz4f &  mmodelado,  // matriz de modelado
   Objeto3D       ** objeto,     // (salida) puntero al puntero al objeto
   Tupla3f &         centro_wc   // (salida) centro del objeto en coordenadas del mundo
)
{
   assert( 0 < ident_busc );

   // COMPLETAR: práctica 5: buscar un sub-objeto con un identificador
   // Se deben de dar estos pasos:

   // 1. calcula el centro del objeto, (sólo la primera vez)
   // ........
   calcularCentroOC();
   // 2. si el identificador del nodo es el que se busca, ya está (terminar)
   // ........
   if(leerIdentificador() == ident_busc){
      centro_wc = mmodelado * leerCentroOC();
      *objeto = this;
      return true;
   }
   // 3. El nodo no es el buscado: buscar recursivamente en los hijos
   //    (si alguna llamada para un sub-árbol lo encuentra, terminar y devolver 'true')
   // ........
   Matriz4f matriz = mmodelado;

   for (unsigned int i = 0; i < entradas.size(); ++i) {
      if ( entradas[i].tipo == TipoEntNGE::objeto){
         if(entradas[i].objeto->buscarObjeto(ident_busc, matriz, objeto, centro_wc))
            return true;
      }
      else if (entradas[i].tipo == TipoEntNGE::transformacion)
         matriz = matriz * (*entradas[i].matriz);
   }
   // ni este nodo ni ningún hijo es el buscado: terminar
   return false ;
}

// ****************************************************************************
// Clase 'NodoCubo24'

NodoCubo24::NodoCubo24(){
   ponerNombre("Cubo de 24 vértices");
   
   Textura * tex = new Textura("../recursos/imgs/window-icon.jpg");
   
   agregar( new Material(tex, 0.8, 0.7, 0.9, 20) );
   agregar( new Cubo24() );
}

/* -- Exámenes de otros años -- */

GrafoEstrellaX::GrafoEstrellaX(unsigned n){
   numParametros=1;
   Cono * cono = new Cono(80,80);
   
   unsigned k=agregar(MAT_Rotacion(0.0, 0.0, 1.0, 0.0));
   rotacionEjeX = leerPtrMatriz(k);
   agregar(new GrafoEstrella(n));
   agregar(new ConoExamen(cono));
   for (unsigned i=1; i<n; i++){
      agregar(MAT_Rotacion(360.0/float(n),1.0,0.0,0.0));
      agregar(new ConoExamen(cono));
   }
}
              
GrafoEstrella::GrafoEstrella(unsigned n){
   assert(n>1);
   EstrellaZ * estrella = new EstrellaZ(n);
   
   agregar(MAT_Rotacion(90.0,0.0,1.0,0.0));
   agregar(MAT_Escalado(2.6,2.6,1.0));
   agregar(MAT_Traslacion(-0.5,-0.5,0));
   agregar(estrella);
}

ConoExamen::ConoExamen(Cono* cono){
   agregar(MAT_Traslacion(0.0,1.3,0.0));
   agregar(MAT_Escalado(0.14,0.15,0.14));
   agregar(cono);
}
              
unsigned GrafoEstrellaX::leerNumParametros() const{
   return numParametros;
}

void GrafoEstrellaX::actualizarEstadoParametro(const unsigned iParam, const float t_sec){
   assert(iParam < leerNumParametros());
   *rotacionEjeX = MAT_Rotacion(360*2.5*t_sec, 1.0, 0.0, 0.0);
}

GrafoCubos::GrafoCubos(){
   numParametros=1;
   RejillaY * rejilla = new RejillaY(8,8);
   
   // Agrego las rejillas verticales
   agregar(new RejillaYCaraEjeXNegativo(rejilla));
   agregar(new RejillaYCaraEjeXPositivo(rejilla));
   agregar(new RejillaYCaraEjeZNegativo(rejilla));
   agregar(new RejillaYCaraEjeZPositivo(rejilla));
   
   // Agrego las rejillas superiores e inferiores (ahorro una traslación)
   
   agregar(new RejillaYInferior(rejilla));
   agregar(new RejillaYSuperior(rejilla));
   
   // Agrego los seis cubos
   
   ObjetoCuboMatriz *obj = new ObjetoCuboMatriz(rotacionCubos);
   agregar(new CuboCaraEjeXNegativo(obj));
   agregar(new CuboCaraEjeXPositivo(obj));
   agregar(new CuboCaraEjeZNegativo(obj));
   agregar(new CuboCaraEjeZPositivo(obj));
   agregar(new CuboCaraInferior(obj));
   agregar(new CuboCaraSuperior(obj));
   
}

RejillaYCaraEjeXNegativo::RejillaYCaraEjeXNegativo(RejillaY * rejilla){
   agregar(MAT_Traslacion(-0.5,-0.5,-0.5));
   agregar(MAT_Rotacion(90.0,0.0,0.0,1.0));
   agregar(rejilla);
}
RejillaYCaraEjeXPositivo::RejillaYCaraEjeXPositivo(RejillaY * rejilla){
   agregar(MAT_Traslacion(+0.5,-0.5,-0.5));
   agregar(MAT_Rotacion(90.0,0.0,0.0,1.0));
   agregar(rejilla);
}
RejillaYCaraEjeZNegativo::RejillaYCaraEjeZNegativo(RejillaY * rejilla){
   agregar(MAT_Traslacion(-0.5,-0.5,-0.5));
   agregar(MAT_Rotacion(-90.0,1.0,0.0,0.0));
   agregar(rejilla);
}
RejillaYCaraEjeZPositivo::RejillaYCaraEjeZPositivo(RejillaY * rejilla){
   agregar(MAT_Traslacion(-0.5,-0.5,+0.5));
   agregar(MAT_Rotacion(-90.0,1.0,0.0,0.0));
   agregar(rejilla);
}

RejillaYSuperior::RejillaYSuperior(RejillaY * rejilla){
   agregar(MAT_Traslacion(-0.5,+0.5,-0.5));
   agregar(rejilla);
}

RejillaYInferior::RejillaYInferior(RejillaY * rejilla){
   agregar(MAT_Traslacion(-0.5,-0.5,-0.5));
   agregar(rejilla);
}

CuboCaraEjeXNegativo::CuboCaraEjeXNegativo(ObjetoCuboMatriz *obj){
   agregar(MAT_Traslacion(-0.5,0.0,0.0));
   agregar(MAT_Escalado(0.5,0.125,0.125));
   agregar(obj);
}
           
CuboCaraEjeXPositivo::CuboCaraEjeXPositivo(ObjetoCuboMatriz *obj) {
   agregar(MAT_Traslacion(+0.5,0.0,0.0));
   agregar(MAT_Escalado(0.5,0.125,0.125));
   agregar(obj);
}
           
CuboCaraEjeZNegativo::CuboCaraEjeZNegativo(ObjetoCuboMatriz *obj) {
   agregar(MAT_Traslacion(0.0,0.0,-0.5));
   agregar(MAT_Escalado(0.125,0.125,0.5));
   agregar(MAT_Rotacion(-90.0,0.0,1.0,0.0));
   agregar(obj);
}
                      
CuboCaraEjeZPositivo::CuboCaraEjeZPositivo(ObjetoCuboMatriz *obj) {
   agregar(MAT_Traslacion(0.0,0.0,+0.5));
   agregar(MAT_Escalado(0.125,0.125,0.5));
   agregar(MAT_Rotacion(-90.0,0.0,1.0,0.0));
   agregar(obj);
}

CuboCaraInferior::CuboCaraInferior(ObjetoCuboMatriz *obj) {
   agregar(MAT_Traslacion(0.0,-0.5,0.0));
   agregar(MAT_Escalado(0.125,0.5,0.125));
   agregar(MAT_Rotacion(90.0,0.0,0.0,1.0));
   agregar(obj);
}

CuboCaraSuperior::CuboCaraSuperior(ObjetoCuboMatriz *obj) {
   agregar(MAT_Traslacion(0.0,+0.5,0.0));
   agregar(MAT_Escalado(0.125,0.5,0.125));
   agregar(MAT_Rotacion(90.0,0.0,0.0,1.0));
   agregar(obj);
}

ObjetoCuboMatriz::ObjetoCuboMatriz(Matriz4f * &rotacionCubos){
   unsigned k=agregar(MAT_Rotacion(0.0,1.0,1.0,1.0));
   rotacionCubos = leerPtrMatriz(k);
   agregar(new Cubo);
}

unsigned GrafoCubos::leerNumParametros() const{
   return numParametros;
}

void GrafoCubos::actualizarEstadoParametro(const unsigned iParam, const float t_sec){
   assert(iParam < leerNumParametros());
   *rotacionCubos = MAT_Rotacion(360*t_sec, 1.0, 0.0, 0.0);
}

VariosCubos::VariosCubos(int n, float s_min, float s_max, float T){
   assert(n>0 && s_min>0 && s_max<1 && s_max>s_min && T>0);
   /* No me da tiempo a implementar las animaciones */
   //numParametros=1;
   Cubo * cubo = new Cubo();
   float s=(s_min+s_max)/2.0,
         lado_anterior = 2.0,
         lado = lado_anterior*s,
         suma_lados = lado_anterior/2.0+lado/2.0;
   
   agregar(cubo);
   
   for (int i=1; i<n; i++){
      agregar(new VariosCubos_gestionaCubos(cubo,lado_anterior,lado,s,i,suma_lados));
      suma_lados+=lado_anterior/2.0+lado/2.0;
   }
}

VariosCubos_gestionaCubos::VariosCubos_gestionaCubos
(Cubo * cubo, float & lado_anterior, float & lado, float s, int i, float suma_lados){
   
   float escalado=1.0;
   for (int j=0; j<i; j++) escalado*=s;
   
   // Agregamos el cubo escalado y trasladado
   agregar(MAT_Traslacion(suma_lados, 0.0, 0.0));
   agregar(MAT_Escalado(escalado, escalado, escalado));
   agregar(cubo);
   lado_anterior = lado;
   lado = lado_anterior*s;
}

NodoDiscoP4::NodoDiscoP4()
{
   ponerNombre("Nodo ejercicio adicional práctica 4, examen 27 enero");
   Textura * textura = new Textura("../recursos/imgs/ea-textura-cuadricula.jpeg");
   agregar ( new Material(textura, 0.5, 0.5, 0.5, 15) );
   agregar( new MallaDiscoP4() );
}

GrafoEsferasP5::GrafoEsferasP5() {

   const unsigned
      n_filas_esferas = 8,
      n_esferas_x_fila = 5 ;
   const float
      e = 0.4/n_esferas_x_fila ;
   
   agregar( MAT_Escalado( e,e,e ));
   
   for( unsigned i = 0 ; i < n_filas_esferas ; i++ ) {
      NodoGrafoEscena * fila_esferas = new NodoGrafoEscena() ;
      for( unsigned j = 0 ; j < n_esferas_x_fila ; j++ ) {
         unsigned id = i*n_esferas_x_fila+j+1;
         MallaInd * esfera = new EsferaID(30,30,id) ;
         fila_esferas->agregar( MAT_Traslacion( 2.2, 0.0, 0.0 ));
         fila_esferas->agregar( esfera );
      }
      agregar( fila_esferas );
      agregar( MAT_Traslacion( 0.0, 0.0, 5.0 ));
   }
}

GrafoEsferasP5_2::GrafoEsferasP5_2() {
   const unsigned
      n_filas_esferas = 8,
      n_esferas_x_fila = 5 ;
   const float
      e = 2.5/n_esferas_x_fila ;
 
   agregar( MAT_Escalado( e, e, e ));
   
   for( unsigned i = 0 ; i < n_filas_esferas ; i++ ) {
      NodoGrafoEscena * fila_esferas = new NodoGrafoEscena() ;
      fila_esferas->agregar( MAT_Traslacion( 3.0, 0.0, 0.0 ));
      for( unsigned j = 0 ; j < n_esferas_x_fila ; j++ ) {
         unsigned id = i*n_esferas_x_fila+j+1;
         MallaInd * esfera = new EsferaID(30,30,id) ;
         fila_esferas->agregar( MAT_Traslacion( 2.5, 0.0, 0.0 ));
         fila_esferas->agregar( esfera );
      }
      agregar( fila_esferas );
      agregar( MAT_Rotacion( 360.0/n_filas_esferas, { 0.0, 1.0, 0.0 }));
   }
}

P4Malla::P4Malla(){
   
   ponerNombre("Malla examen práctica 4 (21-22)");
       
   vertices =
   {   { 0.0, 1.0, 0.0 }, { +1.0, 0.0, +1.0  }, { +1.0, 0.0, -1.0 },  // mira hacia X+ e Y+
       { 0.0, 1.0, 0.0 }, { -1.0, 0.0, -1.0  }, { -1.0, 0.0, +1.0 },  // mira hacia X- e Y+
       { 0.0, 1.0, 0.0 }, { -1.0, 0.0, +1.0  }, { +1.0, 0.0, +1.0 },  // mira hacia Z+ e Y+
       { 0.0, 1.0, 0.0 }, { +1.0, 0.0, -1.0  }, { -1.0, 0.0, -1.0 }   // mira hacia Z- e Y+
   } ;

   triangulos =
   {  {0,1,2}, {3,4,5}, {6,7,8}, {9,10,11}
   } ;
   
   cc_tt_ver = {
      {0.5,1.0}, {0.0,0.0}, {1.0,0.0},
      {0.5,1.0}, {0.0,0.0}, {1.0,0.0},
      {0.5,1.0}, {0.0,0.0}, {1.0,0.0},
      {0.5,1.0}, {0.0,0.0}, {1.0,0.0}
   };
   
   nor_ver = {
      {1.0/sqrt(2), 1.0/sqrt(2), 0.0}, {1.0/sqrt(2), 1.0/sqrt(2), 0.0}, {1.0/sqrt(2), 1.0/sqrt(2), 0.0},
      {-1.0/sqrt(2), 1.0/sqrt(2), 0.0}, {-1.0/sqrt(2), 1.0/sqrt(2), 0.0}, {-1.0/sqrt(2), 1.0/sqrt(2), 0.0},
      {0.0, 1.0/sqrt(2), 1.0/sqrt(2)}, {0.0, 1.0/sqrt(2), 1.0/sqrt(2)}, {0.0, 1.0/sqrt(2), 1.0/sqrt(2)},
      {0.0, 1.0/sqrt(2), -1.0/sqrt(2)}, {0.0, 1.0/sqrt(2), -1.0/sqrt(2)}, {0.0, 1.0/sqrt(2), -1.0/sqrt(2)}
   };
}

P4Nodo::P4Nodo(){
   ponerNombre("Nodo examen práctica 4 (21-22)");
   Textura * textura = new Textura("imgs/textura-examen.jpg");
   agregar ( new Material(textura, 0.8, 0.3, 0.3, 15) );
   agregar( new P4Malla() );
}

P5Malla::P5Malla(int id){
   ponerNombre("P5Malla");
   ponerIdentificador(id);
       
   vertices =
   {
      {1,-1,1}, {1,-1,-1}, {1,1,-1}, {1,1,1}
   } ;

   triangulos =
   {
      {0,1,2}, {0,2,3}
   } ;
   
}

P5Nodo::P5Nodo(){
   
   P5Malla * malla100 = new P5Malla(100);
   agregar(malla100);
   
   agregar(MAT_Rotacion(90.0,0.0,1.0,0.0));
   P5Malla * malla101 = new P5Malla(101);
   agregar(malla101);
   agregar(MAT_Rotacion(90.0,0.0,1.0,0.0));
   P5Malla * malla102 = new P5Malla(102);
   agregar(malla102);
   P5Malla * malla103 = new P5Malla(103);
   agregar(MAT_Rotacion(90.0,0.0,1.0,0.0));
   agregar(malla103);
}

