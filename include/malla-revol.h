// Nombre: José Miguel, Apellidos: González Cañadas, DNI/pasaporte: 77184606X (IG GIM+GIADE, curso 21-22)
// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Declaraciones de la clase MallaRevol
// **
// *********************************************************************

#ifndef IG_MALLAREVOL_HPP
#define IG_MALLAREVOL_HPP

#include <vector>          // usar std::vector
#include <string>

#include "malla-ind.h"   // declaración de 'Objeto3D'
// ---------------------------------------------------------------------
// clase para mallas indexadas obtenidas a partir de la revolución de un perfil

class MallaRevol : public MallaInd
{
   private:

   protected: //

   MallaRevol() {} // solo usable desde clases derivadas con constructores especificos

   // Método que crea las tablas de vértices, triángulos, normales y cc.de.tt.
   // a partir de un perfil y el número de copias que queremos de dicho perfil.
   void inicializar
   (
      const std::vector<Tupla3f> & perfil,     // tabla de vértices del perfil original
      const unsigned               num_copias  // número de copias del perfil
   ) ;
} ;
// --------------------------------------------------------------------- lauvivaldi@hotmail.com


class MallaRevolPLY : public MallaRevol
{
   public:
   MallaRevolPLY( const std::string & nombre_arch,
                  const unsigned nperfiles ) ;
} ;

// ---------------------------------------------------------------------
// Clases de mallas indexadas por revolución de un perfil generado proceduralmente

class Cilindro : public MallaRevol {
   public:
   // Constructor: crea el perfil original y llama a inicializar
   // La base tiene centro en el origen. Radio y altura unidad
   Cilindro (const int num_verts_per, const unsigned nperfiles);
};

class Cilindro2 : public MallaRevol {
   public:
   // Constructor: crea el perfil original y llama a inicializar
   // La base tiene centro en el origen. Radio 1/4 y altura unidad
   Cilindro2 (const int num_verts_per, const unsigned nperfiles);
};

class Cono : public MallaRevol {
   public:
   // Constructor: crea el perfil original y llama a inicializar
   // La base tiene centro en el origen. Radio y altura unidad
   Cono (const int num_verts_per, const unsigned nperfiles);
};

class Esfera : public MallaRevol {
   public:
   // Constructor: crea el perfil original y llama a inicializar
   // La base tiene centro en el origen. Radio unidad
   Esfera (const int num_verts_per, const unsigned nperfiles);
};

class EsferaID : public MallaRevol {
   public:
   // Constructor: crea el perfil original y llama a inicializar
   // La base tiene centro en el origen. Radio unidad
   // Se le pasa el identificador como parámetro
   EsferaID(const int num_verts_per, const unsigned nperfiles, unsigned id);
};
   

#endif
