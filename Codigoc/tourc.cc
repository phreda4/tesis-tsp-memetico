/////////////////////////////////////////////////////////////////////
//
//  Tour con cache
//
/////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <string.h>
#include "stdtmp.h"
#include "tourc.h"
#include "aalgo.h"

/////////////////////////////////////////////////////////////////////
void Tour::clear()
{
if (tour!=NULL)
  {
  delete tour;
  delete position;
  delete cacheC;
  delete cacheV;
  }
tour=position=NULL;
cacheC=NULL;
cacheV=NULL;
}
/////////////////////////////////////////////////////////////////////
void Tour::init(PointSet *s)
{
pset=s;
clear();
tour=new int[pset->cnt];		//  Arreglo de ciudades 
position=new int[pset->cnt];	// y posiciones
tourLenght=0;				//  Longitud total del tour
cntTour=0;				// cnt de ciudades en el tour
cntFlips=0;				// cnt de flips
minimo=0;
cntCache=1;				
while (cntCache<=pset->cnt)	// Averiguar tamagno del cache
  cntCache*=2;
cacheC=new int[cntCache];		// para guardar ciudades de cache
cacheV=new float[cntCache];		// para guardar distancias " "

for (int i=0;i<cntCache;i++)
  cacheC[i]=-1;
}
/////////////////////////////////////////////////////////////////////
void Tour::copyOf(Tour &t)
{
tourLenght=t.tourLenght;
cntTour=t.cntTour;
for (int i=0;i<pset->cnt;i++)
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
  tourLenght+=dist(tour[cntTour-1],city);

cntTour++;
if (cntTour==pset->cnt)
  tourLenght+=dist(city,tour[0]);
}
/////////////////////////////////////////////////////////////////////
void Tour::flip(int a,int b,int c,int d)
{
if (cntFlips==MAXFLIPS)		// si se exede en flips
  makePermanent();		// hacerlos permenente
int pb=toVirtual(position[b]);	// obtener las posiciones
int pc=toVirtual(position[c]);	// verdaderas de las ciudades
if (pc<pb)				// pc siempre debe ser menor
  {
  pc=toVirtual(position[a]);
  pb=toVirtual(position[d]);
  }
Flip *s=&flips[cntFlips];
if (cntFlips>0)				// Fijarse si es un retroceso
  {
  Flip *p=s-1;
  if (p->from==pb && p->to==pc)
    {
    cntFlips--;				// disminuir cantidad de flips
    tourLenght=p->lenght;		// Ajustar la longitud del tour
    return;
    }
  s->min=(p->min>pb)?pb:p->min;		// Mantengo el minimo
  s->max=(p->max<pc)?pc:p->max;		// y maximo
  }
else
  {
  s->min=pb;
  s->max=pc;
  }
s->from=pb;s->to=pc;
s->add=pb+pc;
s->lenght=tourLenght;      // Ajustar la longitud del tour (linea de abajo)
tourLenght+=-dist(a,b)-dist(c,d)+pset->dist(a,c)+pset->dist(b,d);
cntFlips++;				// aumentar cantidad de flips
}
/////////////////////////////////////////////////////////////////////
void Tour::flip2(int a,int b,int c,int d)
{
if (cntFlips==MAXFLIPS)		// si se exede en flips
  makePermanent();		// hacerlos permenente
int pb=toVirtual(position[b]);	// obtener las posiciones
int pc=toVirtual(position[c]);	// verdaderas de las ciudades
if (pc<pb)				// pc siempre debe ser menor
  {
  pc=toVirtual(position[a]);
  pb=toVirtual(position[d]);
  }
Flip *A,*B;
int nA,nB;

A=&flips[cntFlips];nA=cntFlips;
A->from=pb;A->to=pc;
A->add=pb+pc;
A->lenght=tourLenght;      // Ajustar la longitud del tour (linea de abajo)

if (cntFlips>0)				// Fijarse si es un retroceso
  {
  B=A-1;nB=nA-1;
  if (B->from==pb && B->to==pc)
    {
    cntFlips--;				// disminuir cantidad de flips
    tourLenght=B->lenght;		// Ajustar la longitud del tour
    return;
    }
  A->min=(B->min>pb)?pb:B->min;		// Mantengo el minimo
  A->max=(B->max<pc)?pc:B->max;		// y maximo

  A->pmayor=A->pmenor=cntFlips-1;	// anterior menor y mayor
  if (A->from<=B->from)
    {
    nB--;B--;
    while (A->from<=B->from && nB>0)
      { nB--;B--; }
    A->pmenor=nB;
    }
  B=A-1;nB=nA-1;
  if (A->to>=B->to)
    {
    nB--;B--;
    while (A->to>=B->to && nB>0)
      { nB--;B--; }
    A->pmayor=nB;
    }
  A->nmayor=A->nmenor=cntFlips+1;
  for (nB=0/*minimo*/;nB<nA;nB++)
    {
    B=&flips[nB];
    if (B->nmenor>=cntFlips && A->from>=B->from)
      (B->nmenor)++;
    if (B->nmayor>=cntFlips && A->to<=B->to)
      (B->nmayor)++;
    }
  B=&flips[minimo];
  while (B->nmenor<cntFlips && B->nmayor<cntFlips)
    { minimo++;B=&flips[minimo]; }
  }
else
  {
  A->min=pb;A->max=pc;
  A->pmayor=A->pmenor=-1;
  A->nmayor=A->nmenor=1;
  }

tourLenght+=-dist(a,b)-dist(c,d)+pset->dist(a,c)+pset->dist(b,d);
cntFlips++;				// aumentar cantidad de flips
}
/////////////////////////////////////////////////////////////////////
void Tour::removeLastFlip()	// eliminar el ultimo flip
{
if (cntFlips==0)			// Si no hay, no hacer
  return;
cntFlips--;			// disminuir cantidad de flips
Flip *p=&flips[minimo];
while (p->nmenor>=cntFlips && p->nmayor>=cntFlips && minimo>0)
  { minimo--;p=&flips[minimo]; }
tourLenght=flips[cntFlips].lenght;// Ajustar la longitud del tour
}
//
//  Hacer permanente el cambio que esta en el cache
//
/////////////////////////////////////////////////////////////////////
void Tour::makePermanent()
{
if (cntFlips==0)
  return;
int min=flips[cntFlips-1].min;		// minimo nodo que cambio
int max=flips[cntFlips-1].max;		// maximo...
int i,j,gf,gt;
Flip *g,*f=flips;
for (i=0;i<cntFlips;i++,f++)	//  Hacer los flips en orden
   flipPhis(f->from,f->to); // version anterior

for (i=min;i<=max;i++)		// Ajuste de las posiciones
  position[tour[i]]=i;
cntFlips=0;			//  Vaciar el cache de flips
minimo=0;
}
/////////////////////////////////////////////////////////////////////
void Tour::makePermanent2()
{
if (cntFlips==0)
  return;
int min=flips[cntFlips-1].min;		// minimo nodo que cambio
int max=flips[cntFlips-1].max;		// maximo...
int i,j,gf,gt,add,from,to;
Flip *g,*f=flips;
for (i=0;i<cntFlips;i++,f++)	//  Hacer los flips en orden
  {
  if (f->to-f->from<=cntTour/2)
    flipPhis(f->from,f->to);	// Combiene de from a to
  else
    {		// flip inverso, invertir el cache tambien
    gf=f->to+1;if (gf==cntTour) gf=0;
    gt=f->from-1;if (gt<0) gt=cntTour-1;
    from=gf;to=gt;

    flipPhisI(gf,gt);
    add=f->add;if (add<cntTour) add+=cntTour;
    for (j=i+1,g=f+1;j<cntFlips;j++,g++) // recorrer el cache restante
       {
       if (g->to>=from || g->to<=to) 
         gf=add-g->to;
       else
         gf=f->add-g->to;
       if (gf>=cntTour) gf-=cntTour;
       if (gf<0) gf+=cntTour;

       if (g->from>=from || g->from<=to)  
         gt=add-g->from;
       else
         gt=f->add-g->from;
       if (gt>=cntTour) gt-=cntTour;
       if (gt<0) gt+=cntTour;

       if (gt>gf) 
         { g->from=gf;g->to=gt; }
       else 
         { 
         g->from=gt+1;if (g->from>=cntTour) g->from-=cntTour;
         g->to=gf-1;if (g->to<0) g->to+=cntTour;
         }
       g->add=g->from+g->to;
       }
    min=0;max=cntTour-1;
    }
  }
for (i=min;i<=max;i++)		// Ajuste de las posiciones
  position[tour[i]]=i;
cntFlips=0;			//  Vaciar el cache de flips
minimo=0;
}
/////////////////////////////////////////////////////////////////////
void Tour::flipPhis(int from,int to)
{
int i,j;
for (i=from,j=to;i<j;i++,j--)
  swap(tour[i],tour[j]);
}
/////////////////////////////////////////////////////////////////////
void Tour::flipPhisI(int from,int to)
{
int i,j,k;
i=to;j=from;
for (k=(cntTour-(from-to)+1)/2;k>0;k--)
  {
  swap(tour[i],tour[j]);
  j++;if (j==cntTour) j=0;
  i--;if (i<0) i=cntTour-1;
  }
}
/////////////////////////////////////////////////////////////////////
void Tour::test()
{
int i;
float f=0;
for (i=0;i<cntTour-1;i++)
  {
  f+=dist(tour[i],tour[i+1]);
  }
f+=dist(tour[cntTour-1],tour[0]);
if (f!=tourLenght)
  printf("*** %i != %i\n",f,tourLenght);
else
  printf("Ok\n");
}
/////////////////////////////////////////////////////////////////////
int Tour::intLenght()
{
int i;
int f=0;
for (i=0;i<cntTour-1;i++)
  f+=pset->distInt(tour[i],tour[i+1]);
f+=pset->distInt(tour[cntTour-1],tour[0]);
return f;
}
/////////////////////////////////////////////////////////////////////
void Tour::print()
{
int i;
for (i=0;i<cntTour;i++)
  printf("%i-",tour[toReal(i)]);
/*printf("\ntour:");
for (i=0;i<cntTour;i++)
  printf("%i",tour[i]);
printf("\nposi:");
for (i=0;i<cntTour;i++)
  printf("%i",position[i]);
printf("\n");
for (i=0;i<cntFlips;i++)
  printf("(%i %i)",flips[i].from,flips[i].to);*/
printf("\n");
}
/////////////////////////////////////////////////////////////////////
void Tour::save(char *name)
{
FILE *f=fopen(name,"w+t");
if (f==NULL)
  return;
if (cntFlips>0)                 // que no haya flips pendientes
  makePermanent();
int i;
for (i=0;i<cntTour;i++)
  fprintf(f,"%i\n",tour[i]);
fclose(f);
}
/////////////////////////////////////////////////////////////////////
int Tour::toVirtual(int i)
{
int j;
Flip *f=flips;
for (j=0;j<cntFlips;j++,f++)
  if (i>=f->from && i<=f->to)
    i=f->add-i;
return i;
}
/////////////////////////////////////////////////////////////////////
int Tour::toVirtual2(int i)
{
int j=0;
Flip *f;
while (j<cntFlips-1)
  {
  f=&flips[j];
  if (i>=f->from)
    if (i<=f->to) // dentro del intervalo
      { i=f->add-i;j++; }
    else  // mayor que el intervalo
      j=f->nmayor;
  else	// menor que el intervalo
    j=f->nmenor;
  }
return i;
}
/////////////////////////////////////////////////////////////////////
int Tour::toReal(int i)
{
int j;
Flip *f=&flips[cntFlips-1];
for (j=0;j<cntFlips;j++,f--)
  if (i>=f->from && i<=f->to)
    i=f->add-i;
return i;
}
/////////////////////////////////////////////////////////////////////
int Tour::toReal2(int i)
{
int j=cntFlips-1;
Flip *f;
while (j>0)
  {
  f=&flips[j];
  if (i>=f->from)
    if  (i<=f->to)  // dentro del intervalo
      { i=f->add-i;j--; }
    else	// mayor que el intervalo
      j=f->pmayor;
  else		// menor que el intervalo
    j=f->pmenor;
  }
return i;
}
/////////////////////////////////////////////////////////////////////
void Tour::dpx(Tour &t1,Tour &t2)  // xover dpx (AnB)-(A-B)-(B-A)
{
int vec[pset->cnt][2];  // arreglo para guardar vecinos
char grado[pset->cnt];
int i,f=0;

clearTour();
ANIMA("%s clear",name);
for (i=0;i<pset->cnt;i++)
  grado[i]=0;
for (i=0;i<pset->cnt;i++)
  if (t1.nextS(i)==t2.nextS(i) || t1.nextS(i)==t2.prevS(i))
    {
    f++;
    grado[i]++;
    grado[t1.nextS(i)]++;
    vec[i][0]=t1.nextS(i);      // marcar que esto son iguales
    vec[t1.nextS(i)][1]=i;      // hay un camino obligatorio
    }
  else
    {
    vec[i][0]=-(t1.nextS(i)+1);  // Ojo, sobrecarga semantica, negativo
    vec[t1.nextS(i)][1]=-(i+1);  // significa que es distino en los padres
    }
//printf("%f\n",(float)f/pset->cnt);
if (f==pset->cnt)          // los dos son iguales
  {
  copyOf(t1);              // el resultado es una copia
  return;
  }
int next,city=0;
while (grado[city]==2)          // no empezar en una ciudad grado 2
  city++;                       // porque podria hacer un lazo
int first=city;
int r0,r1,d0,d1;
do {
  addPoint(city);
  pset->deletePnt(city);                // borrar la ciudad que se utiliza
  next=vec[city][0];
  if (next<0 || pset->deleted(next))    // next 0 no sirve ?
    {
    next=vec[city][1];
    if (next<0 || pset->deleted(next))  // next 1 no sirve ?
      {
      r0=-vec[city][0]-1;               // quienes son next 0
      r1=-vec[city][1]-1;               // y next 1 ?

      d0=(r0>-1)?!pset->deleted(r0):0;  // estan borrados ?
      d1=(r1>-1)?!pset->deleted(r1):0;
  
      if (d0) pset->deletePnt(r0);      // borrarlos si hace falta
      if (d1) pset->deletePnt(r1);
      next=pset->nn(city);              // buscar el mas cerca
      if (d1) pset->undeletePnt(r1);    // desborrarlos si hace falta
      if (d0) pset->undeletePnt(r0);
      if (next==-1)                     // funciono ?
        next=pset->nn(city);            // buscar sin borrar prohibidos
      }
    }
  if (next!=-1)
    {
    ANIMA("%s add %i %i",name,city,next);
    city=next;            // ya tengo siguiente
    }
} while (next!=-1);     // hasta no encontrar alguno
ANIMA("%s add %i %i",name,city,first);
pset->undeleteAllPnt(); // limpiar el point set
}
/////////////////////////////////////////////////////////////////////
void Tour::dpxp(Tour &t1,Tour &t2)  // xover dpx (AnB)
{
int vec[pset->cnt][2];  // arreglo para guardar vecinos
char grado[pset->cnt];
int i,f=0;

clearTour();
ANIMA("%s clear",name);
for (i=0;i<pset->cnt;i++)
  grado[i]=0;
for (i=0;i<pset->cnt;i++)
  if (t1.nextS(i)==t2.nextS(i) || t1.nextS(i)==t2.prevS(i))
    {
    f++;
    grado[i]++;
    grado[t1.nextS(i)]++;
    vec[i][0]=t1.nextS(i);      // marcar que esto son iguales
    vec[t1.nextS(i)][1]=i;      // hay un camino obligatorio
    }
  else
    {
    vec[i][0]=-(t1.nextS(i)+1);  // Ojo, sobrecarga semantica, negativo
    vec[t1.nextS(i)][1]=-(i+1);  // significa que es distino en los padres
    }
float perc=((float)f/pset->cnt)/4;
//if (f==pset->cnt)          // los dos son iguales
//  {
//  copyOf(t1);              // el resultado es una copia
//  return;
//  }
int next,city=0;
while (grado[city]==2)          // no empezar en una ciudad grado 2
  city++;                       // porque podria hacer un lazo
int first=city;
do {
  addPoint(city);
  pset->deletePnt(city);                // borrar la ciudad que se utiliza
  next=vec[city][0];
  if (next<0 || pset->deleted(next))    // next 0 no sirve ?
    {
    next=vec[city][1];
    if (next<0 || pset->deleted(next))  // next 1 no sirve ?
      next=pset->nn(city);              // buscar el mas cerca
    else if (randomFloat(0,1)<perc)
      next=pset->nn(city);              // buscar el mas cerca
    }
  else if (randomFloat(0,1)<perc)
    next=pset->nn(city);              // buscar el mas cerca
  if (next!=-1)
    {
    ANIMA("%s add %i %i",name,city,next);    
    city=next;            // ya tengo siguiente
    }
} while (next!=-1);     // hasta no encontrar alguno    
ANIMA("%s add %i %i",name,city,first);
pset->undeleteAllPnt(); // limpiar el point set
}
/////////////////////////////////////////////////////////////////////
void Tour::amb(Tour &t1,Tour &t2)  // (A-B)
{
int vec[pset->cnt][2];  // arreglo para guardar vecinos
char grado[pset->cnt];
int f,i;

clearTour();
ANIMA("%s clear",name);
for (i=0;i<pset->cnt;i++)
  grado[i]=0;
for (i=0;i<pset->cnt;i++)
  if (t1.nextS(i)!=t2.nextS(i) && t1.nextS(i)!=t2.prevS(i))
    {
    f++;
    grado[i]++;
    grado[t1.nextS(i)]++;
    vec[i][0]=t1.nextS(i);      // este arco esta en A
    vec[t1.nextS(i)][1]=i;      // pero no en B
    }
  else
    {
    vec[i][0]=-(t1.nextS(i)+1);  // Ojo, sobrecarga semantica
    vec[t1.nextS(i)][1]=-(i+1);  // negativo=esta en B
    }
if (f==pset->cnt)          // los dos son iguales
  {
  copyOf(t1);
  return;
  }
int next,city=0;
while (grado[city]==2)          // no empezar en una ciudad grado 2
  city++;                      // porque podria hacer un lazo
//int r0,r1,d0,d1;
int first=city;
do {
  addPoint(city);
  pset->deletePnt(city);                // si esta borrado no afecta
  next=vec[city][0];
  if (next<0 || pset->deleted(next))
    {
    next=vec[city][1];
    if (next<0 || pset->deleted(next))
      {
//      r0=-vec[city][0]-1;
//      r1=-vec[city][1]-1;

//      d0=(r0>-1)?!pset->deleted(r0):0;
//      d1=(r1>-1)?!pset->deleted(r1):0;
  
//      if (d0) pset->deletePnt(r0);
//      if (d1) pset->deletePnt(r1);
      next=pset->nn(city);
//      if (d1) pset->undeletePnt(r1);
//      if (d0) pset->undeletePnt(r0);
//      if (next==-1)
//        next=pset->nn(city);
      }
    }
  if (next!=-1)
    {    
    ANIMA("%s add %i %i",name,city,next);        
    city=next;
    }
} while (next!=-1);
ANIMA("%s add %i %i",name,city,first);
pset->undeleteAllPnt();
}
/////////////////////////////////////////////////////////////////////
void Tour::inverover(Tour &t1,Tour &t2)  // Xover de Guo Tao y Michalewicz
{
int c,c1,ca,nc,nc1;
float laux=tourLenght;
c=randomInt(0,pset->cnt-1);
for (int i=0;i<MAXFLIPS-1;i++)
  {
//  if (randomFloat(0,1)<0.02)
//    c1=(c+randomInt(1,pset->cnt/4))%pset->cnt;
//  else
    c1=(randomInt(0,1))?t1.nextS(c):t2.nextS(c);
  nc=next(c);
  nc1=next(c1);
  flip(c,nc,c1,nc1);
  ANIMA("%s opt2 %i %i %i %i",name,c,nc,c1,nc1);  
  c=c1;
  }
//if (laux>tourLenght)
  makePermanent();
//else
//  removeAllFlips();
}
////////////////////////////////////////////////////////////////////
void Tour::mutate4r()
{
int f,i,j,p[4];
for(i=0;i<4;i++)        // buscar 4 puntos al azar que no se superpongan
  do {
    f=0;
    p[i]=randomInt(0,pset->cnt-1);
    for (j=0;j<i;j++)
      if (abs(p[j]-p[i])<3)     // deben estar alejados
        { f=1;break; }
  } while (f==1);
for(i=1;i<4;i++)        // ordenar los puntos
  for (j=0;j<i;j++)     // de menor a mayor
    if (p[i]<p[j])
      swap(p[i],p[j]);
doubleBridge(p[0],p[1],p[2],p[3]);  // hacer el cambio
}
/////////////////////////////////////////////////////////////////////
void Tour::mutate4o()
{
int cnt;
int a,b,c,d;
int na,nb,nc,nd;
int nab,nba,ncd,ndc;
float g1,g2;
a=randomInt(0,pset->cnt-1);     // Elegir primer puente
b=a+randomInt(4,pset->cnt-3);
nab=(b-a)-2;                    // ciudades permitidas
nba=pset->cnt-(nab+4);          // entre a y b y b y a
b=b%pset->cnt;
na=a+1;if (na==pset->cnt) na=0; // siguiente de a
nb=b+1;if (nb==pset->cnt) nb=0; // siguiente de b
g1=-dist(tour[a],tour[na])-dist(tour[b],tour[nb])     // costo
   +dist(tour[a],tour[nb])+dist(tour[na],tour[b]);
cnt=0;
do {	// buscar 4 puntos que sean lo mejor posible
  c=(na+randomInt(1,nab))%pset->cnt;    // Elegir 2do puente
  d=(nb+randomInt(1,nba))%pset->cnt;
  nc=c+1;if (nc==pset->cnt) nc=0;       // calcular las siguientes
  nd=d+1;if (nd==pset->cnt) nd=0;       // ciudades
  if (d>nc)
    ncd=(d-nc)-1;                       // calcular huecos
  else
    ncd=pset->cnt-(nc-d+1);
  ndc=pset->cnt-(ncd+4);
  g2=-dist(tour[c],tour[nc])-dist(tour[d],tour[nd])   // costo
       +dist(tour[c],tour[nd])+dist(tour[nc],tour[d]);

  if (g2<g1)        // quedarse con la mejor conbinacion
    {
    swap(a,c);swap(b,d);
    swap(na,nc);swap(nb,nd);
    swap(nab,ncd);swap(nba,ndc);
    swap(g2,g1);
    cnt=0;
    }
  else
    cnt++;
} while (cnt<20);

int i,j,p[4];
p[0]=a;p[1]=b;p[2]=c;p[3]=d;

for(i=1;i<4;i++)        // ordenar los puntos
  for (j=0;j<i;j++)     // de menor a mayor
    if (p[i]<p[j])
      swap(p[i],p[j]);
doubleBridge(p[0],p[1],p[2],p[3]);  // hacer el cambio
}
/////////////////////////////////////////////////////////////////////
void Tour::doubleBridge(int a,int c,int e,int g)
{
if (cntFlips>0)                 // que no haya flips pendientes
  makePermanent();

int b=a+1;if (b==pset->cnt) b=0;
int d=c+1;if (d==pset->cnt) d=0;
int f=e+1;if (f==pset->cnt) f=0;
int h=g+1;if (h==pset->cnt) h=0;
ANIMA("%s opt4 %i %i %i %i %i %i %i %i",name,tour[a],tour[b],tour[c],tour[d],
tour[e],tour[f],tour[g],tour[h]);  
//  Ajustar longitud del tour
tourLenght-=dist(tour[a],tour[b])+dist(tour[c],tour[d])
           +dist(tour[e],tour[f])+dist(tour[g],tour[h]);
tourLenght+=dist(tour[a],tour[f])+dist(tour[b],tour[e])
           +dist(tour[c],tour[h])+dist(tour[d],tour[g]);
int *tourAux=new int[pset->cnt];
int *ta=tourAux,*t,i,j;
for(j=b,t=&tour[j];j!=d;)     // de b a c
  {
  *ta=*t;ta++;j++;
  if (j==pset->cnt) { j=0;t=tour; } else t++;
  }
for(j=h,t=&tour[j];j!=b;)     // de h a a
  {
  *ta=*t;ta++;j++;
  if (j==pset->cnt) { j=0;t=tour; } else t++;
  }
for(j=f,t=&tour[j];j!=h;)     // de f a g
  {
  *ta=*t;ta++;j++;
  if (j==pset->cnt) { j=0;t=tour; } else t++;
  }
for(j=d,t=&tour[j];j!=f;)     // de d a e
  {
  *ta=*t;ta++;j++;
  if (j==pset->cnt) { j=0;t=tour; } else t++;
  }
delete(tour);           // borrar el tour
tour=tourAux;           // reemplazarlo con el nuevo
for (i=0;i<pset->cnt;i++)		// Ajuste de las posiciones
  position[tour[i]]=i;
}
