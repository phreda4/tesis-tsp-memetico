/////////////////////////////////////////////////////////////////////
//
//  Algoritmo Memetico para el problema del TSP
//
/////////////////////////////////////////////////////////////////////
#ifndef TSPMEME_H
#define TSPMEME_H

#include "pointset.h"
#include "tourc.h"
#include "gen.h"
#include "time.h"

#include "tspnn.h"
#include "tspmf.h"
#include "tspsfc.h"
#include "lk.h"
#include "opt2.h"

/////////////////////////////////////////////////////////////////////
class TSPMEME {
  private:
    PointSet *pset;
    Individuo   *population;             // poblacion
    int   *sortPop;                 // poblacion ordenada

    float bestFitness;             // mejor individuo
    float mutationRate;            // tasa de mutacion
    float diversification;         // diversificacion de la poblacion

    int popCnt;                         // cantidad de genes
    Time timeRun;                       // tiempo de corrida

    int bestIndividuo;		// mejor individuo
    float bestIndividuoF;	// valor
    
    Lk          lk;
    Opt2        opt2;
    TSPSFC      tspsfc;
    TSPMF       tspmf;
    TSPNN       tspnn;

    void (TSPMEME::*frun)(Tour*);
    void (TSPMEME::*fborn)();                 // funcion de nacimiento
    void (TSPMEME::*fchoose)(int*,int*,int*); // funcion de eleccion
    void (TSPMEME::*fxover)(int,int,int);     // funcion de crossover
    void (TSPMEME::*fmutat)(int);             // funcion de mutacion
    void (TSPMEME::*flsear)(int);             // funcion de busqueda local

  public:
    TSPMEME() {};
    ~TSPMEME() {};

    void init(PointSet *ps);            // inicializar memetico
    void set(char *arg);                // ajustar variables

    void run(Tour *tour);               // correr algoritmo
    void run1(Tour *tour);               // correr algoritmo 1
    void run2(Tour *tour);               // correr algoritmo 2
    void run3(Tour *tour);               // correr algoritmo 3
    
    void sortPopulation();              // Ordenar poblacion

    void bornSFC();                     // nacimiento por SFC
    void bornMF();                      // nacimiento por MF
    void bornNN();                      // nacimiento por NN

    void choose0(int*,int*,int*);       // Eleccion por ruleta y el peor
    void choose1(int*,int*,int*);       // Eleccion de mejor, cualquiera,peor
    void choose2(int*,int*,int*);       // Eleccion de cualquiera*3
    void choose3(int*,int*,int*);       // Eleccion de peor, cualquiera*2
    void choose4(int*,int*,int*);

    void xoverDPX(int,int,int);         // xover DPX (parent,parent->child)
    void xoverINV(int,int,int);
    void xoverDPXP(int,int,int);         // xover DPX (parent,parent->child)    void xoverAMB(int,int,int);         // xover A-B
    void xoverAMB(int,int,int);         // xover A-B
    void xoverBMA(int,int,int);         // xover B-A

    void mutation4r(int);            // Mutacion 4-opt al azar (like ILK)
    void mutation4o(int);            // 4-opt search (idea de Glover)

    void lsearch2(int);
    void lsearchLk(int);
    void lsearch2lk(int);
  };


#endif

