/////////////////////////////////////////////////////////////////////
//
//  Clase para manejar una lista de prioridad (menor a mayor)
//
//	Definicion:
//		PQ<T> pqt;
//	Inicializacion 
//		pqt.init(Cantidad maxima de elementos);
//		pqt.clear();	// Vaciar pq
//		
//		pqt.insert(clave (float),elemento (class T));
//		pqt.remove(&clave (float),&elemento (class T));
//		pqt.empty==TRUE(1) // Si pq esta vacia
//
//		cantidad de elementos en pq=pqt.cnt();
//		pqt.nth(nro (int),&clave (float),&elemento (class T));
//
/////////////////////////////////////////////////////////////////////
#ifndef PQ_H
#define PQ_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "stdtmp.h"

template <class T>
class PQ {
  private:
    int		act,max;
    T		*ele;	// elementos
    float	*val;	// valores
   public:
    PQ()
      { ele=NULL;val=NULL; }
    ~PQ()
      { free(); }
    void free()
      {
      if (ele!=NULL)
        { delete val;delete ele; }      
      val=NULL;ele=NULL;
      }
    void init(int cnt)
      { 
      free();
      act=0;max=cnt;      
      ele=new T[cnt+2];
      val=new float[cnt+2];
      }
    void clear()
      { act=0; }
    void insert(float v,T e)
      {
      if (act<max)
        act++;
      ele[act]=e;
      val[act]=v;
      upheap(act);
      }
    void remove(float *v,T *e)
      {
      *e=ele[1];
      *v=val[1];
      ele[1]=ele[act];
      val[1]=val[act];
      act--;
      downheap(1);
      }
    int empty()
      { return (act==0); } 
    int cnt()
      { return act; }
    void dump()
      {
      for (int i=1;i<act;i++)
        printf("%i,",ele[i]);
      printf("\n");
      }
  private:
    int upheap(int k)
      {
      T e=ele[k];
      float v=val[k];
      val[0]=MINFLOAT;
      while (val[k/2]>v)
        { 
        ele[k]=ele[k/2];
        val[k]=val[k/2];
        k=k/2; 
        }
      ele[k]=e;
      val[k]=v;
      return k;
      }
    int downheap(int k)
      {
      int j;
      T e=ele[k];
      float v=val[k];
      while (k<=act/2)
        {
        j=2*k;
        if (j<act && val[j]>val[j+1]) j++;
        if (v<=val[j]) break;
        ele[k]=ele[j];
        val[k]=val[j];
        k=j;
        }
      ele[k]=e;
      val[k]=v;
      return k;
      }
  };

#endif