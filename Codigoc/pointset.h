////////////////////////////////////////////////////////////////////
//
//  Point Set
//
////////////////////////////////////////////////////////////////////
#ifndef POINTSET_H
#define POINTSET_H
#include "kdtree.h"
//
//  Clase Point
//
////////////////////////////////////////////////////////////////////
class Point {
  public:
     float x,y;
     char del,look;		// informacion de la ciudad (bits)
  };
//
//  Clas PointSet
//
////////////////////////////////////////////////////////////////////
class PointSet {
  private:
    KdTree	kdtree;		// Kdtree de los puntos
    Point	*point;		// Array de puntos
    int		cntCache;	// Tamanio del cache
    int		*cacheC;	// Info del cache
    float 	*cacheV;  	// Valores del cache
    int		(PointSet::*distanceInt)(int,int);  // Funcion distancia~
  public:
    char	name[60];	// nombre de la instancia
    int		cnt;	// cantidad de puntos
    int		cntDelete;	// cantidad de puntos borrados
    float	xmin,xmax,ymin,ymax;	// Limites

    PointSet();			// constructor
    ~PointSet()			// destructor
      { clear(); }

    void clear();		// limpiar
    void size(int);		// definir cantidad de puntos
    void load(char *);		// cargar de archivo
    void save(char *);          // grabar a archivo

    int addPoint(int,float,float); // agregar punto int

    int nnOld(int nro);		// nn por fuerza bruta
    int nn(int n)		// devuelve el vecino mas cercano
      { return kdtree.nn(n); }
    void nnBallPQ(int n,float v,PQ<int>&p)  // devuelve el pq los vecinos
      { kdtree.nnBallPQ(n,v,p); }
    int nnBallFirst(int n,float v)	// devuelve el primero de radio dado
      { return kdtree.nnBallFirst(n,v); }
    int nnBall()		// devuelve de a uno los siguientes
      { return kdtree.nnBall(); }
    int lastCity()		// ultima ciudad
      { return kdtree.lastCity(); }
    float lastDist()		// ultima distancia
      { return kdtree.lastDist(); }
//
//  Control de puntos borrados
//
    void deletePnt(int nro)
      { point[nro].del=1;cntDelete++;kdtree.deletePnt(nro); }
    void undeletePnt(int nro)
      { point[nro].del=0;cntDelete--;kdtree.undeletePnt(nro); }
    int deleted(int nro)
      { return (point[nro].del==1); }
    void undeleteAllPnt();	// desborrar todos los puntos
    void unlookAllPnt();        // desmarcar todos los puntos
//
//  Control de puntos con don't look
//
    void dontlookPnt(int nro)
      { point[nro].look=1; }
    void lookPnt(int nro)
      { point[nro].look=0; }
    int look(int nro)
      { return (point[nro].look==1); }
//
//  Informacion
//
    float x(int nro)
      { return point[nro].x; }
    float y(int nro)
      { return point[nro].y; }
    float px(int nro,int d)
      { return (d==0)?point[nro].x:point[nro].y; }
//
//  Calculo de distancias
//
    int distInt(int i,int j)            // Distancia entera para resultado
      { return (this->*distanceInt)(i,j); }
    float calcDist(int i,int j)		// Distancia de punto flotante
      { 
      float dx=point[i].x-point[j].x;
      float dy=point[i].y-point[j].y;
      return hypot(dx,dy);
      }
    float dist(int c1,int c2)           // Distancia con cache
      {
      int indice=c1^c2;		// dispersion en cache
      if (c1<c2)
        {
        if (cacheC[indice]!=c1) 	// consultar si esta en cache
          {
          cacheC[indice]=c1;
          cacheV[indice]=calcDist(c1,c2);
          }
        }
      else
        {
        if (cacheC[indice]!=c2) 	// consultar si esta en cache
          {
          cacheC[indice]=c2;
          cacheV[indice]=calcDist(c1,c2);
          }
        }
      return cacheV[indice];
      }
    int distIntMax(int,int);		// distancia Max
    int distIntMan(int,int);   		// distancia Man
    int distIntEuc(int,int);   		// distancia Euc
    int distIntAtt(int,int);   		// distancia Att
    int distIntGeo(int,int);		// distancia Geo

  };
#endif