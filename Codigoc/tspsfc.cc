/////////////////////////////////////////////////////////////////////
//
//  Clase para resolver el TSP por SFC
//
/////////////////////////////////////////////////////////////////////

#include <math.h>
#include <stdio.h>
#include <ctype.h>
#include "aalgo.h"
#include "tspsfc.h"
#include "pq.h"

#define MAXINPUT 32000

/////////////////////////////////////////////////////////////////////
//
// Codigo extraido de
// A routing system based on spacefilling curves
// John J. Bartholdi, III
// April 11, 1996
//
/////////////////////////////////////////////////////////////////////
int TSPSFC::sierpinskiIndex(int x,int y)
{
int result,loopindex,oldx;

loopindex=MAXINPUT;
result=0;
if (x>y) 
  {
  result++;
  x=MAXINPUT-x;
  y=MAXINPUT-y;
  }
while (loopindex>0) {
  result+=result;
  if (x+y>MAXINPUT) 
    {
    result++;
    oldx=x;
    x=MAXINPUT-y;
    y=oldx;
    }
  x+=x;
  y+=y;
  result+=result;
  if (y>MAXINPUT)
    {
    result++;
    oldx=x;
    x=y-MAXINPUT;
    y=MAXINPUT-oldx;
    }
  loopindex=loopindex>>1;
  }
return result;
}

/////////////////////////////////////////////////////////////////////
//  Calcular parametros
/////////////////////////////////////////////////////////////////////
void TSPSFC::limits()
{
int i;
minx=maxx=pset->x(0);
miny=maxy=pset->y(0);
for(i=1;i<pset->cnt;i++)	// obtengo los maximos y minimos
  {
  if (pset->x(i)>maxx) maxx=pset->x(i);
  if (pset->y(i)>maxy) maxy=pset->y(i);
  if (pset->x(i)<minx) minx=pset->x(i);
  if (pset->y(i)<miny) miny=pset->y(i);  
  }
cenx=(maxx+minx)/2;
ceny=(maxy+miny)/2;
}

/////////////////////////////////////////////////////////////////////
void TSPSFC::init(PointSet *ps)
{
pset=ps;
limits();
}

/////////////////////////////////////////////////////////////////////
void TSPSFC::run(Tour *tour,float angle)
{
int x,y,i;
float xc,yc,xa,ya;
float cosa,sina;
PQ<int> pq;

cosa=cos(angle*M_PI/180);	// de grados a radianes
sina=sin(angle*M_PI/180);

pq.init(pset->cnt);
for(i=0;i<pset->cnt;i++)	// Rotar cada punto
  {
  xc=pset->x(i)-cenx;
  yc=pset->y(i)-ceny;
  xa=(xc*cosa-yc*sina)+cenx;
  ya=(xc*sina+yc*cosa)+ceny;
  
  x=(int)((xa-minx)*MAXINPUT/(maxx-minx));
  y=(int)((ya-miny)*MAXINPUT/(maxy-miny));
  pq.insert(sierpinskiIndex(x,y),i);  	// Insertarlo en PQ
  }

tour->init(pset);
float v;
int first,last=-1;
while (!pq.empty()) 				// Hasta que no haya puntos
  {	
  pq.remove(&v,&i);				// Sacar el primero
  tour->addPoint(i);				// Agregar el tour
  if (last==-1)
    first=i;					// Agragar el ultimo
  else
    ANIMA("%s add %i %i",tour->name,last,i);
  last=i;
  }
ANIMA("%s add %i %i",tour->name,last,first);
}
