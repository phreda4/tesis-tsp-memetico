/////////////////////////////////////////////////////////////////////
//
//  Algoritmo Memetico para el problema del TSP
//
/////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aalgo.h"
#include "tspmeme.h"
#include "tourc.h"
#include "stdtmp.h"

#include "tspnn.h"
#include "tspmf.h"
#include "tspsfc.h"
/////////////////////////////////////////////////////////////////////
void TSPMEME::init(PointSet *ps)  // inicializar memetico
{
pset=ps;                // Conjunto de puntos

lk.init(pset);          // Inicializacion
lk.set("");
opt2.init(pset);
tspsfc.init(pset);      // de los algoritmos
tspmf.init(pset);       // usados para las partes
tspnn.init(pset);       // del algoritmo memetico

popCnt=5;              // 10 individuos
timeRun.set(120,0);      // tiempo de corrida 120 s

frun=&TSPMEME::run1;
fborn=&TSPMEME::bornSFC;
fxover=&TSPMEME::xoverDPX;
fmutat=&TSPMEME::mutation4r;
flsear=&TSPMEME::lsearch2lk;
fchoose=&TSPMEME::choose2;
}
/////////////////////////////////////////////////////////////////////
void TSPMEME::set(char *arg)
{
int t;
char *tok;
if (*arg!=0 && *arg!='+')
  {
  for (tok=strtok(arg," ,");tok;tok=strtok(0," ,"))
    {
    if (!strcasecmp(tok,"nn")) fborn=&TSPMEME::bornNN;
    else if (!strcasecmp(tok,"mf")) fborn=&TSPMEME::bornMF;      // Metodos de nacimiento
    else if (!strcasecmp(tok,"sfc")) fborn=&TSPMEME::bornSFC;
    //
    else if (!strcasecmp(tok,"dpx")) fxover=&TSPMEME::xoverDPX;  // Metodos de cruzamiento
    else if (!strcasecmp(tok,"invx")) fxover=&TSPMEME::xoverINV;  // Metodos de cruzamiento
    //
    else if (!strcasecmp(tok,"4r")) fmutat=&TSPMEME::mutation4r; // Metodos de mutacion
    else if (!strcasecmp(tok,"4o")) fmutat=&TSPMEME::mutation4o;
    //
    else if (!strcasecmp(tok,"2opt")) flsear=&TSPMEME::lsearch2;
    else if (!strcasecmp(tok,"lk")) flsear=&TSPMEME::lsearchLk;  //  Metodos de busqueda local
    else if (!strcasecmp(tok,"2lk")) flsear=&TSPMEME::lsearch2lk;
    //
    else if (!strcasecmp(tok,"ch0")) fchoose=&TSPMEME::choose0;
    else if (!strcasecmp(tok,"ch1")) fchoose=&TSPMEME::choose1;
    else if (!strcasecmp(tok,"ch2")) fchoose=&TSPMEME::choose2;  // Metodos de eleccion
    else if (!strcasecmp(tok,"ch3")) fchoose=&TSPMEME::choose3;
    else if (!strcasecmp(tok,"ch4")) fchoose=&TSPMEME::choose4;
    //
    else if (!strcasecmp(tok,"run1")) frun=&TSPMEME::run1;
    else if (!strcasecmp(tok,"run2")) frun=&TSPMEME::run2;
    else if (!strcasecmp(tok,"run3")) frun=&TSPMEME::run3;
    //
    else if (!strncasecmp(tok,"c:",2))       // cantidad de individuos
      sscanf(tok,"c:%i",&popCnt);
    else if (!strncasecmp(tok,"t:",2))      // tiempo de corrida
      {
      sscanf(tok,"t:%i",&t);
      timeRun.set(t,0);                  // poner tiempo de corrida
      }
    }
  }
}
/////////////////////////////////////////////////////////////////////
void TSPMEME::run(Tour *tour)   // correr algoritmo
{
(this->*frun)(tour);
}
/////////////////////////////////////////////////////////////////////
void TSPMEME::run1(Tour *tour)   // correr algoritmo 1
{
int a,b,c,i;
Time timeNow,timePas,timeIni;
tour->init(pset);
timeNow.now();
timeIni=timeNow;
population=new Individuo[popCnt];

mutationRate=0;                             // tasa de mutacion
(this->*fborn)();                           // nacimiento de la poblacion
bestIndividuo=0;
bestIndividuoF=population[0].fitness();
for (i=0;i<popCnt;i++)
  {
  (this->*flsear)(i);                       // Busqueda local
  if (population[i].fitness()>bestIndividuoF)
    { bestIndividuo=i;bestIndividuoF=population[i].fitness();}
// printf("ind:%i=%f\n",i,population[i].fitness());
  }
bestFitness=bestIndividuoF;
population[bestIndividuo].copyTour(tour); 	// copiar mejor individuo
int generation=0;
int cnt=0;
//while(timePas<timeRun)                      // hasta cumplir el tiempo
while (cnt<150)
  {
  (this->*fchoose)(&a,&b,&c);               // Elegir padres y victima
  if (randomFloat(0,1)<mutationRate)        // tasa de mutaciones
    (this->*fmutat)(c);                     // Mutar
  else
    (this->*fxover)(a,b,c);                 // Cruzar Padres sobre victima
  (this->*flsear)(c);                       // Busqueda local

  if (population[c].fitness()>bestIndividuoF)	    // es el mejor ?
    { bestIndividuo=c;bestIndividuoF=population[c].fitness();}

  if (bestIndividuoF>bestFitness)  // Si es mejor que el mejor...
    {
    bestFitness=bestIndividuoF;    			// actualizar mejor
    population[bestIndividuo].copyTour(tour); 	// copiar tour
//   printf("mejoro:%f en %i\n",bestFitness,cnt);
    cnt=0;
    }
  else
    {
    cnt++;
    mutationRate=cnt*0.004;
    }

//  timeNow.now();                         // Ajustar tiempo transcurrido
//  timePas=timeNow-timeIni;
  generation++;
  }
}
/////////////////////////////////////////////////////////////////////
void TSPMEME::run2(Tour *tour)   // correr algoritmo con Herencia
{
const int cycleA[]={1,1,1,2,2,2,3,3,3,0,0,0};   // padres A
const int cycleB[]={4,5,6,7,8,9,10,11,12,1,2,3};// padres B
const int MAXCYCLE=12;                          // largo del ciclo
int cycle=0;                                    // contador de ciclo

int a,b,i,j;
Time timeNow,timePas,timeIni;
tour->init(pset);
timeNow.now();
timeIni=timeNow;
popCnt=13;                      // son 13 en la poblacion
population=new Individuo[popCnt+1];   // 1 para contener al hijo
int tree[popCnt];               // Arbol
int flag[popCnt];               // marcas de los individuos
int free=popCnt;                // individuo hijo
int xover=0;                    // tipo de Xover a aplicar
(this->*fborn)();                           // nacimiento de la poblacion
for (i=0;i<popCnt;i++)
  {
  (this->*flsear)(i);                       // Busqueda local
  tree[i]=i;                                // Asigar a arbol
  flag[i]=0;                                // limpiar marcas
//  printf("%i %f\n",i,population[i].fitness());
  }
population[13].tour.init(pset);          // inicializar libre
for (i=0;i<popCnt-1;i++)                  // Ordenar arbol
  for (j=i+1;j<popCnt;j++)
    if (population[tree[i]].fitness()<population[tree[j]].fitness())
      swap(tree[i],tree[j]);

int generation=0;
int cnt=0;
//while(timePas<timeRun)                      // hasta cumplir el tiempo
while (cnt<150)
  {
  a=tree[cycleA[cycle]];      // elegir A y B en ciclo
  b=tree[cycleB[cycle]];

  flag[cycleB[cycle]]|=1<xover;  // llenar bit
  switch(xover) {
    case 0:
      xoverDPX(a,b,free);   // Cruzar Padres por DPX
      xover++;
      break;
    case 1:
      xoverAMB(a,b,free);   // Cruzar Padres A-B
      xover++;
      break;
    case 2:
      xoverBMA(a,b,free);   // Cruzar Padres B-A
      xover=0;
      break;
    }
  (this->*flsear)(free);                     // Busqueda local

  // ajustar arbol jerarquico
  cnt++;
  if (population[free].fitness()>population[a].fitness())  // es mejor que A ?
    {
    cnt=0;
    if (cycleA[cycle]>0 && population[free].fitness()>population[tree[0]].fitness())  // es mejor que 0 ?
      {
      swap(tree[0],free);
      flag[0]=0;
      swap(tree[cycleA[cycle]],free);
      flag[cycleA[cycle]]=0;
      swap(tree[cycleB[cycle]],free);
      flag[cycleB[cycle]]=0;
      }
    else
      {
      swap(tree[cycleA[cycle]],free);
      flag[cycleA[cycle]]=0;
      swap(tree[cycleB[cycle]],free);
      flag[cycleB[cycle]]=0;
      }
    }
  else if (population[free].fitness()>population[b].fitness())  // es mejor que B ?
    {
    swap(tree[cycleB[cycle]],free);  // B ahora es el hijo
    flag[cycleB[cycle]]=0;
    }
  else if (flag[cycleB[cycle]]==7) // 3 bits a 1
    {
    swap(tree[cycleB[cycle]],free);  // B ahora es el hijo
    (this->*fmutat)(b); // mutar el descendiente
    (this->*flsear)(b); // Busqueda local
    flag[cycleB[cycle]]=0; // limpiar bits
    }

//  timeNow.now();          // Ajustar tiempo transcurrido
//  timePas=timeNow-timeIni;
  generation++;           // aumentar generacion
  cycle++;                // rotar ciclo
  if (cycle==MAXCYCLE)
    {
    cycle=0;
    xover++;if (xover==3) xover=0; //rotar el xover
    }
  }
tour->copyOf(population[tree[0]].tour);
}
/////////////////////////////////////////////////////////////////////
void TSPMEME::run3(Tour *tour)   // correr algoritmo con Herencia
{
const int cycleA[]={1,1,1,2,2,2,3,3,3,0,0,0};   // padres A
const int cycleB[]={4,5,6,7,8,9,10,11,12,1,2,3};// padres B
const int MAXCYCLE=12;                          // largo del ciclo
int cycle=0;                                    // contador de ciclo

int a,b,i,j;
Time timeNow,timePas,timeIni;
tour->init(pset);
timeNow.now();
timeIni=timeNow;
popCnt=14;                      // son 14 en la poblacion
population=new Individuo[popCnt];   // 1 para contener al hijo
int tree[popCnt];               // Arbol
int flag[popCnt];               // marcas de los individuos
int free=popCnt-1;                // individuo hijo
int xover=0;                    // tipo de Xover a aplicar
(this->*fborn)();                           // nacimiento de la poblacion
for (i=0;i<popCnt;i++)
  {
  (this->*flsear)(i);                       // Busqueda local
  tree[i]=i;                                // Asigar a arbol
  flag[i]=0;                                // limpiar marcas
//  printf("%i %f\n",i,population[i].fitness());
  }

for (i=0;i<popCnt-1;i++)                  // Ordenar arbol
  for (j=i+1;j<popCnt;j++)
    if (population[tree[i]].fitness()<population[tree[j]].fitness())
      swap(tree[i],tree[j]);

int generation=0;
int cnt=0;
//while(timePas<timeRun)                      // hasta cumplir el tiempo
while (cnt<150)
  {
  a=tree[cycleA[cycle]];      // elegir A y B en ciclo
  b=tree[cycleB[cycle]];

  flag[cycleB[cycle]]|=1<xover;  // llenar bit

  xoverINV(a,b,free);           // recombinacion
  (this->*flsear)(free);        // Busqueda local

  // ajustar arbol jerarquico
  cnt++;
  if (population[free].fitness()>population[a].fitness())  // es mejor que A ?
    {
    cnt=0;
    if (cycleA[cycle]>0 && population[free].fitness()>population[tree[0]].fitness())  // es mejor que 0 ?
      {
      swap(tree[0],free);
      flag[0]=0;
      swap(tree[cycleA[cycle]],free);
      flag[cycleA[cycle]]=0;
      swap(tree[cycleB[cycle]],free);
      flag[cycleB[cycle]]=0;
      }
    else
      {
      swap(tree[cycleA[cycle]],free);
      flag[cycleA[cycle]]=0;
      swap(tree[cycleB[cycle]],free);
      flag[cycleB[cycle]]=0;
      }
    }
  else if (population[free].fitness()>population[b].fitness())  // es mejor que B ?
    {
    swap(tree[cycleB[cycle]],free);  // B ahora es el hijo
    flag[cycleB[cycle]]=0;
    }
  else if (flag[cycleB[cycle]]==7) // 3 bits a 1
    {
    swap(tree[cycleB[cycle]],free);  // B ahora es el hijo
    (this->*fmutat)(b); // mutar el descendiente
    (this->*flsear)(b); // Busqueda local
    flag[cycleB[cycle]]=0; // limpiar bits
    }

//  timeNow.now();          // Ajustar tiempo transcurrido
//  timePas=timeNow-timeIni;
  generation++;           // aumentar generacion
  cycle++;                // rotar ciclo
  if (cycle==MAXCYCLE)
    {
    cycle=0;
    xover++;if (xover==3) xover=0; //rotar el xover
    }    
  }
tour->copyOf(population[tree[0]].tour);
}

/////////////////////////////////////////////////////////////////////
void TSPMEME::bornSFC()  // nacimiento por SFC
{
Individuo *individuo=population;
int i;
float angle,step;
angle=0;
step=90/popCnt;
for (i=0;i<popCnt;i++,individuo++)
  {
  individuo->tour.init(pset);
  individuo->tour.setName(i);
  ANIMA("Tsp tsp%i",i);
  ANIMA("tsp%i instance xxx",i);
  tspsfc.run(&individuo->tour,angle);
  angle+=step;
  }
}
/////////////////////////////////////////////////////////////////////
void TSPMEME::bornMF()   // nacimiento por MF
{
Individuo *individuo=population;
int i;
for (i=0;i<popCnt;i++,individuo++)
  {
  individuo->tour.init(pset);
  individuo->tour.setName(i);
  ANIMA("Tsp tsp%i",i);  
  ANIMA("tsp%i instance xxx",i);
  tspmf.runRandom(&individuo->tour);
  }
}
/////////////////////////////////////////////////////////////////////
void TSPMEME::bornNN()   // nacimiento por NN
{
Individuo *individuo=population;
int i;
for (i=0;i<popCnt;i++,individuo++)
  {
  individuo->tour.init(pset);
  individuo->tour.setName(i);
  ANIMA("Tsp tsp%i",i);  
  ANIMA("tsp%i instance xxx",i);
  tspnn.runRandom(&individuo->tour);
  }
}
/////////////////////////////////////////////////////////////////////
void TSPMEME::choose0(int *a,int *b,int *c)  // Eleccion por rueda 
{
Individuo *ind=population;
*a=bestIndividuo;
float suma=0,peor=MAXFLOAT;
for (int i=0;i<popCnt;i++,ind++)
  {
  suma+=ind->fitness();
  if (peor>ind->fitness())
    { *c=i;peor=ind->fitness(); }
  }
suma-=population[*c].fitness();
suma-=peor*(popCnt-1);
float add=0,rand=randomFloat(0,suma);
*a=-1;
do {
  (*a)++;if (*a==*c) (*a)++;
  add+=population[*a].fitness()-peor;
} while (add<rand);
suma-=population[*a].fitness()-peor;
add=0;rand=randomFloat(0,suma);
*b=-1;
do {
  (*b)++;if (*b==*a || *b==*c) (*b)++;
  add+=population[*b].fitness()-peor; 
} while (add<rand);
}
/////////////////////////////////////////////////////////////////////
void TSPMEME::choose1(int *a,int *b,int *c)  // Eleccion de mejor, cualquiera,peor
{
*a=bestIndividuo;
float peor=MAXFLOAT;
for (int i=0;i<popCnt;i++)
  if (peor>population[i].fitness())
    { *c=i;peor=population[i].fitness(); }
if (*a==*c) (*c)++;
do { *b=randomInt(0,popCnt-1); } while (*b==*a || *b==*c);
}
/////////////////////////////////////////////////////////////////////
void TSPMEME::choose2(int *a,int *b,int *c)  // Eleccion de cualquiera*3
{
*a=randomInt(0,popCnt-1);
*b=(*a+randomInt(1,popCnt-1))%popCnt;
do { *c=randomInt(0,popCnt-1); } while (*a==*c || *b==*c);
}
/////////////////////////////////////////////////////////////////////
void TSPMEME::choose3(int *a,int *b,int *c)  // Eleccion de mejor, cualquiera*2
{
*a=bestIndividuo;
*b=(*a+randomInt(1,popCnt-1))%popCnt;
do { *c=randomInt(0,popCnt-1); } while (*a==*c || *b==*c);
}
/////////////////////////////////////////////////////////////////////
void TSPMEME::choose4(int *a,int *b,int *c)  // Eleccion de mejor, cualquiera*2
{
static int nro=0;
*c=nro;
*b=(*c)+1;if (*b==popCnt) *b=0;
*a=(*b)+1;if (*a==popCnt) *a=0;
nro++;
}
/////////////////////////////////////////////////////////////////////
void TSPMEME::xoverDPX(int a,int b,int c)  // xover DPX (parent,parent->child)
{
population[c].dpx(population[a],population[b]);
}
/////////////////////////////////////////////////////////////////////
void TSPMEME::xoverINV(int a,int b,int c)  // xover inver
{
population[c].inverover(population[a],population[b]);
}
/////////////////////////////////////////////////////////////////////
void TSPMEME::xoverDPXP(int a,int b,int c)  // xover DPX (parent,parent->child)
{
population[c].dpxp(population[a],population[b]);
}
/////////////////////////////////////////////////////////////////////
void TSPMEME::xoverAMB(int a,int b,int c)  // xover MFX
{
population[c].amb(population[a],population[b]);
}
/////////////////////////////////////////////////////////////////////
void TSPMEME::xoverBMA(int a,int b,int c)  // xover MFX
{
population[c].bma(population[a],population[b]);
}
/////////////////////////////////////////////////////////////////////
void TSPMEME::mutation4r(int a)  // Mutacion 4-opt al azar (like ILK)
{
population[a].mutate4r();
}
/////////////////////////////////////////////////////////////////////
void TSPMEME::mutation4o(int a)  // 4-opt search (idea de Glover)
{
population[a].mutate4o();
}
/////////////////////////////////////////////////////////////////////
void TSPMEME::lsearch2(int a)   // local search por 2opt
{
opt2.run(&population[a].tour);
}
/////////////////////////////////////////////////////////////////////
void TSPMEME::lsearchLk(int a)   // local search por LK
{
lk.runLk(&population[a].tour);
}
/////////////////////////////////////////////////////////////////////
void TSPMEME::lsearch2lk(int a)   // local search por 2opt+LK
{
opt2.run(&population[a].tour);
lk.runLk(&population[a].tour);
}

