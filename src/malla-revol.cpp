// Nombre: José Miguel, Apellidos: González Cañadas, DNI/pasaporte: 77184606X (IG GIM+GIADE, curso 21-22)
// *********************************************************************
// **
// ** Informática Gráfica, curso 2019-20
// ** Implementación de la clase 'MallaRevol'
// **
// *********************************************************************

#include "ig-aux.h"
#include "tuplasg.h"
#include "lector-ply.h"
#include "matrices-tr.h"
#include "malla-revol.h"

using namespace std ;

// *****************************************************************************

// Método que crea las tablas de vértices, triángulos, normales y cc.de.tt.
// a partir de un perfil y el número de copias que queremos de dicho perfil.
void MallaRevol::inicializar
(
   const std::vector<Tupla3f> & perfil,     // tabla de vértices del perfil original
   const unsigned               num_copias  // número de copias del perfil
)
{
   unsigned m = perfil.size();
   vector<Tupla3f> nor_arist;
   Tupla3f aux;
     
   for(unsigned j = 0; j < m-1; j++){
      aux = perfil[j+1]-perfil[j];
      aux={aux[Y],-aux[X],0.0};

      if(aux[X]!=0 or aux[Y]!=0 or aux[Z]!=0)
         aux = aux.normalized();
         
      nor_arist.push_back(aux);
   }
   vector<Tupla3f> nor_ver_perfil;
   nor_ver_perfil.push_back(nor_arist.front());
     
   for(unsigned j = 1; j < m-1; j++){
      aux = nor_arist[j-1]+nor_arist[j];

      if(aux[X]!=0 or aux[Y]!=0 or aux[Z]!=0)
         aux = aux.normalized();
       
      nor_ver_perfil.push_back(aux);
   }
     
   nor_ver_perfil.push_back(nor_arist.back());

   // Cálculo de las coordenadas de textura
   vector<float> d;

   for(unsigned j = 0; j < m-1; j++)
      d.push_back(sqrt(pow(perfil[j][X]-perfil[j+1][X],2.0)+pow(perfil[j][Y]-perfil[j+1][Y],2.0)));

   vector<float> t;
   float suma = 0,
         total = 0;
     
   for(unsigned j = 0; j < m-1; j++)
      total+=d[j];

   for(unsigned j = 0; j < m-1; j++){
      t.push_back(suma/total);
      suma += d[j];
   }
   t.push_back(1.0);
     
   // COMPLETAR: Práctica 2: completar: creación de la malla....
     
   // Inicializar tabla de vértices
   for(unsigned i = 0; i < num_copias; i++)
      for(unsigned j = 0; j < m; j++)
      {
         float alpha = 2*M_PI*i/(num_copias-1);
         float c = cos(alpha);
         float s = sin(alpha);
         vertices.push_back({perfil[j][X]*c, perfil[j][Y], perfil[j][X]*s});
         // Añadir su normal
         nor_ver.push_back({nor_ver_perfil[j][X]*c,nor_ver_perfil[j][Y], nor_ver_perfil[j][X]*s});
         // Añadir sus coordenadas de textura
         cc_tt_ver.push_back({1.0-(float)i/(num_copias-1), 1-t[j]});
      }

   // Inicializar tabla de triángulos
   for(unsigned i = 0; i < num_copias-1; i++)
      for(unsigned j = 0; j < m-1; j++)
      {
         int k = i*m+j;
         triangulos.push_back({k+m, k, k+m+1});
         triangulos.push_back({k+m+1, k, k+1});
      }
}

// -----------------------------------------------------------------------------
// constructor, a partir de un archivo PLY

MallaRevolPLY::MallaRevolPLY
(
   const std::string & nombre_arch,
   const unsigned      nperfiles
)
{
   ponerNombre( std::string("malla por revolución del perfil en '"+ nombre_arch + "'" ));
   // COMPLETAR: práctica 2: crear la malla de revolución
   // Leer los vértice del perfil desde un PLY, después llamar a 'inicializar'
   
   vector<Tupla3f> perfil;
   LeerVerticesPLY(nombre_arch, perfil);
   inicializar(perfil, nperfiles);
}

// Constructor: crea el perfil original y llama a inicializar
// La base tiene centro en el origen. Radio y altura unidad
Cilindro::Cilindro (const int num_verts_per, const unsigned nperfiles){
   std::vector<Tupla3f> perfil;

   ponerNombre("Cilindro");
   
   Tupla3f base_inf(0,0,0);
   perfil.push_back(base_inf);

   for (int i = 0; i < num_verts_per; i++)
      perfil.push_back({1, (float)i/(num_verts_per-1), 0});
   
   Tupla3f base_sup(0,1,0);
   perfil.push_back(base_sup);
   inicializar(perfil, nperfiles);
}

//Constructor: crea el perfil original y llama a inicializar
// la base tiene el centro en el origen. Radio 1/4 y altura unidad
Cilindro2::Cilindro2 (const int num_verts_per, const unsigned nperfiles){
   std::vector<Tupla3f> perfil;

   ponerNombre("Cilindro2");

   for (int i = 0; i < num_verts_per; i++)
      perfil.push_back({0.25, (float)i/(num_verts_per-1), 0});

   inicializar(perfil, nperfiles);
}

// Constructor: crea el perfil original y llama a inicializar
// La base tiene centro en el origen. Radio y altura unidad
Cono::Cono (const int num_verts_per, const unsigned nperfiles){
   std::vector<Tupla3f> perfil;

   ponerNombre("Cono");
   
   Tupla3f base_inf(0,0,0);
   perfil.push_back(base_inf);

   for(int i = 0; i < num_verts_per; i++)
      perfil.push_back({1-(float)i/(num_verts_per-1),(float)i/(num_verts_per-1),0});

   inicializar(perfil, nperfiles);
}

// Constructor: crea el perfil original y llama a inicializar
// La base tiene centro en el origen. Radio unidad
Esfera::Esfera(const int num_verts_per,
          const unsigned nperfiles)
{
   ponerNombre("Esfera");

   std::vector<Tupla3f> perfil;

   for(int i = 0; i < num_verts_per; i++){
      float alpha = M_PI*((float)i/(num_verts_per-1)-(float)1/2);
      perfil.push_back({cos(alpha),sin(alpha),0});
   }
  
   inicializar(perfil, nperfiles);
}

// Constructor: crea el perfil original y llama a inicializar
// La base tiene centro en el origen. Radio unidad
// Se le pasa el identificador como parámetro
EsferaID::EsferaID(const int num_verts_per,
          const unsigned nperfiles, unsigned id)
{
   ponerNombre("EsferaID");
   ponerIdentificador(id);
   std::vector<Tupla3f> perfil;

   for(int i = 0; i < num_verts_per; i++){
      float alpha = M_PI*((float)i/(num_verts_per-1)-(float)1/2);
      perfil.push_back({cos(alpha),sin(alpha),0});
   }
  
   inicializar(perfil, nperfiles);
}
