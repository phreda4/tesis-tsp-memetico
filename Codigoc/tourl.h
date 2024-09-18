/////////////////////////////////////////////////////////////////////
//
//  Tour de listas doblemente vinculadas con 
//  intervalos de inversiones
//
/////////////////////////////////////////////////////////////////////
#ifndef TOUR_H
#define TOUR_H

#include <stdio.h>
#include <stdlib.h>
#include "pointset.h"

#define MAXFLIPS 50	// cantidad maxima de flips permitidos

/////////////////////////////////////////////////////////////////////
struct Flip {
  int a,b,ab;		// limite inferior, superior y suma
  int l,e,g;		// proximo si es menor, igual o mayor al interv.
  };

/////////////////////////////////////////////////////////////////////
struct TourNode {
  int 		pos;	// posicion en el tour
  int		n[2];	// ciudades vecinas
  float		d[2];	// distancias hacia las vecinas
  };
    
/////////////////////////////////////////////////////////////////////
class Tour {
  private:
    PointSet	*pset;
    
    int		*tour;		// Arreglo de posiciones
    TourNode 	*city;		// Arreglo de ciudades
    int		cntTour;	// cantidad de ciudades en el tour
    float	tourLenght;	// longitud total del tour  
    
    int 	cntFlips;    
    Flip	flips[MAXFLIPS];
  public:
    Tour()
      {
      tour=NULL;
      city=NULL;
      }
    ~Tour()
      {
      clear();
      }
      
    void	clear();
    void	init(PointSet *s);

    void 	copyOf(Tour &);
    void	addPoint(int city);
        
    void 	flip(int a,int b,int c,int d);
    void 	makePermanent();
    
    int 	next(int a)
      { 
      int ai=0;
      TourNode *na=&city[a];
      toVirtual(na->pos,ai);
      return na->n[1^ai];
      }
    int		prev(int a)
      { 
      int ai=0;
      TourNode *na=&city[a];
      toVirtual(na->pos,ai);
      return na->n[0^ai];
      }    
    int 	between(int a,int b,int c)
      {
      int aux;	// no importa
      int la=toVirtual(city[a].pos,aux);
      int lb=toVirtual(city[b].pos,aux);
      int lc=toVirtual(city[c].pos,aux);
      return ((la<lb && lb<lc) || (lc<la && la<lb) || (lb<lc && lc<la));
      }
    
    float	dist(int a,int b)
      {
      TourNode *na=&city[a];
      return (na->n[0]==b)?na->d[0]:na->d[1];
      }
    float	lenght()
      { 
      return tourLenght; 
      }
    void	print();
    void 	test();
  private:
    int		toVirtual(int,int&);
  };

#endif
