/////////////////////////////////////////////////////////////////////
//
//   Generador de instancias
//
/////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pointset.h"
#include "stdtmp.h"

/////////////////////////////////////////////////////////////////////
int main(int argc,char *argv[])
{
PointSet pset;
int cntCiudades=0,cntInstancias=0;
char argumento[60];
//
//  buscar argumentos validos
//
for (int argNum=1;argNum<argc;argNum++)
  {
  if (!strncasecmp(argv[argNum],"-c",2))
    sscanf(argv[argNum],"-c%i",&cntCiudades);
  if (!strncasecmp(argv[argNum],"-i",2))
    sscanf(argv[argNum],"-i%i",&cntInstancias);
  }
if (cntCiudades<1 || cntInstancias<1)
  {
  printf("\nModo de uso:\n#geni -c[cantidad de ciudades] -i[cantidad de instancias]\n");
  exit(-1);
  }
pset.size(cntCiudades);
int ciudades,instancias;
for (instancias=0;instancias<cntInstancias;instancias++)
  {
  for (ciudades=0;ciudades<cntCiudades;ciudades++)
    while (pset.addPoint(ciudades,randomFloat(0,100),randomFloat(0,100))==-1) ;
  sprintf(argumento,"r%i-%i.tsp",instancias,cntCiudades);
  pset.save(argumento);
  }
  return 0;
}
