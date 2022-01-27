// Nombre: José Miguel, Apellidos: González Cañadas, DNI/pasaporte: 77184606X (IG GIM+GIADE, curso 21-22)
// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Declaraciones de la clase Objeto3D.hpp
// **
// *********************************************************************

#ifndef IG_MALLAIND_HPP
#define IG_MALLAIND_HPP

#include <vector>          // usar std::vector

#include "objeto3d.h"   // declaración de 'Objeto3D'
#include "array-verts.h" // clase 'ArrayVert'

// ---------------------------------------------------------------------
// clase para objetos gráficos genéricos

class MallaInd : public Objeto3D
{
   protected:
      // COMPLETAR: incluir aquí las variables y métodos privados que sean
      // necesarios para una malla indexada
      // ......

      std::vector<Tupla3f> vertices ;
      std::vector<Tupla3i> triangulos ;

      std::vector<Tupla3f> col_ver ;  // colores de los vértices
      std::vector<Tupla3f> nor_ver ;  // normales de vértices
      std::vector<Tupla3f> nor_tri ;  // normales de triangulos
      std::vector<Tupla2f> cc_tt_ver ; // coordenadas de textura de los vértices
   
      // array de vértices con los segmentos de las normales (vis. con GL_LINES)
      // ( se crea bajo demanda en `visualizarNormales`)
      ArrayVertices * array_verts_normales = nullptr ;
      std::vector<Tupla3f> segmentos_normales ; // guarda los segmentos de normales

     
      // array de vértices con información de tablas para visualizar
      // (se crea bajo demanda: la primera vez que se llama a 'visualizarGL')
      ArrayVertices * array_verts = nullptr ; 

      // normales de triángulos y vértices
      void calcularNormales();

      // calculo de las normales de triángulos (solo si no están creadas ya)
      void calcularNormalesTriangulos() ;
   
      void visualizarNormales();

   public:
      // crea una malla vacía (nombre: "malla indexada nueva vacía")
      MallaInd() ;

      // crea una malla vacía con un nombre concreto:
      MallaInd( const std::string & nombreIni );

      // visualizar el objeto con OpenGL
      virtual void visualizarGL( ContextoVis & cv ) ;

} ;
// ---------------------------------------------------------------------
// Clase para mallas obtenidas de un archivo 'ply'
// es un tipo de malla indexada que define un nuevo constructor
// que recibe el nombre del archivo ply como parámetro

class MallaPLY : public MallaInd
{
   public:
      MallaPLY( const std::string & nombre_arch ) ;
} ;


// ---------------------------------------------------------------------

class Cubo : public MallaInd
{
   public:
      Cubo();
};

// ---------------------------------------------------------------------

class CuboTejado : public MallaInd
{
   public:
      CuboTejado();
};

// --------------------------------------------------------------------

class Cubo24 : public MallaInd
{
  public:
      Cubo24();
} ;

// --------------------------------------------------------------------

class Tetraedro : public MallaInd
{
  public:
      Tetraedro();
} ;

// --------------------------------------------------------------------

class CuboColores : public MallaInd
{
  public:
      CuboColores();
} ;

// --------------------------------------------------------------------

class EstrellaZ : public MallaInd
{
  public:
      EstrellaZ(unsigned n);
      std::vector<Tupla3f> getVertices();
      std::vector<Tupla3i> getTriangulos();
} ;

class CasaX : public MallaInd
{
  public:
      CasaX();
      void calcularTriAreaMax(Tupla3i & tri_max, int & pos);
      void dividirTriMax();
} ;

class MallaTriangulo : public MallaInd
{
  public:
      MallaTriangulo();
} ;

class MallaCuadrado : public MallaInd
{
  public:
      MallaCuadrado();
} ;

class MallaPiramideL : public MallaInd
{
  public:
      MallaPiramideL();
} ;

class PiramideEstrellaZ : public MallaInd
{
  public:
      PiramideEstrellaZ(unsigned n);
} ;

class RejillaY : public MallaInd
{
  public:
      RejillaY(unsigned m, unsigned n);
} ;

class MallaTorre : public MallaInd
{
  public:
      MallaTorre(int n);
} ;

class Piramide : public MallaInd
{
  public:
      Piramide(float s, float t, float h);
} ;

class Cono_Examen : public MallaInd
{
  public:
      Cono_Examen(int n, float r_1, float r_2, float h);
} ;

class MallaDiscoP4 : public MallaInd
{
   public:
      MallaDiscoP4();
};
#endif
