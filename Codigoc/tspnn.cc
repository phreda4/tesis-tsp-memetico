/////////////////////////////////////////////////////////////////////
//
//  Algoritmo Nearest Neighbor para aproximacion al TSP
//
/////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include "aalgo.h"
#include "tspnn.h"

/////////////////////////////////////////////////////////////////////
void TSPNN::init(PointSet *ps)
{
pset=ps;
}

/////////////////////////////////////////////////////////////////////
void TSPNN::run(Tour *tour,int last)
{
int i,tourLast,next;
tour->init(pset);
tour->addPoint(last);			// Primera ciudad
pset->deletePnt(last);			// Borrar la ciudad
for(i=1;i<pset->cnt;i++)
  {
  next=pset->nn(last);			// Obtiene el mas cercano
  ANIMA("%s add %i %i",tour->name,last,next);
  last=next;
  tour->addPoint(last);			// Agregarlo al tour
  pset->deletePnt(last);		// Borrar la ciudad
  }
ANIMA("%s add %i %i",tour->name,last,0);
pset->undeleteAllPnt();			// Desborrar todos los puntos
}
