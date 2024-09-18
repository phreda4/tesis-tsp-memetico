/////////////////////////////////////////////////////////////////////
//
//  Optimizacion 3-Opt
//
/////////////////////////////////////////////////////////////////////
#ifndef OPT3_H
#define OPT3_H

#include "pointset.h"
#include "kdtree.h"
#include "tourc.h"

class Opt3 {
  private:
    PointSet *pset;
    KdTree *kd;
    Tour *tour;
  public:
    Opt3();
    ~Opt3();
    void init(PointSet *);
    void run(Tour *);
  private:
    int opt3Search(int);
  };
  
#endif
