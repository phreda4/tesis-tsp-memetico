/////////////////////////////////////////////////////////////////////
//
//  Tour listas
//
/////////////////////////////////////////////////////////////////////
#include "stdtmp.h"
#include "tour.h"

/////////////////////////////////////////////////////////////////////
void Tour::clear()
{
if (tour!=NULL)
  delete tour;
if (city!=NULL)
  delete city;
tour=NULL;
city=NULL;
}

/////////////////////////////////////////////////////////////////////
void Tour::init(PointSet *s)
{
pset=s;
clear();
//
//  Arreglo de ciudades y posiciones
//
city=new TourNode[pset->cntPoint];
tour=new int[pset->cntPoint];
//
//  Longitud total del tour
//
tourLenght=0;
cntTour=0;
cntFlips=0;
}

/////////////////////////////////////////////////////////////////////
void Tour::copyOf(Tour &t)
{
pset=t.pset;
tourLenght=t.tourLenght;
cntTour=t.cntTour;

int i;
for (i=0;i<pset->cntPoint;i++)
  {
  city[i]=t.city[i];
  tour[i]=t.tour[i];
  }
}

/////////////////////////////////////////////////////////////////////
void Tour::addPoint(int a)
{
tour[cntTour]=a;		// posicion de la ciudad en el tour
city[a].pos=cntTour;
cntTour++;
//
//  Si es la ultima ciudad...
//
if (cntTour==pset->cntPoint)
  {
  int i;
  TourNode *now;
  for (i=0;i<cntTour;i++)
    {
    now=&city[tour[i]];
    now->n[0]=tour[(i>0)?i-1:cntTour-1];
    now->n[1]=tour[(i<cntTour-1)?i+1:0];
    now->d[1]=city[now->n[1]].d[0]=pset->calcDistC(tour[i],now->n[1]);
    tourLenght+=now->d[1];
    }
  }
}

/////////////////////////////////////////////////////////////////////
void Tour::flip(int a,int b,int c,int d)
{
//
//  Obtener posiciones virtuales y banderas de invertido
//
int ia=0,ib=0,ic=0,id=0;
int sa=toVirtual(city[a].pos,ia);
int sb=toVirtual(city[b].pos,ib);
int sc=toVirtual(city[c].pos,ic);
int sd=toVirtual(city[d].pos,id);

//
//  Invertir si es necesario para que quede ordenado ..ab...cd..
//

//if (b!=next(a) || d!=next(c))
//  printf("No son consecutivos...\n");

if ((sa>sb && sa==sb+1) || (sa==0 && sb==cntTour-1))
  { swap(sa,sb);swap(a,b);swap(ia,ib); }
if ((sc>sd && sc==sd+1) || (sc==0 && sd==cntTour-1))
  { swap(sc,sd);swap(c,d);swap(ic,id); }

if (sb>sc)
  { 
  swap(sa,sc);swap(a,c);swap(ia,ic);
  swap(sb,sd);swap(b,d);swap(ib,id);
  }
//
//  Controlar que no sea retroceso
//
Flip *s;
if (cntFlips>0)
  s=&flips[cntFlips-1];
else
  s=NULL;
if (s!=NULL && s->a==sb && s->b==sc)	// necesita ev. por cortocircuito
  {
  cntFlips--;
  }
else
  {
  s=&flips[cntFlips];
  s->a=sb;
  s->b=sc;
  s->ab=sb+sc;
  s->l=s->e=s->g=cntFlips+1;
  cntFlips++;
  //
  //  Reacomodar caminos
  //
  }

TourNode *na=&city[a];
TourNode *nb=&city[b];
TourNode *nc=&city[c];
TourNode *nd=&city[d];
//
//  Hacer el cambio de punteros
//
tourLenght-=na->d[1^ia];		//  Restar los dos arcos
tourLenght-=nd->d[0^id];

na->n[1^ia]=c;
nc->n[1^ic]=a;
na->d[1^ia]=nc->d[1^ic]=pset->calcDistC(a,c);

nb->n[0^ib]=d;
nd->n[0^id]=b;
nb->d[0^ib]=nd->d[0^id]=pset->calcDistC(d,b);

tourLenght+=na->d[1^ia];		// Sumar los arcos
tourLenght+=nb->d[0^ib];		// que entran

if (cntFlips==MAXFLIPS)
  makePermanent();
}

/////////////////////////////////////////////////////////////////////
void Tour::makePermanent() // O(n) !!!
{
int i,prev,now;
TourNode *n=&city[0];

now=0;
prev=n->n[0];
for (i=0;i<cntTour;i++)
  {
  tour[i]=now;
  n->pos=i;

  if (n->n[1]==prev)
    {
    swap(n->n[0],n->n[1]);
    swap(n->d[0],n->d[1]);
    }

  prev=now;

  now=n->n[1];
  n=&city[now];
  }
//
//  Vaciar flips
//
cntFlips=0;
}

/////////////////////////////////////////////////////////////////////
void Tour::print()
{
printf("dump Tour %f\n",tourLenght);
int i,a,b;
for (i=0;i<cntTour;i++)
  printf("%i",tour[i]);
printf(" ");
a=0;
for (i=0;i<cntTour;i++,a=next(a))
  printf("%i",a);
printf("\n");
for (i=0;i<cntTour;i++)
  printf("%i ",i);
printf("\n");
for (i=0;i<cntTour;i++)
  printf("%i ",city[i].n[0]);
printf("\n");
for (i=0;i<cntTour;i++)
  printf("%i ",city[i].n[1]);
printf("\n");
for (i=0;i<cntTour;i++)
  {
  b=0;
  a=toVirtual(city[i].pos,b);
  printf("%c ",b+'*');
  }
printf("\n");
}

/////////////////////////////////////////////////////////////////////
int Tour::toVirtual(int i,int &v)
{
int now=0;
Flip *f;
while (now<cntFlips)
  {
  f=&flips[now];
  if (i<f->a)
    now=f->l;
  else if (i>f->b)
    now=f->g;
  else
    {
    now=f->e;
    i=f->ab-i;
    v^=1;
    }
  }
return i;
}

/////////////////////////////////////////////////////////////////////
void Tour::test()
{
float l=0;
for (int i=0;i<cntTour-1;i++)
  {
  l+=pset->calcDistC(tour[i],tour[i+1]);
  }
l+=pset->calcDistC(tour[0],tour[cntTour-1]);

printf("%f %f\n",tourLenght,l);  
}

