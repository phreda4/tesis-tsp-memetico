/////////////////////////////////////////////////////////////////////
//
//	Tour para un meme, puede guardar pedazos incompletos
//
/////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aalgo.h"
#include "tourmeme.h"

PointSet *TourMeme::pset=NULL;

//
//  Inicializa clase
// 
/////////////////////////////////////////////////////////////////////
void TourMeme::initClass(PointSet *ps)
{
pset=ps;
}
//
//  Inicializa
//
/////////////////////////////////////////////////////////////////////
void TourMeme::init(char *n)
{
if (pset==NULL)
  {
  printf("Error clase TourMeme no inicializada\n");
  exit(0);
  }
nodos=new NodoTM[pset->cnt];
strcpy(name,n);
ANIMA("Tsp %s",name);
ANIMA("Float F%s",name);
ANIMA("F%s 0",name);
}
//
//  Limpia el objeto
//
/////////////////////////////////////////////////////////////////////
void TourMeme::clear()
{
if (nodos!=NULL)
  delete [] nodos;
nodos=NULL;
}
//
// Limpia el tour
//
/////////////////////////////////////////////////////////////////////
void TourMeme::clearTour()
{
NodoTM *p=nodos;
for (int i=0;i<pset->cnt;i++,p++)
  {
  p->vec[0]=p->vec[1]=-1;
  p->grado=0;
  p->desde=-1;
  }
 
arcosUnidos=0;
distanciaArcos=0;

pset->undeleteAllPnt();
ANIMA("%s clear",name);
}
//
//  Copia de un tour fuente (t)
//
/////////////////////////////////////////////////////////////////////
void TourMeme::copyOf(TourMeme &t)
{
clear();
pset=t.pset;
nodos=new NodoTM[pset->cnt]; 

NodoTM *yo=nodos,*vos=t.nodos;
for (int i=0;i<pset->cnt;i++,yo++,vos++)
  *yo=*vos;

arcosUnidos=t.arcosUnidos;
distanciaArcos=t.distanciaArcos;
}
//
//	borrar un arco con control de puntos
//
/////////////////////////////////////////////////////////////////////
void TourMeme::addEdge(int x,int y)
{
//
// precondicion, que haya lugar en los nodos
//
if (nodos[x].grado>1 || nodos[y].grado>1)
  return;
//
// Si ya esta unido no unirlo nuevamente
//
if (nodos[x].grado>0 && nodos[x].vec[0]==y)
  return;
nodos[x].vec[nodos[x].grado++]=y;
nodos[y].vec[nodos[y].grado++]=x;
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
  }
  
arcosUnidos++;
distanciaArcos+=pset->calcDistC(x,y);

ANIMA("%s add %i %i",name,x,y);
ANIMA("F%s %f",name,distanciaArcos);
}
//
//	Borrar un arco con sin control de puntos (no los borra)
//	y sin control de cola de gusano
//
/////////////////////////////////////////////////////////////////////
void TourMeme::addEdge2(int x,int y)
{
//
// precondicion, que haya lugar en los nodos
//
if (nodos[x].grado>1 || nodos[y].grado>1)
  return;
//
// Si ya esta unido no unirlo nuevamente
//
if (nodos[x].grado>0 && nodos[x].vec[0]==y)
  return;
nodos[x].vec[nodos[x].grado++]=y;
nodos[y].vec[nodos[y].grado++]=x;
arcosUnidos++;
distanciaArcos+=pset->calcDistC(x,y);
ANIMA("%s add %i %i",name,x,y);
ANIMA("F%s %f",name,distanciaArcos);
}
//
//	Borrar arco con control de puntos
//
/////////////////////////////////////////////////////////////////////
void TourMeme::delEdge(int x,int y)
{
if (nodos[x].vec[0]==y)
  nodos[x].vec[0]=nodos[x].vec[1];
if (nodos[y].vec[0]==x)
  nodos[y].vec[0]=nodos[y].vec[1];

nodos[x].grado--;
nodos[y].grado--;

pset->undeletePnt(x);
pset->undeletePnt(y);



arcosUnidos--;

distanciaArcos-=pset->calcDistC(x,y);



ANIMA("%s del %i %i",name,x,y);

ANIMA("F%s %f",name,distanciaArcos);

}

//

//	Borrar arco sin control de puntos

//

/////////////////////////////////////////////////////////////////////

void TourMeme::delEdge2(int x,int y)

{

if (nodos[x].vec[0]==y)

  nodos[x].vec[0]=nodos[x].vec[1];

if (nodos[y].vec[0]==x)

  nodos[y].vec[0]=nodos[y].vec[1];



nodos[x].grado--;

nodos[y].grado--;



arcosUnidos--;

distanciaArcos-=pset->calcDistC(x,y);



ANIMA("%s del %i %i",name,x,y);

ANIMA("F%s %f",name,distanciaArcos);

}

//////////////////////////////////////////////////////////////////////

void TourMeme::error(int x)

{

if (x<0 || nodos[x].nn<0)

  {

  printf("%i x:%i\n",pset->cntBorrados,x);

  printf("fallo cnt:%i cntB:%i  gr:%i %i\n",pset->cnt,pset->cntBorrados,

	nodos[x].grado,x);

  for (int k=0;k<pset->cnt;k++)

     printf("%i %i %i %i %i\n",k,nodos[k].grado,

		nodos[k].vec[0],nodos[k].vec[1],nodos[k].desde);

  exit(0);

  }

}

//

//  Construir un tour con Multiple Fragment Random

//

/////////////////////////////////////////////////////////////////////

void TourMeme::buildMF(int maxGen)

{

NodoTM *p;

PQ<int> pq;

int i,j;



pq.init(pset->cnt);

pset->undeleteAllPnt();



ANIMA("%s clear",name);

p=&nodos[0];

for (i=0;i<pset->cnt;i++,p++)

  {

  p->grado=0;

  p->desde=-1;

  p->nn=pset->nn(i);

//

//	Provisoriamente vec[1] contiene la 'generacion' de arco que es

//

  p->vec[1]=0;

  pq.insert(pset->lastDist()*(.5+1.5*rand()/(RAND_MAX+1.0)),i);

  }

arcosUnidos=0;



float dist;

int x,y,last;

while (!pq.empty()) {

  pq.remove(&dist,&x);//printf("%i:",x);

  p=&nodos[x];

  if (p->grado==2)	// ya esta en algun subtour

    continue;

  y=p->nn;

  if (nodos[y].grado<2 && y!=p->desde)

    {

    addEdge(x,y);//printf("%i|",y);

    last=p->desde;	// ultima cabeza de fragmento

    if (p->grado==2)

      continue;

    }

  if (pset->cntBorrados<pset->cnt-2)

    {

    p->vec[1]--;

    if (-p->vec[1]<maxGen)

      {

      if (p->grado==1) pset->deletePnt(p->desde);

      p->nn=pset->nn(x);

      if (p->grado==1) pset->undeletePnt(p->desde);

      pq.insert(pset->lastDist()*(.5+1.5*rand()/(RAND_MAX+1.0)),x);

      }

    }

  }

if (pset->cntBorrados==pset->cnt-2)

  addEdge(last,nodos[last].desde);

//print();  

}



//

//  Multi Fragment Crossover

//

// Podrias hacer un MF con ``distance preservation'' ?

//

// Seria asi:

//

// a) Sean {a_1, a_2, .... a_n} el conjunto de arestas de uno de los padres

// que llamaremos `A' y {b_1, b_2, ..., b_n} las arestas del otro padre `B'.

//

// b) sea ch el `hijo', que sera formado incluyendo primero todas las arestas

// que estan en la interseccion, es decir a_i=b_j para algun par (i,j). Todas

// las otras arestas que pertenecen a uno pero no pertenecen al otro estan

// TABU, es decir, no pueden ser incluidas en el hijo.

//

// c) Completamos el hijo corriendo MF sobre el resto de las aristas.

//

//

/////////////////////////////////////////////////////////////////////

void TourMeme::xover(TourMeme &tf1,TourMeme &tf2,int maxGen)

{

int i,last;

clearTour();

//

// Poner arcos que esten presentes en los dos

//

NodoTM *p1,*p2;

p1=tf1.nodos;

p2=tf2.nodos;

//

// precondicion: los tours estan completos

//

for (i=0;i<pset->cnt;i++,p1++,p2++)

  {

  if (p1->vec[0]==p2->vec[0] || p1->vec[0]==p2->vec[1])

    addEdge(i,p1->vec[0]);

  if (p1->vec[1]==p2->vec[0] || p1->vec[1]==p2->vec[1])

    addEdge(i,p1->vec[1]);

  }

//

//	Agregar en PQ los arcos que faltan

//

PQ<int> pq;

pq.init(pset->cnt);

NodoTM *p=nodos;

p1=tf1.nodos;

p2=tf2.nodos;

for (i=0;i<pset->cnt;i++,p++,p1++,p2++)

  {

  if (p->grado<2)

    {

    if (p->grado==1) pset->deletePnt(p->desde);

    if (nodos[p1->vec[0]].grado<2) pset->deletePnt(p1->vec[0]);

    if (nodos[p1->vec[1]].grado<2) pset->deletePnt(p1->vec[1]);

    if (nodos[p2->vec[0]].grado<2) pset->deletePnt(p2->vec[0]);

    if (nodos[p2->vec[1]].grado<2) pset->deletePnt(p2->vec[1]);

    p->nn=pset->nn(i);

    if (nodos[p1->vec[0]].grado<2) pset->undeletePnt(p1->vec[0]);

    if (nodos[p1->vec[1]].grado<2) pset->undeletePnt(p1->vec[1]);

    if (nodos[p2->vec[0]].grado<2) pset->undeletePnt(p2->vec[0]);

    if (nodos[p2->vec[1]].grado<2) pset->undeletePnt(p2->vec[1]);

    if (p->grado==1) pset->undeletePnt(p->desde);

    if (p->grado==1) pset->deletePnt(p->desde);

    if (p->nn==-1) p->nn=pset->nn(i);

    if (p->grado==1) pset->undeletePnt(p->desde);

//    pq.insert(pset->lastDist(),i);

    pq.insert(pset->lastDist()*(.5+1.5*rand()/(RAND_MAX+1.0)),i);

    }

  }

ANIMA("state 'completando'");

//

// Completar el tour con MF

//

float dist;

int x,y;

while (!pq.empty()) {

  pq.remove(&dist,&x);

  p=&nodos[x];

  p1=&tf1.nodos[x];

  p2=&tf2.nodos[x];

  if (p->grado==2)	// ya esta en algun subtour

    continue;

  y=p->nn;

  if (nodos[y].grado<2 && y!=p->desde)

    {

    addEdge(x,y);

    last=p->desde;

    if (p->grado==2)

      continue;

    }

  if (pset->cntBorrados<pset->cnt-2)

    {

    if (p->grado==1) pset->deletePnt(p->desde);

    if (nodos[p1->vec[0]].grado<2) pset->deletePnt(p1->vec[0]);

    if (nodos[p1->vec[1]].grado<2) pset->deletePnt(p1->vec[1]);

    if (nodos[p2->vec[0]].grado<2) pset->deletePnt(p2->vec[0]);

    if (nodos[p2->vec[1]].grado<2) pset->deletePnt(p2->vec[1]);

    p->nn=pset->nn(x);

    if (nodos[p1->vec[0]].grado<2) pset->undeletePnt(p1->vec[0]);

    if (nodos[p1->vec[1]].grado<2) pset->undeletePnt(p1->vec[1]);

    if (nodos[p2->vec[0]].grado<2) pset->undeletePnt(p2->vec[0]);

    if (nodos[p2->vec[1]].grado<2) pset->undeletePnt(p2->vec[1]);

    if (p->grado==1) pset->undeletePnt(p->desde);

    if (p->grado==1) pset->deletePnt(p->desde);

    if (p->nn==-1) p->nn=pset->nn(x);

    if (p->grado==1) pset->undeletePnt(p->desde);

    pq.insert(pset->lastDist(),x);

    }

  }

addEdge(last,nodos[last].desde);

//printf("last:%i\n",last);

//print();

}





//

//  Mutacion: realiza un 4-opt al azar

//  falta definir si se normaliza antes o no

//

/////////////////////////////////////////////////////////////////////

void TourMeme::mutation()

{

int i,dist,cntPosible;

int ciudad[8];	// 4 aristas

int actual,anterior;



// La primer ciudad es cualquiera

cntPosible=pset->cnt-7;

anterior = ciudad[0] = (int)((float)cntPosible*rand()/(RAND_MAX+1.0));

actual   = ciudad[1] = nodos[anterior].vec[0];



for (i=2;i<8;i+=2)

  {

  dist=(int)( (float)cntPosible*rand()/(RAND_MAX+1.0) )+1;

  cntPosible-=(dist+2);

  actual=ciudad[1];

  anterior=ciudad[0];

  while (dist>0) {

    if (nodos[actual].vec[0]==anterior)

      { anterior=actual;actual=nodos[actual].vec[1]; }

    else

      { anterior=actual;actual=nodos[actual].vec[0]; }   

    dist--;

    }

  ciudad[i]=anterior;

  ciudad[i+1]=actual;

  }

ANIMA("state '**** Mutando ****'");

for (i=0;i<8;i+=2) 

  delEdge(ciudad[i],ciudad[i+1]);



addEdge(ciudad[0],ciudad[5]);

addEdge(ciudad[1],ciudad[4]);

addEdge(ciudad[2],ciudad[7]);

addEdge(ciudad[3],ciudad[6]);

}

//

//  Copia un Tour a este TourMeme

//

/////////////////////////////////////////////////////////////////////

void TourMeme::deTour(Tour &tour)

{

int i,c1,c2;

distanciaArcos=0;

for (i=0;i<pset->cnt;i++)

  {

  c1=tour.city(i);

  c2=tour.city((i+1)%pset->cnt);

  nodos[c1].vec[0]=c2;

  nodos[c2].vec[1]=c1;

  nodos[c1].grado=2;

  distanciaArcos+=pset->calcDistC(c1,c2);

  }

arcosUnidos=pset->cnt;

}

//

//  Copia este TourMeme a un Tour comun

//

/////////////////////////////////////////////////////////////////////

void TourMeme::aTour(Tour &tour)

{

int anterior=0,actual=0;

tour.init(pset);

for (int i=0;i<pset->cnt;i++)

  {

  tour.addPnt(actual);

  if (nodos[actual].vec[0]==anterior)

    {

    anterior=actual;

    actual=nodos[actual].vec[1];

    }

  else

    {

    anterior=actual;

    actual=nodos[actual].vec[0];

    }   

  }

}

//

//  Imprimir tour

//

/////////////////////////////////////////////////////////////////////

void TourMeme::print()

{

int i;

int anterior=0,actual=0;

for (i=0;i<pset->cnt;i++)

  {

  printf("=%i",actual);

  if (nodos[actual].grado!=2)

    printf("*");

  if (nodos[actual].vec[0]==anterior)

    {

    anterior=actual;

    actual=nodos[actual].vec[1];

    }

  else

    {

    anterior=actual;

    actual=nodos[actual].vec[0];

    }   

  }

printf("\n");

for (i=0;i<pset->cnt;i++)

  {

  if ((nodos[i].grado!=2) ||

      (i!=nodos[nodos[i].vec[0]].vec[1] && i!=nodos[nodos[i].vec[0]].vec[0]) ||

      (i!=nodos[nodos[i].vec[1]].vec[1] && i!=nodos[nodos[i].vec[1]].vec[0]))

    {

    for (i=0;i<pset->cnt;i++)

      {

      printf("%i<%i>%i (%i)\n",nodos[i].vec[0],i,nodos[i].vec[1],nodos[i].grado);

      }

    exit(0);

    }

  }

}



