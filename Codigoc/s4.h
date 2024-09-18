/////////////////////////////////////////////////////////////////////
//
//  Busqueda de movimientos 4 opt
//
/////////////////////////////////////////////////////////////////////
#ifndef S4_H
#define S4_H

#include "pointset.h"
#include "tourc.h"
#include "time.h"
#include "pq.h"

/////////////////////////////////////////////////////////////////////
class Search4 {
  private:
    PointSet *pset;		// Conjunto de puntos
    Tour *tour;			// tour actual

    float lenghtIni;		// longitud inicial del tour
    PQ<int> *listas;           	// colas de prioridad para ciudades
    int     cntCitys;           // cantidad de ciudades del PQ

    int cntMaxima;         // Maxima cantidad de puntos a buscar
    Time tIlk;                  // tiempo de corridad de ILK
    float fSize;		// Tamanio de la PQ para Flk

  public:
    Search4() { listas=NULL; };
    ~Search4() { clear(); };
    void clear();               // Limpiar variables
    void init(PointSet *);      // Inicializar algoritmo
    void set(char *);           // ajustar parametros
    void runS4(Tour *);
  private:
    void check(int a1,int b1,int a,int b);
  };
  
#endif
