// Nombre: José Miguel, Apellidos: González Cañadas, DNI/pasaporte: 77184606X (IG GIM+GIADE, curso 21-22)
// *********************************************************************
// **
// ** Informática Gráfica
// ** Declaraciones de la clase latapeones.h
// **
// *********************************************************************

#ifndef LATAPEONES_H
#define LATAPEONES_H

#include "objeto3d.h"
#include "grafo-escena.h"


class SuperiorLata : public NodoGrafoEscena{
    public:
        SuperiorLata();
};

class CuerpoLata : public NodoGrafoEscena{
    public:
        CuerpoLata(const std::string & tex);
};

class InferiorLata : public NodoGrafoEscena{
    public:
        InferiorLata();
};

class Lata : public NodoGrafoEscena{
    public:
        Lata(const std::string & textura);
};

class LataCocaCola : public NodoGrafoEscena{
    public:
        LataCocaCola();
};

class LataPepsi : public NodoGrafoEscena{
    public:
        LataPepsi();
};

class LataUGR : public NodoGrafoEscena{
    public:
        LataUGR();
};

class PeonMadera : public NodoGrafoEscena{
    public:
        PeonMadera();
};

class PeonBlanco : public NodoGrafoEscena{
    public:
        PeonBlanco();
};

class PeonNegro : public NodoGrafoEscena{
    public:
        PeonNegro();
};

class Peones : public NodoGrafoEscena{
    public:
        Peones();
};

class LataPeones: public NodoGrafoEscena{
    public:
        LataPeones();
};

class Latas : public NodoGrafoEscena{
    public:
        Latas();
};

class VariasLatasPeones : public NodoGrafoEscena{
    public:
        VariasLatasPeones();
};

#endif
