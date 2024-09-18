/////////////////////////////////////////////////////////////////////
//
//  Optimizacion Lin-Kerningham e Iterated-Lin-Kernigham
//
/////////////////////////////////////////////////////////////////////
#ifndef ILK_H
#define ILK_H

#include "pointset.h"
#include "tourc.h"
#include "time.h"
#include "pq.h"

//
//  Nodo que guarda las informacion del flip
//
/////////////////////////////////////////////////////////////////////
struct NodeLk {
  int a,b,c,d;
  };

/////////////////////////////////////////////////////////////////////
struct NodeEdge {
  int a,b;
  NodeEdge() 		{}
  NodeEdge(int i,int j)	{ a=i;b=j; }
  };
  
/////////////////////////////////////////////////////////////////////
class Lk {
  private:
    PointSet *pset;		// Conjunto de puntos
    Tour *tour;			// tour actual

    int level;			// nivel de la recursion
    NodeLk *lkNow;		// apuntador al nodo actual de la recursion
    NodeLk *tryTour;	        // nodos para guardar informacion de cada nivel
    
    float lenghtIni;		// longitud inicial del tour
    PQ<int> *pqNow;		// apuntador al PQ actual de la recursion
    PQ<int> *tryPq;            	// colas de prioridad para ciudades

    int recursionMaxima;        // Maximo largo del tour
    int ciudadesMaxima;         // Maxima cantidad de puntos a buscar
    Time tIlk;                  // tiempo de corridad de ILK

  public:
    Lk() { tryTour=NULL;tryPq=NULL; };
    ~Lk() { clear(); };
    void clear();               // Limpiar variables
    void init(PointSet *);      // Inicializar algoritmo
    void set(char *);           // ajustar parametros
    void runIlk(Tour *);
    void runLk(Tour *);
  private:
    int lkSearch(int p) { return lkSearch(p,tour->next(p)); };
    int lkSearch(int,int);
    int searchNewPath(int a,int b);    
    void fillBestMoves(int cityFrom,float radius);
  };
  
#endif
