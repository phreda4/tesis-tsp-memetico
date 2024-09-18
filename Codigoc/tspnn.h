/////////////////////////////////////////////////////////////////////
//
//  Algoritmo Nearest Neighbor para aproximacion al TSP
//
/////////////////////////////////////////////////////////////////////
#ifndef TSPNN_H
#define TSPNN_H

#include <stdlib.h>
#include "pointset.h"
#include "tourc.h"

/////////////////////////////////////////////////////////////////////
class TSPNN {
  private:
    PointSet *pset;	// Conjunto de puntos
  public:
    void init(PointSet *ps);
    void run(Tour *t,int);
    void run(Tour *t)
      { run(t,0); }
    void runRandom(Tour *t)
      { run(t,randomInt(0,pset->cnt-1)); }
  };

#endif
