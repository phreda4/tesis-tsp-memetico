/////////////////////////////////////////////////////////////////////
//
//  Clase 2 Opt
//
/////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include "opt2.h"
#include "aalgo.h"

// precondiciones:
//	pset sin puntos borrados  *pset->undeleteAllPnt();
//	tour ya construido
/////////////////////////////////////////////////////////////////////
void Opt2::run(Tour *t)
{
int city,cnt;
tour=t;
city=0;		// Comienzo con ciudad 0
cnt=0;
do {
  if (!pset->look(city))
    {
    if (opt2Search(city))		// Buscar un 2-opt
      cnt=0;
    else
      {
      cnt++;
      pset->dontlookPnt(city);  // marcar fracaso en la busqueda
      }
    }
  else
    cnt++;
  city=tour->next(city);	// Siguiente ciudad
} while (cnt<pset->cnt);	// Hasta que de una vuelta completa
tour->makePermanent();		// Fijar los cambios
}
//
//	Busqueda de 2opt
//
/////////////////////////////////////////////////////////////////////
int Opt2::opt2Search(int cityA)
{
int cityPrevA,cityNextA,cityNow,cityNextNow;
cityNextA=tour->next(cityA);	// Proxima ciudad
cityPrevA=tour->prev(cityA);	// Ciudad anterior
pset->deletePnt(cityPrevA);	// Borro el anterior ya que no se permite
float distANextA=tour->dist(cityA,cityNextA);
cityNow=pset->nnBallFirst(cityA,pset->dist(cityA,cityNextA));
while (cityNow!=-1) 
  {
  cityNextNow=tour->next(cityNow);	// Proxima ciudad
  if (distANextA+tour->dist(cityNow,cityNextNow) > 
      pset->lastDist()+pset->dist(cityNextA,cityNextNow))
    {
    pset->undeletePnt(cityPrevA);	// reestablesco el anterior
    tour->flip(cityA,cityNextA,cityNow,cityNextNow);	// Hacer el flip
    pset->lookPnt(cityA);
    pset->lookPnt(cityNextA);
    pset->lookPnt(cityNow);
    pset->lookPnt(cityNextNow);
    ANIMA("%s opt2 %i %i %i %i",tour->name,cityA,cityNextA,cityNow,cityNextNow);
    return 1;				// Cambio con exito
    }
  cityNow=pset->nnBall();		// Proxima ciudad
  }
pset->undeletePnt(cityPrevA);	// reestablesco el anterior
return 0;				// Sin exito
}
