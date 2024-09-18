/////////////////////////////////////////////////////////////////////
//
//  Tsp
//
/////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "pointset.h"
#include "aalgo.h"
#include "tourc.h"
#include "time.h"
//
// Constructores de tours
//
#include "tspnn.h"
#include "tspmf.h"
#include "tspsfc.h"
#include "tspmeme.h"
//
// Optimizadores de tours
//
#include "opt2.h"
#include "opt3.h"
#include "lk.h"
//
//  Constructores de tour
//  Nearest Neighbor
//
/////////////////////////////////////////////////////////////////////
void construyeNN(PointSet *pset,Tour *tour)
{
TSPNN	tspnn;

ANIMA("state 'inicializando nn'");
tspnn.init(pset);
ANIMA("state 'run nn'");
tspnn.run(tour);
}
//
//  Multiple Fragment
//
/////////////////////////////////////////////////////////////////////
void construyeMF(PointSet *pset,Tour *tour)
{
TSPMF	tspmf;
ANIMA("state 'inicializando mf'");
tspmf.init(pset);
ANIMA("state 'run mf'");
tspmf.run(tour);
}
//
//  Space Filling Curves
//
/////////////////////////////////////////////////////////////////////
void construyeSFC(PointSet *pset,Tour *tour)
{
TSPSFC	tspsfc;

ANIMA("state 'inicializando sfc'");
tspsfc.init(pset);
ANIMA("state 'run sfc'");
tspsfc.run(tour,0);             // resultado, angulo
}
//
//  Memetico
//
/////////////////////////////////////////////////////////////////////
void construyeMEME(PointSet *pset,Tour *tour,char *arg)
{
TSPMEME	tspmeme;

ANIMA("state 'inicializando memetico'");
tspmeme.init(pset);
tspmeme.set(arg);
ANIMA("state 'run memetico'");
tspmeme.run(tour);
}
//
//  Optimizadores de tour
//
/////////////////////////////////////////////////////////////////////
void optimiza2opt(PointSet *pset,Tour *tour)
{
Opt2	opt2;

ANIMA("state 'init 2-Opt'");
opt2.init(pset);
ANIMA("state 'run 2-Opt'");
opt2.run(tour);
}

/////////////////////////////////////////////////////////////////////
void optimiza3opt(PointSet *pset,Tour *tour)
{
Opt3	opt3;

ANIMA("state 'init 3-Opt'");
opt3.init(pset);
ANIMA("state 'run 3-Opt'");
opt3.run(tour);
}

/////////////////////////////////////////////////////////////////////
void optimizaLk(PointSet *pset,Tour *tour,char *arg)
{
Lk	lk;

ANIMA("state 'init Lk'");
lk.init(pset);
lk.set(arg);
ANIMA("state 'run Lk'");
lk.runLk(tour);
}
/////////////////////////////////////////////////////////////////////
void optimizaIlk(PointSet *pset,Tour *tour,char *arg)
{
Lk	lk;

ANIMA("state 'init Ilk'");
lk.init(pset);
lk.set(arg);
ANIMA("state 'run Ilk'");
lk.runIlk(tour);	// 30 segundos
}

/////////////////////////////////////////////////////////////////////
//
//  Principal
//
///main//////////////////////////////////////////////////////////////////
int main(int argc,char *argv[])
{
//
//  Si tiene pocos argumentos, mostrar ayuda
//
if (argc<3) 
  {
  printf("Forma de uso\n");
  printf("\n");
  printf("# tsp att532.tsp -mmf+2opt+ilk [-a/nimacion] [-s/estaditica] [-r/resultado]\n\n");
  exit(1);
  }
//
//  Variables para calculo de tiempo
//
Time start,next,res;

PointSet pset;          // instancia a solucionar
Tour tour;              // solucion encontrada
char tarea[80];         // algoritmos a ejecutar
char tarean[80];        // copia de tarea para usar como nombre
char name[80];          // nombre de la instancia
char flaga=0,flagr=0,flags=0;   // banderas para animar y grabar resultado
float optimo=-1;
//
//  buscar argumentos validos
//
for (int argNum=1;argNum<argc;argNum++)
  {
  if (!strcasecmp("-a",argv[argNum]))	// activar la animacion
    flaga=1;
  else if (!strcasecmp("-r",argv[argNum]))	// grabar el resultado
    flagr=1;
  else if (!strcasecmp("-s",argv[argNum]))      // grabar estadistica
    flags=1;
  else if (!strncasecmp("-m",argv[argNum],2))  // forma de resolver
    strcpy(tarea,&argv[argNum][2]);
  else if (!strncasecmp("-oc",argv[argNum],3))  // optimo calculado
    optimo=-2;
  else if (!strncasecmp("-o",argv[argNum],2))  // optimo
    sscanf(argv[argNum],"-o%f",&optimo);
  }
strcpy(tarean,tarea);
strcpy(name,argv[1]);
//
//  activar animacion si se pide
//
if (flaga)
  aalgo.init(argv[1],tarea);

//
//  Preparando instancia
//
ANIMA("String state");
ANIMA("Tsp tsp");
ANIMA("tsp instance %s",argv[1]);
ANIMA("state 'cargando instancia'");
//
//  Carga PointSet de archivo (.TSP)
//
start.now();            // comenzar a calcular tiempo
pset.load(name);
if (optimo==-2) // estimar cota optimo por Held-Karp (cnt punto en 100*100)
  {
  optimo=(0.70805+(0.52229/sqrt(pset.cnt))+(1.31572/pset.cnt)
        -(3.07474/(pset.cnt*sqrt(pset.cnt))))*sqrt(pset.cnt*100*100);
  }
if (flags==1)
  {
  FILE *f=fopen("stat.txt","at");
  fprintf(f,"%s %s ;",name,tarean);
  fclose(f);
  }
//
// Activacion de opciones
//
char *tok;
int len,construido=0;               // todabia no hay un tour
for(tok=strtok(tarea,"+");tok;tok=strtok(0,"+"))
  {
  if (!strcasecmp(tok,"nn"))
    {
    printf("Construyendo Tour NN (nearest Nehibour)\n");
    construyeNN(&pset,&tour);
    }
  else if (!strcasecmp(tok,"mf"))
    {
    printf("Construyendo Tour MF (Multiple Fragments)\n");
    construyeMF(&pset,&tour);
    }
  else if (!strcasecmp(tok,"sfc"))
    {
    printf("Construyendo Tour SFC (Space Filling Curves)\n");
    construyeSFC(&pset,&tour);
    }
  else if (!strncasecmp(tok,"meme",4))
    {
    printf("Construyendo Algoritmo Memetico\n");
    construyeMEME(&pset,&tour,&tok[4]);
    }
  else if (!strcasecmp(tok,"2opt"))
    {
    printf("Optimizando por 2-Opt.\n");
    optimiza2opt(&pset,&tour);
    }
  else if (!strcasecmp(tok,"3opt"))
    {
    printf("Optimizando por 3-Opt.\n");
    optimiza3opt(&pset,&tour);
    }
  else if (!strncasecmp(tok,"lk",2))
    {
    printf("Optimizando por Lin-Kerningham.\n");
    optimizaLk(&pset,&tour,&tok[2]);
    }
  else if (!strncasecmp(tok,"ilk",3))
    {
    printf("Optimizando por Iterated-Lin-Kerningham.\n");
    optimizaIlk(&pset,&tour,&tok[3]);
    }
  next.now();                   // ajustar tiempo
  res=next-start;               // calcular tiempo transcurrido
  len=tour.intLenght();
  if (optimo==-1)
    printf("Tour:%i Tiempo:%s\n",len,res.toStr());
  else
    printf("Tour:%f Tiempo:%s\n",(len-optimo)/optimo,res.toStr());
  if (flags==1)
    {
    FILE *f=fopen("stat.txt","at");
    if (optimo==-1)
      fprintf(f,"%i ; %s ;",len,res.toStr());
    else
      fprintf(f,"%f ; %s ;",(len-optimo)/optimo,res.toStr());
    fclose(f);
    }
  }
if (flags==1)
  {
  FILE *f=fopen("stat.txt","at");
  fprintf(f,"\n");
  fclose(f);
  }
if (flagr==1)                   // Grabar resultado
  {
  char *p=name;
  while (*p!='.' && *p!=0) p++;
  *p=0;
  strcat(name,".sol");
  tour.save(name);
  }
  return 0;
}

