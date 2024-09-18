/////////////////////////////////////////////////////////////////////
//
//  Tour con cache
//
/////////////////////////////////////////////////////////////////////
#ifndef TOUR_H
#define TOUR_H

#define MAXFLIPS 50
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "pointset.h"

/////////////////////////////////////////////////////////////////////
struct Flip {
  int from,to;		// desde y hasta
  int add;		// suma del intervalo
  float lenght;		// longitud anterior del tour
  int min,max;		

  int pmenor,pmayor,nmenor,nmayor;
  };

/////////////////////////////////////////////////////////////////////
class Tour {
  private:
    PointSet	*pset;
    
    int		*position;	// Arreglo de posiciones
    int		*tour;		// Arreglo de ciudades
    int		cntTour;	// cantidad de ciudades en el tour
    float       tourLenght;	// longitud total del tour
    
    int 	cntFlips;		// cantidad actual de inversiones
    Flip	flips[MAXFLIPS];	// Inversiones
    int		minimo;			// ultimo nivel del cache a comprobar si cambia sus punteros
    
    int		cntCache;       // Tamanio del cache
    int		*cacheC;        // Clave del cache
    float       *cacheV;        // Valor de la distancia
  public:
    char	name[10];	// Nombre del tour
    
    Tour()
      { tour=position=NULL;cacheC=NULL;cacheV=NULL;strcpy(name,"tsp"); }
    ~Tour()
      { clear(); }
      
    void	clear();                // limpiar instancia
    void	init(PointSet *s);      // Inicializar tour

    void        clearTour()             // limpiar tour
      { tourLenght=0;cntTour=0; }
    void 	setName(int i)
      { sprintf(name,"tsp%i",i); }

    void 	copyOf(Tour &);         // hacer copia de tour
    void	addPoint(int city);     // agregar ciudad al tour
        
    void 	flip(int,int,int,int);     // hacer un flip
    void 	flip2(int,int,int,int);     // hacer un flip
    void        removeLastFlip();	// eliminar el ultimo flip
    void        removeAllFlips()	// Eliminar todos los flips
      {
      if (cntFlips>0)
        tourLenght=flips[0].lenght;
      cntFlips=0;
      }    
    void 	makePermanent();           // hacer permanentes los flips
    void 	makePermanent2();           // hacer permanentes los flips
    void        doubleBridge(int,int,int,int);  // 4-opt

    int		nextS(int a)		// proxima ciudad sin cache
      {
      int p=position[a]+1;
      if (p==cntTour) p=0;
      return tour[p];
      }
    int 	next(int a)             // proxima ciudad de a
      {
      int p=toVirtual(position[a])+1;
      if (p==cntTour) p=0;
      return tour[toReal(p)];
      }
    int		prevS(int a)
      {
      int p=position[a]-1;
      if (p==-1) p=cntTour-1;
      return tour[p];
      }
    int		prev(int a)             // ciudad previa de a
      {
      int p=toVirtual(position[a])-1;
      if (p==-1) p=cntTour-1;
      return tour[toReal(p)];
      }    
    int 	between(int a,int b,int c)  // true si b esta entre a y c
      {
      int la=toVirtual(position[a]);
      int lb=toVirtual(position[b]);
      int lc=toVirtual(position[c]);
      return ((la<lb && lb<lc) || (lc<la && la<lb) || (lb<lc && lc<la));
      }
    float       dist(int a,int b)       // distancia entre a y b
      { 
      int i=a^b;
      if (a>b)
        {
        if (cacheC[i]==b)
          return cacheV[i];
        cacheC[i]=b;
        cacheV[i]=pset->dist(a,b);
        }
      else
        {
        if (cacheC[i]==a)
          return cacheV[i];
        cacheC[i]=a;
        cacheV[i]=pset->dist(a,b);
        }
      return cacheV[i];
      }
    float 	lenght()                // longitud del tour
      { return tourLenght; }
    int         intLenght();
    void	print();                // imprimir el tour
    void        save(char *);           // Grabar tour

    int city(int p)                     // ciudad de la posicion p
      { return toVirtual(position[p]); }
    int toVirtual(int i);               // transformar posicion a virtual
    int toReal(int i);                  // transformar posicion a real

    int toVirtual2(int i);               // transformar posicion a virtual
    int toReal2(int i);                  // transformar posicion a real

    void dpx(Tour &,Tour &);            // DPX
    void dpxp(Tour &,Tour &);            // DPXp
    void amb(Tour &,Tour &);            // A-B
    void inverover(Tour &,Tour &);  // Xover de Guo Taoy Michalewicz

    void mutate4r();
    void mutate4o();

    void test();
  private:
    void flipPhis(int from,int to);     // flip real en el tour
    void flipPhisI(int from,int to);    // flip inverso    
  };

#endif
