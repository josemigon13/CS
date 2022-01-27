// Nombre: José Miguel, Apellidos: González Cañadas, DNI/pasaporte: 77184606X (IG GIM+GIADE, curso 21-22)
// *********************************************************************
// **
// ** Informática Gráfica, curso 2020-21
// ** Declaraciones de la clase Objeto3D.hpp
// **
// *********************************************************************

#include "ig-aux.h"
#include "tuplasg.h"
#include "malla-ind.h"   // declaración de 'ContextoVis'
#include "lector-ply.h"
#include <cmath>


// *****************************************************************************
// funciones auxiliares

// *****************************************************************************
// métodos de la clase MallaInd.

MallaInd::MallaInd()
{
   // nombre por defecto
   ponerNombre("malla indexada, anónima");
}
// -----------------------------------------------------------------------------

MallaInd::MallaInd( const std::string & nombreIni )
{
   // 'identificador' puesto a 0 por defecto, 'centro_oc' puesto a (0,0,0)
   ponerNombre(nombreIni) ;
}

//-----------------------------------------------------------------------------
// calcula la tabla de normales de triángulos una sola vez, si no estaba calculada

void MallaInd::calcularNormalesTriangulos()
{
   // si ya está creada la tabla de normales de triángulos, no es necesario volver a crearla
   const unsigned nt = triangulos.size() ;
   assert( 1 <= nt );
   if ( 0 < nor_tri.size() )
   {
      assert( nt == nor_tri.size() );
      return ;
   }

   // COMPLETAR: Práctica 4: creación de la tabla de normales de triángulos
   // ....
   Tupla3f ver1, ver2, ver3, aux1, aux2, normal;
   for(unsigned int i = 0; i<triangulos.size(); i++){
      ver1 = vertices[triangulos[i](0)];
      ver2 = vertices[triangulos[i](1)];
      ver3 = vertices[triangulos[i](2)];

      aux1 = ver2-ver1;
      aux2 = ver3-ver1;

      normal = aux1.cross(aux2);
      if(normal.lengthSq() > 0)
         nor_tri.push_back(normal.normalized());
      else
         nor_tri.push_back({0.0, 0.0, 0.0});
   }
}

// -----------------------------------------------------------------------------
// calcula las dos tablas de normales

void MallaInd::calcularNormales()
{
   // COMPLETAR: en la práctica 4: calculo de las normales de la malla
   // se debe invocar en primer lugar 'calcularNormalesTriangulos'
   // .......

   // Calculamos las normales de cada triángulo
   calcularNormalesTriangulos();

   // Inicializamos vector
   for (unsigned int i = 0; i < vertices.size(); ++i){
      nor_ver.push_back({0.0, 0.0, 0.0});
   }

   // A cada vértice de un triángulo le sumamos la normal de dicho triángulo. No es necesario dividir porque normalizamos
   for (unsigned int i = 0; i < triangulos.size(); ++i){
      nor_ver[triangulos[i](0)] = nor_ver[triangulos[i](0)] + nor_tri[i];
      nor_ver[triangulos[i](1)] = nor_ver[triangulos[i](1)] + nor_tri[i];
      nor_ver[triangulos[i](2)] = nor_ver[triangulos[i](2)] + nor_tri[i];
   }

   // Guardamos la normal normalizada de cada vértice si no es nula
   for (unsigned int i = 0; i < vertices.size(); ++i){
      if (nor_ver[i].lengthSq() > 0)
         nor_ver[i] = nor_ver[i].normalized();
   }
}

void MallaInd::visualizarNormales()
{
   using namespace std ;

   if ( nor_ver.size() == 0 )
   {
      cout << "Advertencia: intentando dibujar normales de una malla que no tiene tabla (" << leerNombre() << ")." << endl ;
      return ;
   }
   if ( array_verts_normales == nullptr )
   {
      for( unsigned i = 0 ; i < vertices.size() ; i++ )
      {
         segmentos_normales.push_back( vertices[i] );
         segmentos_normales.push_back( vertices[i]+ 0.35f*(nor_ver[i]) );
      }
      array_verts_normales = new ArrayVertices( GL_FLOAT, 3, segmentos_normales.size(), segmentos_normales.data() );
   }

   array_verts_normales->visualizarGL_MI_DAE( GL_LINES );
   CError();
}

// ----------------------------------------------------------------------------

void MallaInd::visualizarGL( ContextoVis & cv )
{
   using namespace std ;
   assert( cv.cauce_act != nullptr );

   if ( triangulos.size() == 0 || vertices.size() == 0 )
   {  cout << "advertencia: intentando dibujar malla vacía '" << leerNombre() << "'" << endl << flush ;
      return ;
   }
   
   if ( cv.visualizando_normales )
   {  visualizarNormales(  );
      return ;
   }

   // guardar el color previamente fijado
   const Tupla4f color_previo = leerFijarColVertsCauce( cv );

   // COMPLETAR: práctica 1: si el puntero 'array_verts' es nulo, crear el objeto ArrayVerts
   //   * en el constructor se dan los datos de la tabla de coordenadas de vértices (tabla 'vertices')
   //   * después hay que invocar a 'fijarIndices', usando el formato y datos de la tabla de triángulos ('triangulos')
   //   * si las tablas 'col_ver', 'cc_tt_ver' o 'nor_ver' no están vacías, hay que invocar los métodos 
   //     'fijarColores', 'fijarCoordText' y 'fijarNormales', como corresponda.

   if (array_verts == nullptr){
      array_verts = new ArrayVertices(GL_FLOAT, 3, vertices.size(), vertices.data());
   
      array_verts->fijarIndices(GL_UNSIGNED_INT, 3*triangulos.size(), triangulos.data());

      if (col_ver.size() != 0)
         array_verts->fijarColores(GL_FLOAT, 3, col_ver.data());
      if (cc_tt_ver.size() != 0)
         array_verts->fijarCoordText(GL_FLOAT, 2, cc_tt_ver.data());
      if (nor_ver.size() != 0)
         array_verts->fijarNormales(GL_FLOAT, nor_ver.data());
   }

   // COMPLETAR: práctica 1: visualizar según el modo (en 'cv.modo_envio')
   //   ** inmediato begin/end       : usar método 'visualizarGL_MI_BVE' de 'ArrayVerts'
   //   ** inmediato con una llamada : usar método 'visualizarGL_MI_DAE' de 'ArrayVerts'
   //   ** diferido (con un VAO)     : usar método 'visualizarGL_MD_VAO' de 'ArrayVerts'
   // (en cualquier caso hay que pasar como parámetro el tipo de primitiva adecuada a una malla de triángulos).
   // .....

   switch (cv.modo_envio){
      case ModosEnvio::inmediato_begin_end:
         array_verts->visualizarGL_MI_BVE(GL_TRIANGLES);
         break;
      case ModosEnvio::inmediato_drawelements:
         array_verts->visualizarGL_MI_DAE(GL_TRIANGLES);
         break;
      case ModosEnvio::diferido_vao:
         array_verts->visualizarGL_MD_VAO(GL_TRIANGLES);
         break;
      default:
         break;
   }
   
   // restaurar el color previamente fijado
   glColor4fv( color_previo );
   
}

// ****************************************************************************
// Clase 'MallaPLY'

MallaPLY::MallaPLY( const std::string & nombre_arch )
{
   ponerNombre( std::string("malla leída del archivo '") + nombre_arch + "'" );

   // COMPLETAR: práctica 2: leer archivo PLY e inicializar la malla
   // ..........................
   LeerPLY (nombre_arch, vertices, triangulos);

   // COMPLETAR: práctica 4: invocar  a 'calcularNormales' para el cálculo de normales
   // .................
   calcularNormales();
}

// ****************************************************************************
// Clase 'Cubo'

Cubo::Cubo()
:  MallaInd( "cubo 8 vértices" )
{
   vertices =
      {  { -1.0, -1.0, -1.0 }, // 0
         { -1.0, -1.0, +1.0 }, // 1
         { -1.0, +1.0, -1.0 }, // 2
         { -1.0, +1.0, +1.0 }, // 3
         { +1.0, -1.0, -1.0 }, // 4
         { +1.0, -1.0, +1.0 }, // 5
         { +1.0, +1.0, -1.0 }, // 6
         { +1.0, +1.0, +1.0 }, // 7
      } ;

   triangulos =
      {  {0,1,3}, {0,3,2},
         {4,7,5}, {4,6,7},

         {0,5,1}, {0,4,5},
         {2,3,7}, {2,7,6},

         {0,6,4}, {0,2,6},
         {1,5,7}, {1,7,3}
      } ;
   calcularNormales();
}

// ****************************************************************************
// Clase 'Tetraedro'

Tetraedro::Tetraedro()
:  MallaInd( "tetraedro 4 vértices" )
{

   vertices =
      {  { -1.0, -1.0, -1.0 }, // 0
         { -1.0, -1.0, +1.0 }, // 1
         { -1.0, +1.0, +1.0 }, // 2
         { +1.0, -1.0, +1.0 }, // 3
      } ;

   triangulos =
      {  {0,1,2}, {0,3,2},
         {1,3,2}, {0,1,3},
      } ;

   ponerColor({-0.25, 0.25, 0.25});
   calcularNormales();
}

// ****************************************************************************
// Clase 'CuboColores'

CuboColores::CuboColores()
:  MallaInd( "Cubo colores 8 vértices" )
{

   vertices =
      {  { -1.0, -1.0, -1.0 }, // 0
         { -1.0, -1.0, +1.0 }, // 1
         { -1.0, +1.0, -1.0 }, // 2
         { -1.0, +1.0, +1.0 }, // 3
         { +1.0, -1.0, -1.0 }, // 4
         { +1.0, -1.0, +1.0 }, // 5
         { +1.0, +1.0, -1.0 }, // 6
         { +1.0, +1.0, +1.0 }, // 7
      } ;

   triangulos =
      {  {0,1,3}, {0,3,2},
         {4,7,5}, {4,6,7},

         {0,5,1}, {0,4,5},
         {2,3,7}, {2,7,6},

         {0,6,4}, {0,2,6},
         {1,5,7}, {1,7,3}
      } ;

   for (unsigned i = 0; i < vertices.size(); i++){
      col_ver.push_back(vertices[i]*0.5f + Tupla3f(0.5,0.5,0.5));
   }
}

// ****************************************************************************
// Clase 'Cubo24'

Cubo24::Cubo24(){
   vertices =
      {  { -1.0, -1.0, -1.0 }, // 0
         { -1.0, -1.0, +1.0 }, // 1
         { -1.0, +1.0, -1.0 }, // 2
         { -1.0, +1.0, +1.0 }, // 3
         { +1.0, -1.0, -1.0 }, // 4
         { +1.0, -1.0, +1.0 }, // 5
         { +1.0, +1.0, -1.0 }, // 6
         { +1.0, +1.0, +1.0 }, // 7

    { -1.0, -1.0, -1.0 }, // 0 +8
         { -1.0, -1.0, +1.0 }, // 1 +8
         { -1.0, +1.0, -1.0 }, // 2 +8
         { -1.0, +1.0, +1.0 }, // 3 +8
         { +1.0, -1.0, -1.0 }, // 4 +8
         { +1.0, -1.0, +1.0 }, // 5 +8
         { +1.0, +1.0, -1.0 }, // 6 +8
         { +1.0, +1.0, +1.0 }, // 7 +8

    { -1.0, -1.0, -1.0 }, // 0 +16
         { -1.0, -1.0, +1.0 }, // 1 +16
         { -1.0, +1.0, -1.0 }, // 2 +16
         { -1.0, +1.0, +1.0 }, // 3 +16
         { +1.0, -1.0, -1.0 }, // 4 +16
         { +1.0, -1.0, +1.0 }, // 5 +16
         { +1.0, +1.0, -1.0 }, // 6 +16
         { +1.0, +1.0, +1.0 } // 7 +16
      } ;


   triangulos =
      {  {0,1,3}, {0,3,2}, // X-
         {4,7,5}, {4,6,7}, // X+ (+4)
         {8,13,9}, {8,12,13}, // Y-
         {10,11,15}, {10,15,14}, // Y+ (+2)
         {16,22,20}, {16,18,22}, // Z-
         {17,21,23}, {17,23,19}  // Z+ (+1)
      } ;

   cc_tt_ver=
     {{0,1}, // 0
      {1,1}, // 1
      {0,0}, // 2
      {1,0}, // 3
      {1,1}, // 4
      {0,1}, // 5
      {1,0}, // 6
      {0,0}, // 7

      {0,0}, // 0 +8
      {1,0}, // 1 +8
      {1,0}, // 2 +8
      {0,0}, // 3 +8
      {0,1}, // 4 +8
      {1,1}, // 5 +8
      {1,1}, // 6 +8
      {0,1}, // 7 +8

      {1,1}, // 0 +16
      {0,1}, // 1 +16
      {1,0}, // 2 +16
      {0,0}, // 3 +16
      {0,1}, // 4 +16
      {1,1}, // 5 +16
      {0,0}, // 6 +16
      {1,0} // 7 +16
     };

   calcularNormales();
}

/* -- Exámenes de otros años -- */

EstrellaZ::EstrellaZ(unsigned n)
{
   assert (n>1);
   vertices.push_back({0.0,0.0,0.0});
   
   for (unsigned i=0; i<2*n; i++)
   {
      vertices.push_back({cos(i*M_PI/n), sin(i*M_PI/n), 0.0});
   }
   for (unsigned i=0; i<2*n; i++)
   {
      if (i % 2 != 0)
         vertices[i+1][0] /= 2.0, vertices[i+1][1] /= 2.0;
   }
   for (unsigned i=0; i<2*n; i++)
   {
      vertices[i+1][0] /= 2.0, vertices[i+1][1] /= 2.0;
   }
   for (unsigned i=0; i<2*n+1; i++)
   {
      vertices[i][0] += 0.5, vertices[i][1] += 0.5;
   }
   for (unsigned i=0; i<2*n-1; i++)
   {
      triangulos.push_back({0,i+1,i+2});
   }
   triangulos.push_back({0,1,2*n});
   
   col_ver.push_back({1,1,1});
   for (unsigned i = 1; i < vertices.size(); i++){
      col_ver.push_back(vertices[i]);
   }
   
}

std::vector<Tupla3f> EstrellaZ::getVertices(){
   return vertices;
}

std::vector<Tupla3i> EstrellaZ::getTriangulos(){
   return triangulos;
}

/*       EJERCICIO 1 TEORÍA 20/21      */

CasaX::CasaX()
{
   vertices =
      {  { 0.0, 0.0, 0.0 }, // 0
         { 0.0, +0.65, 0.0 }, // 1
         { 0.0, +0.65, +0.75 }, // 2
         { 0.0, 0.0, +0.75 }, // 3
         { +1.0, 0.0, +0.75 }, // 4
         { +1.0, +0.65, +0.75 }, // 5
         { +1.0, +0.65, 0.0 }, // 6
         { +1.0, 0.0, 0.0 }, // 7
         
         { +1.0, +1.0, +0.375 },
         { 0.0, +1.0, +0.375 }
      } ;

   triangulos =
   {
      {0,2,3}, {0,1,2},
      {2,3,4}, {2,4,5},
      {4,6,5}, {4,7,6},
      {0,6,1}, {7,6,0},
      
      {5,6,8}, {2,1,9},
      {2,8,9}, {2,8,5},
      {8,1,6}, {8,1,9}
   } ;
   dividirTriMax();
   
   for (unsigned i = 0; i < vertices.size(); i++){
      col_ver.push_back(vertices[i]);
   }
}

void CasaX::calcularTriAreaMax(Tupla3i & tri_max, int & pos)
{
   float area_max=0;
   Tupla3f u,v;
   
   for (int i=0; i<int(triangulos.size()); i++){
      u=vertices[triangulos[i][1]]-vertices[triangulos[i][0]];
      v=vertices[triangulos[i][2]]-vertices[triangulos[i][0]];
      float area_tri=sqrt(u.cross(v).lengthSq())*1.0/2;
      if (area_max<area_tri){
         area_max=area_tri;
         tri_max=triangulos[i];
         pos=i;
      }
   }
}

void CasaX::dividirTriMax()
{
   Tupla3i tri_max; int pos; calcularTriAreaMax(tri_max,pos);
   float x=0, y=0, z=0;
   
   for(int i=0; i<3; i++){
      x+=vertices[tri_max[i]][0];
      y+=vertices[tri_max[i]][1];
      z+=vertices[tri_max[i]][2];
   }
   vertices.push_back({1.0/3*x,  1.0/3*y,    1.0/3*z});
   triangulos.push_back({tri_max[0], tri_max[1], vertices.size()-1});
   triangulos.push_back({tri_max[1], tri_max[2], vertices.size()-1});
   triangulos[pos] = {tri_max[2], tri_max[0], vertices.size()-1};
}

MallaTriangulo::MallaTriangulo(){
   vertices =
   {
      { -0.5, 0.0, 0.0 },
      { +0.5, 0.0, 0.0 },
      { 0.0, sqrt(2), 0.0 }
   };
   triangulos = { {0,1,2} };
}

MallaCuadrado::MallaCuadrado(){
   vertices =
   {
      { -0.5, -0.5, 0.0 },
      { -0.5, +0.5, 0.0 },
      { +0.5, -0.5, 0.0 },
      { +0.5, +0.5, 0.0 }
   };
   triangulos = { {0,1,2}, {1,2,3} };
}

MallaPiramideL::MallaPiramideL(){
   vertices =
   {
      { 0.0, 0.0, 0.0 },
      { 0.0, 0.0, +2.0 },
      { +1.5, 0.0, +2.0 },
      { +1.5, 0.0, +1.0 },
      { +3.0, 0.0, +1.0 },
      { +3.0, 0.0, 0.0 },
      { +1.5, +3.0, +1.0 }
   };
   
   triangulos =
   {
      {1,2,6}, {2,3,6}, {3,4,6}, {4,5,6},
      {5,6,0}, {0,1,6},
      {5,1,0}, {4,5,3}, {1,2,3}
   };
}

PiramideEstrellaZ::PiramideEstrellaZ(unsigned n){
   assert (n>1);
   EstrellaZ estrellaZ(n);
   vertices = estrellaZ.getVertices();
   triangulos = estrellaZ.getTriangulos();
   
   vertices.push_back({0.5,0.5,0.5});
   
   for (unsigned i=2*n; i>1; i--){
      triangulos.push_back({i,i-1,2*n+1});
   }
   
   col_ver.push_back({1,1,1});
   for (unsigned i = 1; i < vertices.size(); i++){
      col_ver.push_back(vertices[i]);
   }
   col_ver.push_back({1,1,1});
}

RejillaY::RejillaY(unsigned m, unsigned n){
   assert(m>1 && n>1);
   
   for (unsigned i=0; i<m; i++){
      for (unsigned j=0; j<n; j++){
         vertices.push_back({float(j)/float(n-1),0.0,float(i)/float(m-1)});
      }
   }
   
   for (unsigned i=0; i<m-1; i++){
      for (unsigned j=0; j<n-1; j++){
         triangulos.push_back({i*n+j, i*n+j+1, i*n+j+n});
      }
   }
   
   for (unsigned i=0; i<m-1; i++){
      for (unsigned j=0; j<n-1; j++){
         triangulos.push_back({i*n+j+1, i*n+j+n, i*n+j+n+1});
      }
   }
   for (unsigned i = 0; i < vertices.size(); i++){
      col_ver.push_back(vertices[i]);
   }
}

MallaTorre::MallaTorre(int n){
   
   for (int i=0; i<=n; i++){
      vertices.push_back({-0.5,float(i),-0.5});
      vertices.push_back({-0.5,float(i),+0.5});
      vertices.push_back({+0.5,float(i),+0.5});
      vertices.push_back({+0.5,float(i),-0.5});
   }
   
   for (int i=0; i<n; i++){
      triangulos.push_back({(n-1)*i,(n-1)*i+1,(n-1)*i+5});
      triangulos.push_back({(n-1)*i,(n-1)*i+4,(n-1)*i+5});
      triangulos.push_back({(n-1)*i+1,(n-1)*i+2,(n-1)*i+6});
      triangulos.push_back({(n-1)*i+1,(n-1)*i+5,(n-1)*i+6});
      triangulos.push_back({(n-1)*i+2,(n-1)*i+3,(n-1)*i+7});
      triangulos.push_back({(n-1)*i+2,(n-1)*i+6,(n-1)*i+7});
      triangulos.push_back({(n-1)*i+3,(n-1)*i,(n-1)*i+4});
      triangulos.push_back({(n-1)*i+3,(n-1)*i+7,(n-1)*i+4});
   }
   
}

Piramide::Piramide(float s, float t, float h){
   assert (s>0 && t>0 && h>0 && t>s);
   
   vertices =
   {  { -t/2.0, 0.0, -t/2.0 }, // 0
      { -s/2.0, h, -s/2.0 }, // 1
      { -t/2.0, 0.0, +t/2.0 }, // 2
      { -s/2.0, h, +s/2.0 }, // 3
      { +t/2.0, 0.0, +t/2.0 }, // 4
      { +s/2.0, h, +s/2.0 }, // 5
      { +t/2.0, 0.0, -t/2.0 }, // 6
      { +s/2.0, h, -s/2.0 } // 7
   } ;

   triangulos =
   {
      {2,3,5}, {2,4,5},
      {4,5,7}, {4,6,7},
      {1,6,7}, {0,1,6},
      {0,2,3}, {0,1,3}
   } ;
}

Cono_Examen::Cono_Examen(int n, float r_1, float r_2, float h){
   assert(n>0 && r_1 > r_2 && r_2 > 0 && h>0);
   
   for (int i=0; i<n; i++)
   {
      vertices.push_back({r_1*cos(2*i*M_PI/n), 0.0, r_1*sin(2*i*M_PI/n)});
      vertices.push_back({r_2*cos(2*i*M_PI/n), h, r_2*sin(2*i*M_PI/n)});
   }
   for (int i=0; i<2*n-2; i++)
   {
      triangulos.push_back({i,i+1,i+2});
   }
   triangulos.push_back({2*n-2, 2*n-1, 0});
   triangulos.push_back({2*n-1, 0, 1});
}

MallaDiscoP4::MallaDiscoP4() {
   ponerColor({1.0, 1.0, 1.0});
   const unsigned ni = 23, nj = 31 ;
   
   for( unsigned i= 0 ; i < ni ; i++ )
      for( unsigned j= 0 ; j < nj ; j++ )
      {
         // Primero se introduce la fila de puntos que está sobre el eje X+
         // Se introduce del más cercano al (0,0) al más lejano al (0,0)
         // ( el último que se introduce de la primera fila es el (1,0) )
         const float
            fi = float(i)/float(ni-1),
            fj = float(j)/float(nj-1),
            ai = 2.0*M_PI*fi,
            x =fj*cos(ai),
            y =fj*sin(ai),
            z =0.0;
         vertices.push_back({ x, y, z });
         //cc_tt_ver.push_back({ (x+1.0)/2.0, (y+1.0)/2.0 });     // (1,0) -> (1,1/2), (-1,-1) -> (0,0), (-1,0) -> (0,1/2)
         cc_tt_ver.push_back({ sqrt(pow(x,2)+pow(y,2)), fi });    // POLARES (módulo, ángulo respecto OX)
         //cc_tt_ver.push_back({ 1.0/2 + atan2(z,x)/(2.0*M_PI), 1.0/2 + atan2( y, sqrt(pow(x,2)+pow(z,2))/(2.0*M_PI) ) });  // ESFÉRICAS
      }
   for( unsigned i= 0 ; i < ni-1 ; i++ )
      for( unsigned j= 0 ; j < nj-1 ; j++ )
      {
         triangulos.push_back({ i*nj+j, i*nj+(j+1), (i+1)*nj+(j+1) });
         triangulos.push_back({ i*nj+j, (i+1)*nj+(j+1), (i+1)*nj+j });
      }
}
