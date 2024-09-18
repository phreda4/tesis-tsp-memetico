/////////////////////////////////////////////////////////////////////
//
//  Clase 3 Opt
//
/////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include "pq.h"
#include "opt3.h"
#include "aalgo.h"

/////////////////////////////////////////////////////////////////////
Opt3::Opt3()
{
}

/////////////////////////////////////////////////////////////////////
Opt3::~Opt3()
{
}

/////////////////////////////////////////////////////////////////////
void Opt3::init(PointSet *ps)
{
pset=ps;
}

/////////////////////////////////////////////////////////////////////
// precondiciones:
//	pset sin puntos borrados  pset->undeleteAllPnt();
//	tour ya construido
void Opt3::run(Tour *t)
{
int cnt;
tour=t;

int city=0;
cnt=0;
do {
  if (opt3Search(city))
    {
    cnt=0;
//    printf("largo:%f\n",tour->lenght());
    }
  else
    cnt++;
  city=tour->next(city);
} while (cnt<pset->cnt);
}


/////////////////////////////////////////////////////////////////////
// las letras estan segun el paper de Jhonson
int Opt3::opt3Search(int cityA)
{
float ganAN,ganAP,ganAB;
int cityB,cityC,cityNow;

pset->deletePnt(cityA);
pset->deletePnt(tour->next(cityA));
pset->deletePnt(tour->prev(cityA));

ganAN=pset->dist(cityA,tour->next(cityA));
ganAP=pset->dist(cityA,tour->prev(cityA));

PQ<int> pq;
pq.init(50);	// 50 puntos a buscar como maximo
pq.clear();

cityNow=pset->nnBallFirst(cityA,ganAN);
while (cityNow!=-1) {
  pq.insert(pset->lastDist(),cityNow);
  cityNow=pset->nnBall();
  }

while (!pq.empty()) {
  pq.remove(&ganAB,&cityB);
  }


/*
// guarda la solucion
int a,b,c,d,e,f;
int mode;		// 0-2-opt 1-3-Opta 2-3-optb

int cityF,cityE,cityA,cityB,cityC,cityD;
int cityPrev,cityNow;

// arcos a borrar
float ganEF,ganAB,ganCD;

// arcos a agregar
float ganEB,ganFB,ganEC,ganFC,ganAD,ganAE;
float ganAux,ganMax;

PQ<int> pq;
pq.init(50);	// 50 puntos a buscar como maximo

ganMax=0;

cityE=tour->city(nodeE);
cityF=tour->city(nodeF);
ganEF=pset->calcDistC(cityE,cityF);

cityPrev=tour->city(tour->prev(nodeE));

pset->deletePnt(cityPrev);
pset->deletePnt(cityF);
pset->deletePnt(cityE);

pq.clear();
cityNow=pset->nnBallFirst(cityF,ganEF);
while (cityNow!=-1) {
  pq.insert(pset->lastDist(),cityNow);
  cityNow=pset->nnBall();
  }

while (!pq.empty()) {
  pq.remove(&ganFB,&cityB);
  cityA=tour->city(tour->next(tour->nodeOf(cityB)));
  ganAB=pset->calcDistC(cityA,cityB);
  ganEB=pset->calcDistC(cityE,cityB);

  // fijarse tambien 2-Opt
  ganAE=pset->calcDistC(cityA,cityE);
  ganAux=ganEF+ganAB-ganAE-ganFB;
  if (ganAux>ganMax)
    {
    ganMax=ganAux;
    mode=0;
    e=cityE;b=cityB;
    }

  pset->deletePnt(cityA);
  pset->deletePnt(cityB);

// elijo el mayor para encontrar todas las posibilidades
  ganAux=(ganEB<ganFB)?ganFB:ganEB;	
  cityD=pset->nnBallFirst(cityA,ganAB+ganEF-ganAux);
  while (cityD!=-1) {

    aalgo.out("tsp addTest %i %i",cityA,cityD);
    aalgo.out("tsp delTest %i %i",cityA,cityD);

    cityC=tour->city(tour->next(tour->nodeOf(cityD)));
    ganAD=pset->lastDist();
    ganCD=pset->calcDistC(cityC,cityD);
    ganEC=pset->calcDistC(cityC,cityD);
    ganFC=pset->calcDistC(cityC,cityD);

    if (ganEB+ganFC<ganEC+ganFB)
      {
      ganAux=ganAB+ganEF+ganCD- ganEB-ganFC -ganAD;
      if (ganAux>ganMax)
        {
        ganMax=ganAux;  
        mode=2;
        a=cityA;b=cityB;c=cityC;d=cityD;e=cityE;f=cityF;  
        }
      }
    else
      {
      ganAux=ganAB+ganEF+ganCD- ganEC-ganFB -ganAD;
      if (ganAux>ganMax)
        {
        ganMax=ganAux;
        mode=1;
        e=cityE;b=cityB;d=cityD;
        }
      }

    cityD=pset->nnBall();
    }

  pset->undeletePnt(cityA);
  pset->undeletePnt(cityB);
  }
  
pset->undeletePnt(cityPrev);
pset->undeletePnt(cityF);
pset->undeletePnt(cityE);

if (ganMax>0)	// se realiza un Opt
  {
  if (mode==0)
    {
    tour->flip(tour->nodeOf(e),tour->nodeOf(b));
    }
  else if (mode==1)
    {
    tour->flip(tour->nodeOf(e),tour->nodeOf(b));    
    tour->flip(tour->nodeOf(e),tour->nodeOf(d));
    }
  else
    {
    tour->flip(tour->nodeOf(e),tour->nodeOf(d));
    tour->flip(tour->nodeOf(c),tour->nodeOf(b));
    tour->flip(tour->nodeOf(f),tour->nodeOf(a));
    }
  return 1;
  }
*/
return 0;

}
