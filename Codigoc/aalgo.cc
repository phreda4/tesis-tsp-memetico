/////////////////////////////////////////////////////////////////////
//
//  Clase recolectora de datos para animacion de algoritmos
//
/////////////////////////////////////////////////////////////////////
#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "aalgo.h"

//
//  Instancia unica presente en todos los contextos que
//  incluyan "aalgo.h"
//
/////////////////////////////////////////////////////////////////////
Aalgo aalgo;

/////////////////////////////////////////////////////////////////////
Aalgo::Aalgo()
{
activo=0;
fout=NULL;
gettimeofday(&start,NULL);
}

/////////////////////////////////////////////////////////////////////
Aalgo::~Aalgo()
{
if (fout!=NULL)
  fclose(fout);		// Cerrar el archivo
}

/////////////////////////////////////////////////////////////////////
void Aalgo::init(char *instance,char *solve)
{
char name[80],*p;
strcpy(name,instance);
p=name;
while (*p!='.' && *p!=0) p++;	// buscar nombre de la instancia (hasta .)
*p=0;
strcat(name,"_");
strcat(name,solve);	// Agregar forma de resolucion
strcat(name,".anm");
p=name;
while (*p!=0) {         // convertir los + en _
  if (*p=='+') *p='_';
  p++;
  }
fout=fopen(name,"w+t");		// Abrir archivo de animacion para grabar
if (fout==NULL)
  {
  printf("No se puede abrir el archivo para animacion.\n");
  exit(-1);
  }
activo=1;			// Indicar que la animacion esta activa
}

/////////////////////////////////////////////////////////////////////
void Aalgo::sub(struct timeval *r,struct timeval *x,struct timeval *y)
{
if (x->tv_usec<y->tv_usec)
  {
  int nsec=(y->tv_usec-x->tv_usec)/1000000+1;
  y->tv_usec-=1000000*nsec;
  y->tv_sec+=nsec;
  }
if (x->tv_usec-y->tv_usec>1000000)
  {
  int nsec=(y->tv_usec-x->tv_usec)/1000000;
  y->tv_usec+=1000000*nsec;
  y->tv_sec-=nsec;
  }
r->tv_sec=x->tv_sec-y->tv_sec;
r->tv_usec=x->tv_usec-y->tv_usec;
}

/////////////////////////////////////////////////////////////////////
void Aalgo::out()
{
if (activo==0)  return;		// retornar si esta inactivo
struct timeval s;
gettimeofday(&now,NULL);	// Obtener tiempo
sub(&s,&now,&start);		// restar el tiempo actual al anterior
fprintf(fout,"%i %s\n",s.tv_usec/10,string);
start=now;				// el tiempo actual ahora sera anterior
fflush(fout);	// para evitar perdida de una linea (para debug)
}
