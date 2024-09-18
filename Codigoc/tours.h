/////////////////////////////////////////////////////////////////////
//
//
//
/////////////////////////////////////////////////////////////////////
#ifndef TOUR_H
#define TOUR_H

#define MAXFLIPS 50

#include <stdio.h>
#include <stdlib.h>
#include "pointset.h"

/////////////////////////////////////////////////////////////////////
struct Segment {
  int from,to;
  char invert;
  Segment *prev,*next;  
  
  Segment(int a,int b,int i)
    {
    from=a;to=b;invert=i;prev=next=NULL;
    }
  };
  
/////////////////////////////////////////////////////////////////////
class Tour {
  private:
    PointSet	*pset;
    
    int		*position;	// Arreglo de posiciones
    int		*tour;		// Arreglo de ciudades
    int		cntTour;	// cantidad de ciudades en el tour
    float	tourLenght;	// longitud total del tour  
    
    int 	maxN;		// cantidad de nodos
    Segment 	*root;		// raiz de la lista
    
    int 	maxSeg;		// cantidad maxima de segmentos
    int 	cntSeg;		// cantidad actual de segmentos
    Segment 	**sortS;	// segmentos ordenados 
  public:
    Tour()
      {
      root=NULL;
      sortS=NULL;
      tour=position=NULL;
      }
    ~Tour()
      {
      clear();
      }
      
    void	clear();
    void	init(PointSet *s);

    void 	copyOf(Tour &);
    void	addPoint(int city);
        
    void 	flipPermanent(int a,int b,int c,int d);
    void 	flip(int a,int b,int c,int d);
    void 	makePermanent();
    
    int 	next(int a);
    int		prev(int a);
    int 	between(int a,int b,int c);
    
    float	dist(int a,int b);
    float	lenght()
      { return tourLenght; }
    void	print();
  private:
    Segment	*findS(int a);
    int 	findN(int a);
    void 	insSort(Segment *s,int n);
    void 	delSort(int n);
  };

#endif