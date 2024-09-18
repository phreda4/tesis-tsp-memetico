#include "pointset.h"
#include "tourc.h"
#include "stdtmp.h"
#include <stdlib.h>

int main()
{
PointSet pset;
Tour tour1,tour2;
pset.load("prueba.tsp");
tour1.init(&pset);
tour2.init(&pset);
for (int i=0;i<pset.cnt;i++)
  {
  tour1.addPoint(i);
  tour2.addPoint(i);
  }
int i,j,a,b,c,d;
//do {
  for (i=0;i<7;i++)
   {
   printf("%i \n",i);
   a=randomInt(0,pset.cnt-1);
   c=(a+randomInt(2,pset.cnt-2))%pset.cnt-1;
   b=tour1.next(a);
   d=tour1.next(c);
   tour1.flip(a,b,c,d);
   tour2.flip(a,b,c,d);
   } 
  tour1.makePermanent();
  tour2.makePermanent2();
  j=0;
//} while (j==0);  
tour1.print();
tour2.print();
return 0;3
}