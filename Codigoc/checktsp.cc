/////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "pointset.h"
#include "tourc.h"
/////////////////////////////////////////////////////////////////////
//
//  Principal
//
/////////////////////////////////////////////////////////////////////
int main(int argc,char *argv[])
{
PointSet pset;          // instancia a solucionar
Tour tour;              // solucion encontrada
char name[80];          // nombre de la instancia
char test[80];
int total,d;
strcpy(name,argv[1]);
strcpy(test,argv[2]);
//
//  Carga PointSet de archivo (.TSP)
//
pset.load(name);
tour.init(&pset);
FILE *f=fopen(test,"rt");
int i,p,a=-1;
total=0;
for (i=0;i<pset.cnt;i++)
  {
  fscanf(f,"%i",&p);
  tour.addPoint(p);
  if (a!=-1)
    {
    d=(int)pset.dist(p,a);
    total+=d;
    }
  a=p;
  }
fclose(f);
tour.test();
printf("%i\n",total);
printf("%f\n",tour.lenght());
return 0;
}
