// Nombre: José Miguel, Apellidos: González Cañadas, DNI/pasaporte: 77184606X (IG GIM+GIADE, curso 21-22)
// *********************************************************************
// ** Informática Gráfica 2021-2022
// ** Declaraciones de la clase modejo-jer.h
// *********************************************************************

#ifndef MODELO_JER_H
#define MODELO_JER_H

#include "grafo-escena.h"
#include "malla-ind.h"
#include "malla-revol.h"

class SillaArticulada : public NodoGrafoEscena{
   private:
      Matriz4f * rotacionRespaldo = nullptr;          // Matriz de rotación del respaldo
      Matriz4f * rotacionAsiento = nullptr;           // Matriz de rotación del asiento
      Matriz4f * traslacionSoporteAsiento = nullptr;  // Matriz de traslación del soporte del asiento
      Matriz4f * rotacionRueda = nullptr;             // Matriz de rotación de una rueda
      Matriz4f * traslacionSilla = nullptr;           // Matriz de traslación de la silla
      unsigned numParametros;

   public:
      SillaArticulada();
      unsigned leerNumParametros() const;
      void actualizarEstadoParametro(const unsigned iParam, const float t_sec);
};

class Respaldo : public NodoGrafoEscena{
   public:
      Respaldo();
};

class SoporteRespaldo : public NodoGrafoEscena{
   public:
      SoporteRespaldo();
};

class RuedaConectora : public NodoGrafoEscena{
   public:
      RuedaConectora(Matriz4f * &M);
};

class ConectorAsiento : public NodoGrafoEscena{
   public:
      ConectorAsiento(Matriz4f * &M);
};

class Asiento : public NodoGrafoEscena{
   public:
      Asiento(Matriz4f * &M);
};

class SoporteAsiento : public NodoGrafoEscena{
   public:
      SoporteAsiento();
};

class ConectorPata : public NodoGrafoEscena{
   public:
      ConectorPata(Matriz4f * &M);
};

class Pata : public NodoGrafoEscena{
   public:
      Pata(Matriz4f * &M);
};

class Rueda : public NodoGrafoEscena{
   public:
      Rueda(Matriz4f * &M);
};

#endif // MODELO_JER_H
