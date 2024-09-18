////////////////////////////////////////////////////////////////////
//
//  PointSet. Para manejar conjuntos de puntos
//
////////////////////////////////////////////////////////////////////
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "pointset.h"
#include "aalgo.h"
//
//  Constructor
//
////////////////////////////////////////////////////////////////////
PointSet::PointSet():kdtree()
{
point=NULL;
cacheC=NULL;
cacheV=NULL;
cnt=0;
}
//
//  Limpiar 
//
////////////////////////////////////////////////////////////////////
void PointSet::clear()
{
if (point!=NULL) 
  {
  delete point;
  delete cacheC;
  delete cacheV;
  point=NULL;
  cacheC=NULL;
  cacheV=NULL;
  cnt=0;
  }
}
//
//  Definir cantidad de puntos
//
////////////////////////////////////////////////////////////////////
void PointSet::size(int nro)
{
if (point!=NULL) 		// control de usado
  {
  delete point;
  delete cacheC;
  delete cacheV;
  }
cnt=nro;			// cantidad de puntos
point=new Point[cnt];	// reserva lugar para puntos 

cntCache=1;
while(cntCache<=nro)
  cntCache*=2;			// tamanio del cache
cacheC=new int[cntCache];	// reserva lugar
cacheV=new float[cntCache];	// para cache

for (int i=0;i<cntCache;i++)	// limpieza del cache
  cacheC[i]=-1;
}
//
//  Cargar desde disco el archivo .TSP 
//
////////////////////////////////////////////////////////////////////
void PointSet::load(char *n)
{
FILE *fp;
char buffer[80],car;
int i;
int salir=0; 
int cntCiudades;

fp=fopen(n,"rt");       // Abrir el archivo de instancia
if (fp==NULL)
  {
  printf("Error en PointSet.\n");
  printf("No existe o no se pudo cargar la instancia %s.\n",n);
  exit(-1);
  }
do {
  fscanf(fp,"%s",buffer);
  if ((strncasecmp("DIMENSION",buffer,9)==0))
    {
    fscanf(fp," :");
    if (fscanf(fp,"%d", &cntCiudades)!=1) 
      { fclose(fp);return; } 
    }
  else if ((strncasecmp("EDGE_WEIGHT_TYPE",buffer,16)==0))
    {
    fscanf(fp," :");
    fscanf(fp,"%s",buffer);
    if (strncasecmp(buffer,"att",3)==0)		distanceInt=&PointSet::distIntAtt;
    else if (strncasecmp(buffer,"man",3)==0)	distanceInt=&PointSet::distIntMan;
    else if (strncasecmp(buffer,"max",3)==0)	distanceInt=&PointSet::distIntMax;
    else if (strncasecmp(buffer,"geo",3)==0)    distanceInt=&PointSet::distIntGeo;
    else					distanceInt=&PointSet::distIntEuc;
    }
  else if ((strncasecmp("COMMENT",buffer,7) == 0))
    {
    fscanf(fp," :");
    i=0;
    while((car=fgetc(fp))!='\n')
      name[i++]=car;
    name[i++]=0;
    }
  else if (strncasecmp("NODE_COORD_SECTION",buffer,17) == 0)
    {
    if (cntCiudades<1)
      { fclose(fp);return; }
    salir=1;
    }
} while (salir==0);

int l,xi,yi;
float xf,yf;
xmax=MINFLOAT;          // inicializar limites
xmin=MAXFLOAT;
ymax=MINFLOAT;
ymin=MAXFLOAT;
size(cntCiudades);
for (i=0;i<cntCiudades;i++)
  {
  if (fscanf(fp,"%ld%g%g",&l,&xf,&yf)!=3)
    { fclose(fp);return; }
  point[i].x=xf;          // Guardar ciudad
  point[i].y=yf;
  point[i].del=point[i].look=0; // Limpiar los flags
  if (xf<xmin) xmin=xf; // actualizar limites
  if (xf>xmax) xmax=xf;
  if (yf<ymin) ymin=yf;
  if (yf>ymax) ymax=yf;
  }
fclose(fp);             // cerrar el archivo de instancia
cntDelete=0;            // ningun punto borrado
kdtree.build(this);     // Construyo kdtree
}
//
//  Grabar a archivo .TSP
//
////////////////////////////////////////////////////////////////////
void PointSet::save(char *n)
{
FILE *fp;

fp=fopen(n,"w+t");      // Abrir archivo para grabar
if (fp==NULL)
  {
  printf("Error en PointSet.\n");
  printf("No puedo grabar la instancia %s.\n",n);
  exit(-1);
  }
fprintf(fp,"DIMENSION :%i\n",cnt);
fprintf(fp,"EDGE_WEIGHT_TYPE : EUC\n");
fprintf(fp,"COMMENT : Instancia generada\n");
fprintf(fp,"NODE_COORD_SECTION\n");

int i;
for (i=0;i<cnt;i++)
  fprintf(fp,"%ld %g %g\n",i+1,point[i].x,point[i].y);
fclose(fp);       // Cerrar archivo
}
////////////////////////////////////////////////////////////////////
int PointSet::addPoint(int i,float x,float y)
{
for(int j=0;j<i;j++)                    // lo siento... O(i)
  if (point[j].x==x && point[j].y==y)
    return -1;                          // ya existe este punto
point[i].x=x;
point[i].y=y;
return 0;
}
//
//  Funciones para calculo de distancia en distintas metricas
//
////////////////////////////////////////////////////////////////////
int PointSet::distIntMax(int i,int j)
{
float xd=fabs(point[i].x-point[j].x);
float yd=fabs(point[i].y-point[j].y);
return (xd<yd)?(int)yd:(int)xd;
}
////////////////////////////////////////////////////////////////////
int PointSet::distIntMan(int i,int j)
{
float xd=fabs(point[i].x-point[j].x);
float yd=fabs(point[i].y-point[j].y);
return (int)(xd+yd+0.5);
}
////////////////////////////////////////////////////////////////////
int PointSet::distIntEuc(int i,int j)
{
float dx=point[i].x-point[j].x;
float dy=point[i].y-point[j].y;
return (int)(hypot(dx,dy)+0.5);
} 
////////////////////////////////////////////////////////////////////
int PointSet::distIntAtt(int i,int j)
{
float dx=point[i].x-point[j].x;
float dy=point[i].y-point[j].y;
float v=sqrt((dx*dx+dy*dy)/10.0);
int e=(int)v;
if (e<v)
  return e+1;
return e;
}
////////////////////////////////////////////////////////////////////
int PointSet::distIntGeo(int i,int j)
{
float PI=3.141592;
float lati,loni,latj,lonj,min;
int deg;
deg =(int)point[i].x;
min =point[i].x - deg;
lati=(PI*(deg+5.0*min)/3.0)/180.0;
deg =(int)point[i].y;
min =point[i].y - deg;
loni=(PI*(deg+5.0*min)/3.0)/180.0;
deg =(int)point[j].x;
min =point[j].x - deg;
latj=(PI*(deg+5.0*min)/3.0)/180.0;
deg =(int)point[j].y;
min =point[j].y - deg;
lonj=(PI*(deg+5.0*min)/3.0)/180.0;
float RRR=6378.388;
float q1=cos(loni-lonj);
float q2=cos(lati-latj);
float q3=cos(lati+latj);
return (int)(RRR*acos(0.5*((1.0+q1)*q2-(1.0-q1)*q3))+1.0);
}
//
//   Desborrar todos los puntos
//
////////////////////////////////////////////////////////////////////
void PointSet::undeleteAllPnt()
{
if (cntDelete==0)
  return;
for (int i=0;i<cnt;i++)
  point[i].del=point[i].look=0;
cntDelete=0;
kdtree.undeleteAll();
}
//
//   Desmarcar todos los puntos
//
////////////////////////////////////////////////////////////////////
void PointSet::unlookAllPnt()
{
for (int i=0;i<cnt;i++)
  point[i].look=0;
}
//
//  Calcular vecino mas cercano con enumeracion exaustiva
//
////////////////////////////////////////////////////////////////////
int PointSet::nnOld(int nro) // O(n)
{
int i;
float dist,mdist=MAXFLOAT;
int mvec=-1;
for (i=0;i<cnt;i++)
  if (i!=nro)
    {
    dist=calcDist(nro,i);
    if (dist<mdist)
      { mdist=dist;mvec=i; }
    }
return mvec;
}

