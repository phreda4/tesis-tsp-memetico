/////////////////////////////////////////////////////////////////////
//
//      Test de comparacion entre kdtree y doble array
//
/////////////////////////////////////////////////////////////////////
#include "doblea.h"
#include "pointset.h"

/////////////////////////////////////////////////////////////////////
void main()
{
DobleA da;
PointSet ps;


ps.load("att532.tsp");

da.build(&ps);

int c1,c2,c3;
int ck=0,cd=0,cc=0;
for (int i=0;i<ps.cntPoint;i++)
  {
  c2=ps.nn(i);
//  c2=da.nn(i);
  c3=ps.nnOld(i);
  if (c2!=c3)
    cd++;
  if (c2!=c3)
    printf("(%i[%4.2f].%i[%4.2f])\n",c2,ps.calcDistC(i,c2),c3,ps.calcDistC(i,c3));
  }
printf("kdtree-old:%i\n",cd);
}
