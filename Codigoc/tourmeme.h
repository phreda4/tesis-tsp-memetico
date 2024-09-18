/////////////////////////////////////////////////////////////////////
//
//  Clase para almacenar un tour en un meme
//
/////////////////////////////////////////////////////////////////////
#ifndef TOURMEME_H
#define TOURMEME_H

#include <stdlib.h>
#include "pointset.h"
#include "tour.h"

/////////////////////////////////////////////////////////////////////
struct NodoTM {
  int	desde;		// estos campos se utilizan en la clase
  int	nn;		// superior (y eso esta mal)
  
  char	grado;  	// cantidad de arcos que salen de el
  int	vec[2]; 	// vec[0]=prox, vec[1]=ant
  };
  
/////////////////////////////////////////////////////////////////////
class TourMeme {
  protected:
    static PointSet	*pset;    
      
    NodoTM	*nodos;
    int		arcosUnidos;
    float	distanciaArcos;
    float	longitudMinima;
    char 	name[10];
  public:
    TourMeme()
      { nodos=NULL; }
    ~TourMeme()
      { clear(); }
    void initClass(PointSet *ps);
    void init(char *n);
          
    void clear();
    void clearTour();    

    void copyOf(TourMeme &t);
    
    void addEdge(int,int);
    void delEdge(int,int);

    void addEdge2(int,int);	// sin control de puntos
    void delEdge2(int,int);	// sin control de puntos
    
    float lenght()	{ return distanciaArcos; }
    int cntArcos()	{ return arcosUnidos; }

    void buildMF(int maxGen);
    void xover(TourMeme &tf1,TourMeme &tf2,int maxGen);
    void localSearch();
    
    void mutation();
        
    void deTour(Tour &tour);
    void aTour(Tour &tour);
    void print();

    int completo()
      { return (arcosUnidos==pset->cnt); }
  private:
    int optSearch(int,int);
    void error(int);  
  };

#endif
