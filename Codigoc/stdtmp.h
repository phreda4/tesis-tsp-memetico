/////////////////////////////////////////////////////////////////////
//
//	Rutinas de apoyo
//
/////////////////////////////////////////////////////////////////////
#ifndef ESTTMP_H
#define ESTTMP_H

#include <stdlib.h>

#ifndef MAXFLOAT
#define MAXFLOAT        1e80            // El float maximo
#endif

#ifndef MINFLOAT
#define MINFLOAT        -MAXFLOAT       // El float minimo
#endif

/////////////////////////////////////////////////////////////////////
template <class T>
void swap(T &a,T &b)    // intercambiar dos variables de tipo <T>
{
T aux=a;a=b;b=aux;
}

/////////////////////////////////////////////////////////////////////
inline int randomInt(int a,int b) // Numero entero al azar entre a y b
{
return a+(int)((b-a+1)*(float)rand()/RAND_MAX);
}

/////////////////////////////////////////////////////////////////////
inline float randomFloat(float a,float b)  // Numero al azar entre a y b
{
return a+(b-a)*(float)rand()/RAND_MAX;
}


#endif
