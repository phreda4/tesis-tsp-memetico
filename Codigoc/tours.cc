/////////////////////////////////////////////////////////////////////
//
//  Tour
//
/////////////////////////////////////////////////////////////////////
#include "tour.h"

/////////////////////////////////////////////////////////////////////
void Tour::clear()
{
if (root!=NULL)
  {
  for(int i=0;i<cntSeg;i++)
    delete(sortS[i]);
  delete(sortS);
  }
if (tour!=NULL)
  delete tour;
if (position!=NULL)
  delete position;
tour=position=NULL;
}

/////////////////////////////////////////////////////////////////////
void Tour::init(PointSet *s)
{
pset=s;
clear();
//
//  Arreglo de ciudades y posiciones
//
tour=new int[pset->cntPoint];
position=new int[pset->cntPoint];
//
//  Nodo unico de inicio
//
root=new Segment(0,pset->cntPoint-1,0);
root->next=root->prev=root;
//
//  Arreglo ordenado de segmentos
//
maxSeg=MAXFLIPS*2+1;
cntSeg=1;
sortS=new (Segment*)[maxSeg];
sortS[0]=root;
//
//  Longitud total del tour
//
tourLenght=0;
cntTour=0;
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
  position[i]=t.position[i];
  tour[i]=t.tour[i];
  }
}

/////////////////////////////////////////////////////////////////////
void Tour::addPoint(int city)
{
position[city]=cntTour;
tour[cntTour]=city;

if (cntTour>0)
  tourLenght+=pset->calcDistC(tour[cntTour-1],city);

cntTour++;
if (cntTour==pset->cntPoint)
  tourLenght+=pset->calcDistC(city,tour[0]);
}

/////////////////////////////////////////////////////////////////////
void  Tour::flipPermanent(int a,int b,int c,int d)
{
}
//
//  b=next(a);
//  d=next(c);
//
/////////////////////////////////////////////////////////////////////
void Tour::flip(int ca,int cb,int cc,int cd)
{
int undo=0;
int na,nb,nc,nd;
Segment *sa,*sb,*sc,*sd;
Segment *s,*t;
//
//  Obtener las posiciones de las ciudades
//
int a=position[ca];
int b=position[cb];
int c=position[cc];
int d=position[cd];
//
//  Separar el segmento entre a y b (si es necesario)
//
na=findN(a);sa=sortS[na];
nb=findN(b);sb=sortS[nb];
if (sa==sb)	// partir los segmentos si es necesario
  {
  if (a<b)
    {
    sa=new Segment(sa->from,a,sa->invert);
    sb->from=b;
    // poner sa antes de sb
    insSort(sa,na);nb++;
    }
  else
    {
    sa=new Segment(a,sa->to,sa->invert);
    sb->to=a;
    insSort(sa,++na);
    }
  sa->prev=sb->prev;
  sb->prev=sa;
  sa->next=sb;
  sa->prev->next=sa;
  }
else
  undo++;
//
//  Separar el segmento entre c y d (si es necesario)
//
nc=findN(c);sc=sortS[nc];
nd=findN(d);sd=sortS[nd];
if (sc==sd)
  {
  if (c<d)
    {
    sd=new Segment(d,sd->to,sd->invert);
    sc->to=c;
    insSort(sd,++nd);
    }
  else
    {
    sd=new Segment(sd->from,d,sd->invert);
    sc->from=c;
    insSort(sd,nd);nc++;
    }
  sd->next=sc->next;
  sc->next=sd;
  sd->prev=sc;
  sd->next->prev=sd;
  }
else
  undo++;
//
//  Invertir los segmentos intermedios
//
if (sb==sc)
  sb->invert^=1;
else
  {
  for (s=sb;s!=sd;s=s->prev)
    {
    s->invert^=1;
    t=s->next;s->next=s->prev;s->prev=t;
    }
  }
sa->next=sc;
sc->prev=sa;
sb->next=sd;
sd->prev=sb;
//
//  controlar si puede ser un retroceso
//
if (undo==2)	
  {
  if (sa->invert==sc->invert && sa->to+1==sc->from)	
    {
    sa->to=sc->to;
    sa->next=sc->next;
    sa->next->prev=sa;
    if (sc==sb) 
      sb=sa;
    if (root==sc)
      root=sa;
    delete(sc);
    delSort(nc);
    }
  if (sb->invert==sd->invert && sb->to+1==sd->from)
    {
    sb->to=sd->to;
    sb->next=sd->next;
    sb->next->prev=sb;
    if (root==sd)
      root=sb;
    delete(sd);
    delSort(nd);
    }
  }
}

/////////////////////////////////////////////////////////////////////
int Tour::next(int a)
{
Segment *s=findS(a);
if (s->invert==0)
  {
  if (s->to==a)
    if (s->next->invert==0)
      return s->next->from;
    else
      return s->next->to;
  else
    return a+1;
  }
else
  {
  if (s->from==a)
    if (s->next->invert==0)
      return s->next->from;
    else
      return s->next->to;
  else
    return a-1;
  }
}

/////////////////////////////////////////////////////////////////////
int Tour::prev(int a)
{
Segment *s=findS(a);
if (s->invert==1)
  {
  if (s->to==a)
    if (s->prev->invert==1)
      return s->prev->from;
    else
      return s->prev->to;
  else
    return a+1;
  }
else
  {
  if (s->from==a)
    if (s->prev->invert==1)
      return s->prev->from;
    else
      return s->prev->to;
  else
    return a-1;
  }
}

/////////////////////////////////////////////////////////////////////
int Tour::between(int a,int b,int c)
{
}

/////////////////////////////////////////////////////////////////////
float Tour::dist(int a,int b)
{
}

/////////////////////////////////////////////////////////////////////
void Tour::print()
{
Segment *s=root;
do {
  printf("(%i %i %i)",s->from,s->to,s->invert);
  s=s->next;
} while(s!=root);
printf("\n");
for(int i=0;i<cntSeg;i++)
  printf("%i,",sortS[i]->from);
printf("\n");
}

/////////////////////////////////////////////////////////////////////
Segment *Tour::findS(int a)
{
int min=0,max=cntSeg-1;
int n=(max+min)/2;
Segment *s=sortS[n];
while (!(s->from<=a && s->to>=a))
  {
  if (s->from>a) 
    max=n-1;
  else
    min=n+1;
  n=(min+max)/2;
  s=sortS[n];
  }
return s;
}

/////////////////////////////////////////////////////////////////////
int Tour::findN(int a)
{
int min=0,max=cntSeg-1;
int n=(max+min)/2;
Segment *s=sortS[n];
while (!(s->from<=a && s->to>=a))
  {
  if (s->from>a) 
    max=n-1;
  else
    min=n+1;
  n=(min+max)/2;
  s=sortS[n];
  }
return n;
}

/////////////////////////////////////////////////////////////////////
void Tour::insSort(Segment *s,int n)
{
for (int i=cntSeg;i>n;i--)
  sortS[i]=sortS[i-1];
sortS[n]=s;
cntSeg++;
}

/////////////////////////////////////////////////////////////////////
void Tour::delSort(int n)
{
for (int i=n;i<cntSeg;i++)
  sortS[i]=sortS[i+1];
cntSeg--;
}
