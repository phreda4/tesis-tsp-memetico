/////////////////////////////////////////////////////////////////////
//
//  Clase para manejar una pila FIFO
//
/////////////////////////////////////////////////////////////////////
#ifndef FIFO_H
#define FIFO_H

#include <stdlib.h>

/////////////////////////////////////////////////////////////////////
template <class T>
class FifoNode {
  public:
    T thing;
    FifoNode<T> *next;
  };

/////////////////////////////////////////////////////////////////////  
template <class T>
class Fifo {
  private:
    FifoNode<T> *first,*last;
  public:
    Fifo()
      { first=last=NULL; }
    ~Fifo()
      { clear(); }
    int empty()
      { return (first==NULL); }
    void clear();
    void push(T);
    T pop();
  };

/////////////////////////////////////////////////////////////////////
template <class T>
void Fifo<T>::clear()
{
last=first;
while (last!=NULL)
  {
  first=last;
  last=last->next;
  delete first;
  }
 first=last=NULL;
}

/////////////////////////////////////////////////////////////////////
template <class T>
void Fifo<T>::push(T e)
{
FifoNode<T> *aux=new FifoNode<T>;
aux->next=NULL;
aux->thing=e;
if (first==NULL)
  first=last=aux;
else
  {
  last->next=aux;
  last=aux;
  }
}

/////////////////////////////////////////////////////////////////////
template <class T>
T Fifo<T>::pop()	
{
T thing=first->thing;
FifoNode<T> *aux=first;
first=first->next;
if (first==NULL)
  last=NULL;
delete aux;
return thing;
}

#endif