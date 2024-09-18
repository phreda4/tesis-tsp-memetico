#ifndef PQ_2
#define PQ_2

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "stdtmp.h"

class Pq2 {
  private:
    int		act,max;
    int		*ele;	// elementos
    float	*val;	// valores
   public:
    Pq2()
      { ele=NULL;val=NULL; }
    ~Pq2()
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
      ele=new int[cnt+2];
      val=new float[cnt+2];
      }
    void clear()
      { act=0; }
    void insert(float v,int e)
      {
      if (act<max)
        {
        act++;
        ele[act]=e;
        val[act]=v;
        upheap(act);
        }
      }
    void replace(float v,int e)         // cambiar v de e
      {
      }
    void remove(float *v,int *e)
      {
      *e=ele[1];
      *v=val[1];
      ele[1]=ele[act];
      val[1]=val[act];
      act--;
      downheap(1);
      }
    void del(int a)  // a es posicion
      {
      ele[a]=ele[act];
      val[a]=val[act];
      act--;
      downheap(a);
      }
    int empty()
      { return (act==0); } 
    int cnt()
      { return act; }
    int next(int a)             // siguiente en orden
      {
      float v=val[a];
      }
    int prev(int e,float *v)             // anterior en orden
      {
      }
    int first(float *v)                  // primer elemento
      {
      *v=val[1];
      return ele[1];
      }
    int last(float *v)                  // ultimo
      {
      *v=val[act-1];
      return ele[act-1];
      }
    int near(float val,float *v)        // mas cercano a
      {}
    void nth(int nro,float *v,int *e)	// elemento numero nro
      {
//      if (nro>act) return;	// comprobacion de limites
      int k,n=0,c=1;
      while (n<nro)
        {
        k=c*2;
        if (val[k]>val[k+1]) k++;
        n++;
        }
      *e=ele[c];
      *v=val[c];
      }

    void print()
      {
      for (int i=1;i<=act;i++)
        printf("%i(%2.1f),",(int)ele[i],val[i]);
      }
  private:
    void upheap(int k)
      {
      int e=ele[k];
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
      }
    void downheap(int k)
      {
      int j;
      int e=ele[k];
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
      }
  };



#endif
