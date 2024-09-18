/////////////////////////////////////////////////////////////////////
//
//  Algoritmo Multiple Fragment para aproximacion al TSP
//
/////////////////////////////////////////////////////////////////////
#ifndef TSPMF_H
#define TSPMF_H

#include <stdlib.h>

#include "pointset.h"
#include "tourc.h"
#include "pq.h"

/////////////////////////////////////////////////////////////////////
struct NodoMF {
  int desde;
  char grado;
  int nn;
  int vecinos[2]; 
  };
  
/////////////////////////////////////////////////////////////////////
class TSPMF {
  private:
    PointSet *pset;
    NodoMF *nodos;
    PQ<int> pq;
  public:
    TSPMF()
      { nodos=NULL; }
    ~TSPMF()
      { clear(); }
    void clear();
    void init(PointSet *ps);
    void run(Tour *);
    void runRandom(Tour *);
    void toTour(Tour *t);
  };

#endif
