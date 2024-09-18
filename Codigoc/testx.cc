#include "tourc.h"

const int o1[10]={5,3,9,1,2,8,0,6,7,4};
const int o2[10]={1,2,5,3,9,4,8,6,0,7};

void main()
{
PointSet pset;
pset.load("rand10.tsp");

Tour t1,t2,t3;

t1.init(&pset);t2.init(&pset);t3.init(&pset);

int i,j;

for (i=0;i<pset.cntPoint;i++)
  {
  t1.addPoint(o1[i]);
  t2.addPoint(o2[i]);
  }
t3.dpx(t1,t2);

printf("1:");t1.print();
printf("2:");t2.print();
printf("3:");t3.print();

t3.mfx(t1,t2);
printf("4:");t3.print();

}
