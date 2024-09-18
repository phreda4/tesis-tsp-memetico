#include <stdlib.h>
#include <stdio.h>
#include "time.h"
#include "stdtmp.h"

void main()
{
Time timeNow,timeIni,timePas,timeRun(30,0);

timeNow.now();
timeIni=timeNow;

printf("\n\n\n");
char *tarea="meme(20,mf,dpx,flk,4o)";
printf("tarea:%s\n",tarea);
printf("parse(tarea,mf,nn,-1)=%i\n",parse(tarea,"mf,nn",-1));
printf("parse(tarea,lk,flk,-1)=%i\n",parse(tarea,"lf,flk",-1));
printf("parse1(tarea,lk,nn,-1)=%i\n",parse1(tarea,"lk,nn",-1));
printf("parse(tarea,n:*,-1)=%i\n",parse(tarea,"n:%",-1));
char nuevo[80];
printf("extract((),tarea,nuevo)=%s\n",extract("()",tarea,nuevo));
printf("nuevo=%s\n",nuevo);

printf("tiempo:");timePas.print();printf("  ");
printf("rand:%f %i\n",randomFloat(5,10),randomInt(5,11));
}
