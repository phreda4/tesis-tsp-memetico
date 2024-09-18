/////////////////////////////////////////////////////////////////////
//
//  Clase para resolver el TSP por SFC
//
/////////////////////////////////////////////////////////////////////
#ifndef TSPSFC_H
#define TSPSFC_H

#include "pointset.h"
#include "tourc.h"

/////////////////////////////////////////////////////////////////////
//  Clase para obtener un recorrido de tsp 
/////////////////////////////////////////////////////////////////////
class TSPSFC {
  private:
    PointSet *pset;
    float minx,maxx,miny,maxy;		// max y min de las ciudades p/cal
    float cenx,ceny;			// centro de las ciudades cargadas
    
    // Metodos    
    int sierpinskiIndex(int x,int y);	// indice de la curva
    void limits();    		// calcular parametros
  public:
    void init(PointSet *ps);
    void run(Tour *tour,float a);
  };  
  
#endif
