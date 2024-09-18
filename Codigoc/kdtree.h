/////////////////////////////////////////////////////////////////////
//
//	Clase KdTree
//
////////////////////////////////////////////////////////////////////
#ifndef KD_TREE_H
#define KD_TREE_H

#include "fifo.h"
#include "pq.h"

#define CUTOFF		10	// cantidad de puntos en el bucket
#define	DIM		2	// dimencion de los puntos (2D)

class PointSet;
//
//  Nodo de kdtree
//
////////////////////////////////////////////////////////////////////
struct KdNode {
  int		bucket;		// 1=bucket
  int		cutdim;		// dimension del corte
  float		cutval;		// valor del corte
  union {
    KdNode	*loson;		// nodo inferior
    int		lopt;		// limites inferior
  };
  union {    
    KdNode	*hison;		// nodo superior
    int		hipt;		// limite superior
  };
  int           hiptn;          // limite superio con puntos borrados
  KdNode	*father;	// padre 
  };
//
//  Kdtree
//
////////////////////////////////////////////////////////////////////
class KdTree {
  friend class PointSet;
  private:
    PointSet	*pset;		// pointset
    KdNode	*root;		// raiz de Kdtree
    KdNode	**bucketptr;	// para acceder por ciudad
    int		*perm;
    int 	nntarget,nnptnum;
    float       nndist;
    
    Fifo<int>	ballList;	// lista de ciudades en radio dado 
    Fifo<float> ballListD;	// lista de distancias en radio dado
  public:
    KdTree();			// constructor
    ~KdTree();			// destructor

    void clear();		// limpiar
    void build(PointSet *ps);	// construir

    void deletePnt(int);        // borrar un punto
    void undeletePnt(int);      // desborrar un punto
    void undeleteAll();         // desborrar todo

    int nn(int);		// devuelve el vecino mas cercano
    
    int nnBallFirst(int,float);	// devuelve el primero de radio dado
    int nnBall();		// devuelve de a uno los siguientes
    int lastCity()		// ultima ciudad
      { return nnptnum; }
    float lastDist()		// ultima distancia
      { return nndist; }
    
    void nnBallPQ(int,float,PQ<int>&);  // devuelve el pq los vecinos
    
  private:
    KdNode *buildr(int,int);
    void select(int,int,int);
    int findMaxSpread(int l,int u);
    void rnn(KdNode *p);
    void rnnBall(KdNode *p);
    void rnnBallPQ(KdNode *p,PQ<int> &);
    void undeleteAll(KdNode *p);
  };

#endif

