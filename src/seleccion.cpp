// Nombre: José Miguel, Apellidos: González Cañadas, DNI/pasaporte: 77184606X (IG GIM+GIADE, curso 21-22)
#include "ig-aux.h"
#include "tuplasg.h"
#include "practicas.h"
#include "camara.h"
#include "fbo.h"
#include "seleccion.h"

// framebuffer object usado para selección
static Framebuffer * fbo = nullptr ;

// ---------------------------------------------------------------------

Framebuffer * FBOActual()
{
   return fbo ;
}

// ---------------------------------------------------------------------
// fijar el color actual en el cauce usando un identificador entero

void FijarColVertsIdent( Cauce & cauce, const int ident )  // 0 ≤ ident < 2^24
{
   // COMPLETAR: práctica 5: fijar color actual de OpenGL usando 'ident' (glColor3ub)
   // .....
   const unsigned char byteR = (ident) % 0x100U;      // rojo, byte menos significativo
   const unsigned char byteG = (ident / 0x100U ) % 0x100U;      // verde, byte intermedio
   const unsigned char byteB = (ident / 0x10000U ) % 0x100U;      // azul, byte mas significativo
   glColor3ub(byteR, byteG, byteB);
}

// ----------------------------------------------------------------------------------
// leer un identificador entero codificado en el color de un pixel en el
// framebuffer activo actualmente

int LeerIdentEnPixel( int xpix, int ypix )
{
   // COMPLETAR: práctica 5: leer el identificador codificado en el color del pixel (x,y)
   // .....(sustituir el 'return 0' por lo que corresponda)
   // .....

   unsigned char bytes[3];
   glReadPixels( xpix,ypix, 1,1, GL_RGB,GL_UNSIGNED_BYTE, (void *)bytes);
   return bytes[0] + ( 0x100U*bytes[1] ) + ( 0x10000U*bytes[2] );
}

// -------------------------------------------------------------------------------
// Función principal de selección, se llama al hacer click con el botón izquierdo
//
// (x,y)       = posición donde se ha hecho click en coordenadas del sistema de ventanas (enteras)
// objeto_raiz = objeto raiz (no subobjeto de otro) que se está visualizando en la ventana
// cv_dib      = contexto de visualización usado para dibujar el objeto
//
// devuelve: true si se ha seleccionado algún objeto, false si no

bool Seleccion( int x, int y, Escena * escena, ContextoVis & cv_dib )
{
   using namespace std ;
   assert( escena != nullptr );

   // COMPLETAR: práctica 5:
   // Visualizar escena en modo selección y leer el color del pixel en (x,y)
   // Se deben de dar estos pasos:

   //cout << "Seleccion( x == " << x << ", y == " << y << ", obj.raíz ==  " << objeto_raiz.leerNombre() << " )" << endl ;

   // 1. Crear (si es necesario) y activar el framebuffer object (fbo) de selección
   // .........
   if(fbo == nullptr)
     fbo = new Framebuffer(cv_dib.ventana_tam_x, cv_dib.ventana_tam_y);

   // 2. crear un 'ContextoVis' apropiado, en ese objeto:
   //    * activar modo selecion, desactivar iluminación, poner modo relleno
   //    * usar el mismo cauce, y la misma cámara que en 'cv_dib'
   //    * fijar el tamaño de la ventana igual que en 'cv_dib'
   //
   // ..........
   ContextoVis cv(cv_dib);
   cv.modo_seleccion = true;
   cv.iluminacion = false;
   cv.modo_visu = ModosVisu::relleno;

   FijarColVertsIdent(*cv.cauce_act, 0);

   // 3. Activar fbo, cauce y viewport. Configurar cauce (modo solido relleno, sin ilum.
   //    ni texturas). Limpiar el FBO (color de fondo: 0)
   // .......
   fbo->activar(cv_dib.ventana_tam_x, cv_dib.ventana_tam_y);

   cv.cauce_act->activar();
   cv.cauce_act->fijarEvalMIL(false);

   glViewport(0, 0, cv.ventana_tam_x, cv.ventana_tam_y);
   glClearColor(0, 0, 0, 1);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   // 4. Activar la cámara (se debe leer de la escena con 'camaraActual')
   // ....
   CamaraInteractiva * cam = escena->camaraActual();
   cam->activar(*cv.cauce_act);

   // 5. Visualizar el objeto raiz actual (se debe leer de la escena con 'objetoActual()')
   // ........
   Objeto3D * objeto_raiz_act = escena->objetoActual();
   objeto_raiz_act->visualizarGL(cv);

   // 6. Leer el color del pixel (usar 'LeerIdentEnPixel')
   // (hay que hacerlo mientras está activado el framebuffer de selección)
   // .....
   int id = LeerIdentEnPixel(x, y);

   // 7. Desactivar el framebuffer de selección
   // .....
   fbo->desactivar();

   // 8. Si el identificador del pixel es 0, imprimir mensaje y terminar (devolver 'false')
   // ....
   if (id == 0) {
      cout << "Miramos hacia el origen" << endl;
      cam->mirarHacia({0.0,0.0,0.0});
      return true;
   }

   // 9. Buscar el objeto en el objeto_raiz (puede ser un grafo de escena)
   //    e informar del nombre del mismo (si no se encuentra, indicarlo)
   //   (usar 'buscarObjeto')
   // .....

   Objeto3D * objeto;
   Tupla3f centro;
   if(objeto_raiz_act->buscarObjeto(id, MAT_Ident(), &objeto, centro)){
      cam->mirarHacia(centro);
      cout << "Objeto con id " << id << " seleccionado: " + objeto->leerNombre() << endl;
      
      // Ejercicios adicionales 1 y 2 de la P5
      /*if (objeto->leerNombre() == "EsferaID"){
         unsigned columna = (objeto->leerIdentificador()-1)%5+1;
         unsigned fila = (objeto->leerIdentificador()-1)/5+1;
         cout << "Se ha seleccionado la esfera número " << columna << " de la fila número " << fila << endl;
      }*/
      if (objeto->leerIdentificador() == 100){
         cout << "Este (X+)" << endl;
      }
      else if (objeto->leerIdentificador() == 101){
         cout << "Norte (Z-)" << endl;
      }
      else if (objeto->leerIdentificador() == 102){
         cout << "Oeste (X-)" << endl;
      }
      else if (objeto->leerIdentificador() == 103){
         cout << "Sur (Z+)" << endl;
      }
   }
   else{
      cout << "Nada" << endl;
      return false;
   }
   // al final devolvemos 'true', ya que hemos encontrado un objeto
   return true ;
}
