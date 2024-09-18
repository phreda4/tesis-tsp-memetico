/////////////////////////////////////////////////////////////////////
//
//  Algoritmo Multiple Fragment
//
/////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include "aalgo.h"
#include "tspmf.h"

/////////////////////////////////////////////////////////////////////
void TSPMF::clear()
{
if (nodos!=NULL)
  delete [] nodos;
nodos=NULL;
}

/////////////////////////////////////////////////////////////////////
void TSPMF::init(PointSet *ps)
{
clear();
pset=ps;
nodos=new NodoMF[pset->cnt];
pq.init(pset->cnt);
}

/////////////////////////////////////////////////////////////////////
void TSPMF::run(Tour *tour)
{
int i,j;
for (i=0;i<pset->cnt;i++)		// Inicializar nodos[]
  {
  nodos[i].grado=0;				// Cantidad de arcos unidos
  nodos[i].desde=-1;				// Cola del subtour
  nodos[i].nn=pset->nn(i);			// Ciudad mas cercana
  pq.insert(pset->lastDist(),i);		// Insertar en PQ
  }
float dist;
int x,y;
for (i=0;i<pset->cnt-1;i++)
  {
  while (1) {
    pq.remove(&dist,&x);			// Sacar del PQ
    if (nodos[x].grado==2)			// ya esta en algun subtour
      continue;
    y=nodos[x].nn;
    if (nodos[y].grado<2 && y!=nodos[x].desde)	// No permitido
      break;
    if (nodos[x].grado==1) pset->deletePnt(nodos[x].desde);
    nodos[x].nn=pset->nn(x);				// Proximo mas cercano
    if (nodos[x].grado==1) pset->undeletePnt(nodos[x].desde);
    pq.insert(pset->lastDist(),x);		// Insertar en PQ
    }
  nodos[x].vecinos[nodos[x].grado++]=y;
  nodos[y].vecinos[nodos[y].grado++]=x;
  if (nodos[x].grado==2)
    {
    pset->deletePnt(x);
    if (nodos[y].grado==1)
      {
      nodos[y].desde=nodos[x].desde;
      nodos[nodos[x].desde].desde=y;
      }
    else
      {
      pset->deletePnt(y);
      nodos[nodos[y].desde].desde=nodos[x].desde;
      nodos[nodos[x].desde].desde=nodos[y].desde;
      }
    }
  else
    {
    if (nodos[y].grado==1)
      {
      nodos[x].desde=y;
      nodos[y].desde=x;
      }
    else
      {
      pset->deletePnt(y);
      nodos[x].desde=nodos[y].desde;
      nodos[nodos[y].desde].desde=x;
      }
    pset->deletePnt(nodos[x].desde);
    nodos[x].nn=pset->nn(x);			// Proximo mas cercano
    pset->undeletePnt(nodos[x].desde);
    pq.insert(pset->lastDist(),x);		// Insertar en PQ
    }
  ANIMA("%s add %i %i",tour->name,x,y);
  }
i=nodos[x].desde;       		// Ultima union
j=nodos[i].desde;
nodos[i].vecinos[1]=j;
nodos[j].vecinos[1]=i;
ANIMA("%s add %i %i",tour->name,j,i);
pset->undeleteAllPnt();			// Desborrar todos los puntos
toTour(tour);
}

/////////////////////////////////////////////////////////////////////
void TSPMF::runRandom(Tour *tour)
{
int i,j;
pq.clear();
for (i=0;i<pset->cnt;i++)		// Inicializar nodos[]
  {
  nodos[i].grado=0;				// Cantidad de arcos unidos
  nodos[i].desde=-1;				// Cola del subtour
  nodos[i].nn=pset->nn(i);			// Ciudad mas cercana
  pq.insert(pset->lastDist()*(.5+1.5*rand()/(RAND_MAX+1.0)),i);		// Insertar en PQ
  }
float dist;
int x,y;
for (i=0;i<pset->cnt-1;i++)
  {
  while (1) {
    pq.remove(&dist,&x);			// Sacar del PQ
    if (nodos[x].grado==2)			// ya esta en algun subtour
      continue;
    y=nodos[x].nn;
    if (nodos[y].grado<2 && y!=nodos[x].desde)	// No permitido
      break;
    if (nodos[x].grado==1) pset->deletePnt(nodos[x].desde);
    nodos[x].nn=pset->nn(x);				// Proximo mas cercano
    if (nodos[x].grado==1) pset->undeletePnt(nodos[x].desde);
    pq.insert(pset->lastDist()*(.5+1.5*rand()/(RAND_MAX+1.0)),x);		// Insertar en PQ
    }
  nodos[x].vecinos[nodos[x].grado++]=y;
  nodos[y].vecinos[nodos[y].grado++]=x;
  if (nodos[x].grado==2)
    {
    pset->deletePnt(x);
    if (nodos[y].grado==1)
      {
      nodos[y].desde=nodos[x].desde;
      nodos[nodos[x].desde].desde=y;
      }
    else
      {
      pset->deletePnt(y);
      nodos[nodos[y].desde].desde=nodos[x].desde;
      nodos[nodos[x].desde].desde=nodos[y].desde;
      }
    }
  else
    {
    if (nodos[y].grado==1)
      {
      nodos[x].desde=y;
      nodos[y].desde=x;
      }
    else
      {
      pset->deletePnt(y);
      nodos[x].desde=nodos[y].desde;
      nodos[nodos[y].desde].desde=x;
      }
    pset->deletePnt(nodos[x].desde);
    nodos[x].nn=pset->nn(x);			// Proximo mas cercano
    pset->undeletePnt(nodos[x].desde);
    pq.insert(pset->lastDist(),x);		// Insertar en PQ
    }
  ANIMA("%s add %i %i",tour->name,x,y);
  }
i=nodos[x].desde;       		// Ultima union
j=nodos[i].desde;
nodos[i].vecinos[1]=j;
nodos[j].vecinos[1]=i;
ANIMA("%s add %i %i",tour->name,j,i);
pset->undeleteAllPnt();			// Desborrar todos los puntos
toTour(tour);
}

/////////////////////////////////////////////////////////////////////
void TSPMF::toTour(Tour *tour)
{
int llast=0,last=0;
tour->init(pset);
for (int i=0;i<pset->cnt;i++)
  {
  tour->addPoint(last);
  if (nodos[last].vecinos[0]==llast)
    {
    llast=last;
    last=nodos[last].vecinos[1];
    }
  else
    {
    llast=last;
    last=nodos[last].vecinos[0];
    }   
  }
}
