/////////////////////////////////////////////////////////////////////
//
//  Gen.cc
//
/////////////////////////////////////////////////////////////////////
#ifndef GEN_H
#define GEN_H

#include "tourc.h"

//
//      Clase Individuo
//
/////////////////////////////////////////////////////////////////////
class Individuo {
public:
  Tour tour;

  Individuo() {}

  float fitness()
    { return -tour.lenght(); }
  void copyTour(Tour *t)
    { t->copyOf(tour); }

  void dpx(Individuo &g1,Individuo &g2)        // xover dpx
    { tour.dpx(g1.tour,g2.tour); }
  void dpxp(Individuo &g1,Individuo &g2)       // xover dpxp
    { tour.dpxp(g1.tour,g2.tour); }
  void amb(Individuo &g1,Individuo &g2)        // xover amb
    { tour.amb(g1.tour,g2.tour); }
  void bma(Individuo &g1,Individuo &g2)        // xover amb
    { tour.amb(g2.tour,g1.tour); }
  void inverover(Individuo &g1,Individuo &g2)  // inverover 
    { tour.inverover(g1.tour,g2.tour); }

  void mutate4r()              // mutar al azar
    { tour.mutate4r(); }
  void mutate4o()              // mutar buscando optimo
    { tour.mutate4o(); }

};

#endif
