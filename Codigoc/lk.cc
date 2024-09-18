/////////////////////////////////////////////////////////////////////
//
//  Clase optimizador Lin-Kerningham
//
/////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <string.h>
#include "lk.h"
#include "aalgo.h"

/////////////////////////////////////////////////////////////////////
void Lk::clear()
{
if (tryPq!=NULL)
  {
  for (int i=0;i<recursionMaxima;i++)
    tryPq[i].free();
  delete []tryPq;
  tryPq=NULL;
  }
if (tryTour!=NULL)
  {
  delete []tryTour;
  tryTour=NULL;
  }
}
/////////////////////////////////////////////////////////////////////
void Lk::set(char *arg)
{
clear();
int t;
if (*arg!=0 && *arg!='+')
  {
  char *tok;      // parse de opciones
  for (tok=strtok(arg," ,");tok;tok=strtok(0," ,"))
    {
    if (!strncasecmp(tok,"r:",2))
      sscanf(tok,"r:%i",&recursionMaxima);
    else if (!strncasecmp(tok,"c:",2))
      sscanf(tok,"c:%i",&ciudadesMaxima);
    else if (!strncasecmp(tok,"t:",2))
      {
      sscanf(tok,"t:%i",&t);
      tIlk.set(t,0);
      }
   }
  }
tryTour=new NodeLk[recursionMaxima];
tryPq=new PQ<int>[recursionMaxima];
for (int i=0;i<recursionMaxima;i++)
  tryPq[i].init(ciudadesMaxima);
}

/////////////////////////////////////////////////////////////////////
void Lk::init(PointSet *ps)
{
pset=ps;

recursionMaxima=5;	// Maximo largo del tour default
ciudadesMaxima=20;	// valor default
tIlk.set(300,0);        // 5 minutos de ilk por default
}
//
//  Lk
//
/////////////////////////////////////////////////////////////////////
void Lk::runIlk(Tour *t)
{
//Time tStart,tNow;
runLk(t);					//  Primera busqueda
Tour aux;
aux.init(pset);
aux.copyOf(*t);
//t->copyOf(aux);
//tStart.now();
int cnt=0;
do {                    // Iteracion
  t->mutate4r();
  runLk(t);
  if (t->lenght()<aux.lenght())
    {
    aux.copyOf(*t);
//    printf("Longitud:%f %i\n",tour->lenght(),cnt);
    cnt=0;
    }
  else
    {
    t->copyOf(aux);
    cnt++;
    }
//printf("%i\r",cnt);fflush(stdout);
//  tNow.now();
} while (cnt<150);
//} while(tNow-tStart<tIlk);
}
//
//  Lk simple
//
/////////////////////////////////////////////////////////////////////
void Lk::runLk(Tour *t)
{
tour=t;

int cnt=0;                      // cantidad de ciudades probadas
int cityA=0;
pset->unlookAllPnt();
do {
  if (!pset->look(cityA))       // si no se busco antes
    {
    if (lkSearch(cityA))        // buscar una optimizacion
      cnt=0;                    // se encontro, volver contador a 0
    else
      { cnt++;pset->dontlookPnt(cityA);}  // marcar visitada con fracaso
    }
  else
    cnt++;                      // no buscar en esta ciudad
  cityA=tour->next(cityA);      // siguiente ciudad
} while (cnt<pset->cnt);  // mientrar no se haya completado una vuelta sin opt
}
/////////////////////////////////////////////////////////////////////
int Lk::lkSearch(int cityP,int cityNP)
{
lenghtIni=tour->lenght()*.995;		//  Longitud inicial del tour
pset->deletePnt(cityP);
pset->deletePnt(cityNP);
lkNow=tryTour;				//  Comienzo de arreglo de movimientos
pqNow=tryPq;
level=0;
int result=searchNewPath(cityP,cityNP);	//  Comienzo de la recursion
pset->undeletePnt(cityNP);
pset->undeletePnt(cityP);
return result;
}

/////////////////////////////////////////////////////////////////////
int Lk::searchNewPath(int cityP,int cityNextP)
{
float distPA;						// Distancia entre la ciudad P y la ciudad A
int cityA,cityNextA;					//  ciudad A y ciudad siguiente a A (NextA)
lkNow->a=cityP;						// Guardar el arco implicado
lkNow->b=cityNextP;
int direccion=(cityNextP==tour->next(cityP))?0:1;
fillBestMoves(cityP,					//  Llenar nowTour[level] con las ciudades a probar
    tour->dist(cityP,cityNextP)-(tour->lenght()-lenghtIni));
if (level<recursionMaxima-1)                            // si continua recursion...
  {
  while (!pqNow->empty()) {				//  Mientras haya ciudades...
    pqNow->remove(&distPA,&cityA);			//  Sacar una ciudad del PQ
  
    if (direccion==0)					//  Elegir la ciudad que deje
      cityNextA=tour->next(cityA);			// abierto el ciclo
    else
      cityNextA=tour->prev(cityA);
    lkNow->c=cityA;
    lkNow->d=cityNextA;
    if (direccion==0)                                     // segun direccion
      tour->flip(cityP,cityNextP,cityA,cityNextA);        // hacer el flip
    else
      tour->flip(cityNextP,cityP,cityNextA,cityA);
    if (tour->lenght()<lenghtIni) // es mejor la nueva solucion ?
      {
      if (aalgo.run())            // si estoy animando el algoritmo
        {
        int i;
        NodeLk *n=tryTour;        // grabar cada movimiento
        for (i=0;i<=level;i++,n++)
          ANIMA("%s opt2 %i %i %i %i",tour->name,n->a,n->b,n->c,n->d);
        }
      tour->makePermanent();      // fijar el cambio
      return 1;                   // Se logro una optimizacion
      }
    else
      {
      pset->deletePnt(cityA);
      lkNow++;pqNow++;level++;
      int aux=searchNewPath(cityNextA,cityNextP);
      level--;pqNow--;lkNow--;
      pset->undeletePnt(cityA);      
      if (aux==0)                    // si no se optimizo
        tour->removeLastFlip();      // borrar el ultimo cambio
      else                           // si se optimizo
        {
        pset->lookPnt(cityA);      // marcar las ciudades
        pset->lookPnt(cityNextA);  // que pueden tener mas optimizaciones
        return 1;
        }
      }
    }
} else {
  while (!pqNow->empty()) {		//  Mientras haya ciudades...
    pqNow->remove(&distPA,&cityA);	//  Sacar una ciudad del PQ
  
    if (direccion==0)			//  Elegir la ciudad que deje
      cityNextA=tour->next(cityA);	// abierto el ciclo
    else
      cityNextA=tour->prev(cityA);
    lkNow->c=cityA;
    lkNow->d=cityNextA;
    if (direccion==0)                              // segun direccion
      tour->flip(cityP,cityNextP,cityA,cityNextA); // hacer el flip
    else
      tour->flip(cityNextP,cityP,cityNextA,cityA);
    if (tour->lenght()<lenghtIni) // es mejor la nueva solucion ?
      {
      if (aalgo.run())            // si estoy animando el algoritmo
        {
        int i;
        NodeLk *n=tryTour;        // grabar cada movimiento
        for (i=0;i<=level;i++,n++)
          ANIMA("%s opt2 %i %i %i %i",tour->name,n->a,n->b,n->c,n->d);
        }
      tour->makePermanent();      // fijar el cambio
      return 1;                   // Se logro una optimizacion
      }
    else
      tour->removeLastFlip();      // borrar el ultimo cambio
    }
  }
return 0;
}
//
//  Llenar la PQ[tl] desde la ciudad con el radio dado
//
/////////////////////////////////////////////////////////////////////
void Lk::fillBestMoves(int cityFrom,float radius)
{
int cityPrev=tour->prev(cityFrom);		//  Guardar las ciudad anterior
int cityNext=tour->next(cityFrom);		// y proxima 
int dp=pset->deleted(cityPrev);		//  Borrar las ciudades no permitidas
int dn=pset->deleted(cityNext);
if (!dp) pset->deletePnt(cityPrev);		// no borrar las que ya estan
if (!dn) pset->deletePnt(cityNext);		// borradas
pqNow->clear();					//  Limpiar la PQ
pset->nnBallPQ(cityFrom,radius,*pqNow);	//  Hacer consulta por radio
if (!dp) pset->undeletePnt(cityPrev);	//  Desborrar ciudades
if (!dn) pset->undeletePnt(cityNext);
}
