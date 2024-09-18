/////////////////////////////////////////////////////////////////////
//
//  Busqueda de movimientos 4 opt
//
/////////////////////////////////////////////////////////////////////
#include <string.h>
#include "s4.h"

////////////////////////////////////////////////////////////////////
void Search4::clear()  // Limpiar variables
{
if (listas!=NULL)
  {
  for (int i=0;i<pset->cnt;i++)
    listas[i].free();
  delete []listas;
  listas=NULL;
  }
}

////////////////////////////////////////////////////////////////////
void Search4::init(PointSet *ps) // Inicializar algoritmo
{
pset=ps;
}

////////////////////////////////////////////////////////////////////
void Search4::set(char *arg) // ajustar parametros
{
clear();
int t;

cntMaxima=2;

if (*arg!=0 && *arg!='+')
  {
  char *tok;	  // parse de opciones
  for (tok=strtok(arg," ,");tok;tok=strtok(0," ,"))
    {
    if (!strncasecmp(tok,"t:",2))
      sscanf(tok,"t:%i",&cntMaxima);
    }
  }
listas=new PQ<int>[pset->cnt];
for (int i=0;i<pset->cnt;i++)
  listas[i].init(cntMaxima);
}

////////////////////////////////////////////////////////////////////
void Search4::runS4(Tour *t)
{

//for (a=0;b<pset->cntPoint;b++)
//  {
//  }

}
////////////////////////////////////////////////////////////////////
void Search4::
{
}
int a,b,a1,b1,i,j,n=pset->cntPoint;
tour=t;
for (i=n-3;i>4;i--) // n
  {
  for (j=0;j<n/2;j++) // n/2
    {
    a=j;b=(j+i)%n;
    for (;b-a<4;)
      {
      a1=a;b1=b;
      a++;if (a>=n) a-=n;
      b--;if (b<0) b+=n;
      check(a1,b1,a,b);
      }
    }
  }
if ((n&0x1)==1)         // n es impar, falta 1 arco
  {
  a1=(n/2)+1;a=a1+1;
  b1=a+4;b=a+3;
  check(a1,b1,a,b);
  }
}
////////////////////////////////////////////////////////////////////
void Search4::check(int	a1,int b1,int a,int b)
{
int i,j,c;
int ca1=tour->city(a1);
int ca=tour->city(a);
int cb1=tour->city(b1);
int cb=tour->city(b);
float g,g1=tour->dist(ca1,ca)+tour->dist(cb,cb1)
          -pset->dist(ca1,cb1)-pset->dist(ca,cb);
listas[ca1].insert(g1,cb);
//lin
//stas[cb].insert(g1,ca1);
//if (b-a<n/2)              // hay menos ciudades entre b y a
//  {
  for (i=a+1;i<b-2;i++)     // ciudades entre a y b
    {
    PQ<int> *lista=&listas[tour->city(i)];
    for (j=0;j<lista->cnt();j++)
      {
      lista->getNth(j,&g,&c);
      if (a1-2<=c && c<=b-2) // NO esta bien
        if (g1+g>0)
  	{
  	tour->doubleBridge(a1,b,i,c);
          lista->setNth(i,g,c);
          lista->setNth(c,g,i);
          lista->setNth(a1,g,b);
          lista->setNth(b,g,a1);
  	}
      }
    }
//  }
//else    // ciudades entre b1 y a1
//  {
//  for (i=b1+1;i!=a1
//  }
}
