/////////////////////////////////////////////////////////////////////
//
//  Optimizacion 2-Opt
//
/////////////////////////////////////////////////////////////////////
#ifndef OPT2_H
#define OPT2_H

#include "pointset.h"
#include "tourc.h"

class Opt2 {
  private:
    PointSet *pset;
    Tour *tour;
  public:
    void init(PointSet *p)
      { pset=p; }
    void run(Tour *);
  private:
    int opt2Search(int);
  };
  
#endif
