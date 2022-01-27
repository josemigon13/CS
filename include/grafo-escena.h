// Nombre: José Miguel, Apellidos: González Cañadas, DNI/pasaporte: 77184606X (IG GIM+GIADE, curso 21-22)
// *********************************************************************
// **
// ** Gestión de una grafo de escena (declaraciones)
// ** Copyright (C) 2014-2017 Carlos Ureña
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


#ifndef GRAFO_ESCENA_HPP
#define GRAFO_ESCENA_HPP

//#include <obj-gra.hpp>
//#include <materiales.hpp>

#include "matrices-tr.h"
#include "objeto3d.h"
#include "malla-revol.h"
#include "materiales-luces.h"

// *********************************************************************
// declaración adelantada de estructura para un nodo del grafo de escena

class NodoGrafoEscena ;

// *********************************************************************
// tipo enumerado con los tipos de entradas del nodo del grafo de escena

enum class TipoEntNGE { objeto, transformacion, material, noInicializado } ;

// *********************************************************************
// Entrada del nodo del Grafo de Escena

struct EntradaNGE
{
   TipoEntNGE tipo ;   // objeto, transformacion, material
   union
   {  Objeto3D * objeto ;  // ptr. a un objeto (no propietario)
      Matriz4f * matriz ;  // ptr. a matriz 4x4 transf. (propietario)
      Material * material ; // ptr. a material (no propietario)
   } ;
   // constructores (uno por tipo)
   EntradaNGE( Objeto3D * pObjeto ) ;      // (copia solo puntero)
   EntradaNGE( const Matriz4f & pMatriz ); // (crea copia en el heap)
   EntradaNGE( Material * pMaterial ) ;    // (copia solo puntero)
   ~EntradaNGE() ;
} ;

// *********************************************************************
// Nodo del grafo de escena: es un objeto 3D parametrizado, que contiene una lista de entradas

class NodoGrafoEscena : public Objeto3D
{
   protected:
   // COMPLETAR: práctica 3: definir variables y métodos privados del nodo
   // .......
   std::vector<EntradaNGE> entradas; // vector de entradas
   bool centro_calculado = false;

   public:

   NodoGrafoEscena() ;

   // visualiza usando OpenGL
   virtual void visualizarGL( ContextoVis & cv ) ;

   // añadir una entrada al final, hace copia de la entrada
   // devuelve indice de la entrada dentro del vector de entradas
   unsigned agregar( const EntradaNGE & entrada );

   // construir una entrada y añadirla (al final)
   unsigned agregar( Objeto3D * pObjeto ); // objeto (copia solo puntero)
   unsigned agregar( const Matriz4f & pMatriz ); // matriz (copia objeto)
   unsigned agregar( Material * pMaterial ); // material (copia solo puntero)

   // devuelve el puntero a la matriz en la i-ésima entrada
   Matriz4f * leerPtrMatriz( unsigned iEnt );

   // método para buscar un objeto con un identificador
   virtual bool buscarObjeto( const int ident_busc, const Matriz4f & mmodelado,
                    Objeto3D ** objeto, Tupla3f & centro_wc )  ;

   // si 'centro_calculado' es 'false', recalcula el centro usando los centros
   // de los hijos (el punto medio de la caja englobante de los centros de hijos)
   virtual void calcularCentroOC() ;

} ;

// ****************************************************************************
// Clase 'NodoCubo24'

class NodoCubo24 : public NodoGrafoEscena
{
  public:
      NodoCubo24();
} ;

/* -- Exámenes de otros años -- */

class GrafoEstrellaX : public NodoGrafoEscena{
   private:
      Matriz4f * rotacionEjeX = nullptr;
      unsigned numParametros;
   public:
      GrafoEstrellaX(unsigned n);
      unsigned leerNumParametros() const;
      void actualizarEstadoParametro(const unsigned iParam, const float t_sec);
};

class GrafoEstrella : public NodoGrafoEscena{
   public:
      GrafoEstrella(unsigned n);
};

class ConoExamen : public NodoGrafoEscena{
   public:
      ConoExamen(Cono* cono);
};

class GrafoCubos : public NodoGrafoEscena{
   private:
      Matriz4f * rotacionCubos = nullptr;
      unsigned numParametros;
   public:
      GrafoCubos();
      unsigned leerNumParametros() const;
      void actualizarEstadoParametro(const unsigned iParam, const float t_sec);
};

class RejillaYCaraEjeXNegativo : public NodoGrafoEscena{
   public:
      RejillaYCaraEjeXNegativo(RejillaY * rejilla);
};
class RejillaYCaraEjeXPositivo : public NodoGrafoEscena{
   public:
      RejillaYCaraEjeXPositivo(RejillaY * rejilla);
};
class RejillaYCaraEjeZNegativo : public NodoGrafoEscena{
   public:
      RejillaYCaraEjeZNegativo(RejillaY * rejilla);
};
class RejillaYCaraEjeZPositivo : public NodoGrafoEscena{
   public:
      RejillaYCaraEjeZPositivo(RejillaY * rejilla);
};

class RejillaYSuperior : public NodoGrafoEscena{
   public:
      RejillaYSuperior(RejillaY * rejilla);
};

class RejillaYInferior : public NodoGrafoEscena{
   public:
      RejillaYInferior(RejillaY * rejilla);
};

class ObjetoCuboMatriz : public NodoGrafoEscena{
   public:
      ObjetoCuboMatriz(Matriz4f * &rotacionCubos);
};

class CuboCaraEjeXNegativo : public NodoGrafoEscena{
   public:
      CuboCaraEjeXNegativo(ObjetoCuboMatriz *obj);
};
class CuboCaraEjeXPositivo : public NodoGrafoEscena{
   public:
      CuboCaraEjeXPositivo(ObjetoCuboMatriz *obj);
};
class CuboCaraEjeZNegativo : public NodoGrafoEscena{
   public:
      CuboCaraEjeZNegativo(ObjetoCuboMatriz *obj);
};
class CuboCaraEjeZPositivo : public NodoGrafoEscena{
   public:
      CuboCaraEjeZPositivo(ObjetoCuboMatriz *obj);
};

class CuboCaraSuperior : public NodoGrafoEscena{
   public:
      CuboCaraSuperior(ObjetoCuboMatriz *obj);
};

class CuboCaraInferior : public NodoGrafoEscena{
   public:
      CuboCaraInferior(ObjetoCuboMatriz *obj);
};

class VariosCubos : public NodoGrafoEscena{
   public:
      VariosCubos(int n, float s_min, float s_max, float T);
};

class VariosCubos_gestionaCubos : public NodoGrafoEscena{
   public:
      VariosCubos_gestionaCubos(Cubo * cubo, float & lado_anterior, float & lado, float s, int i, float suma_lados);
};

class NodoDiscoP4 : public NodoGrafoEscena{
   public:
      NodoDiscoP4();
};

class GrafoEsferasP5 : public NodoGrafoEscena{
   public:
      GrafoEsferasP5();
};

class GrafoEsferasP5_2 : public NodoGrafoEscena{
   public:
      GrafoEsferasP5_2();
};

class P4Malla : public MallaInd{
   public:
      P4Malla();
};

class P4Nodo : public NodoGrafoEscena{
   public:
      P4Nodo();
};

class P5Malla : public MallaInd{
   public:
      P5Malla(int id);
};

class P5Nodo : public NodoGrafoEscena{
   public:
      P5Nodo();
};

#endif // GRAFO_ESCENA_HPP
