////////////////////////////////////////////////////////////////////
//
//  Clase para manejar Kd-Trees
//
////////////////////////////////////////////////////////////////////
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "pointset.h"
#include "stdtmp.h"
//#include "kdtree.h" no se debe incluir porque genera referencia circular
#include "pq.h"

////////////////////////////////////////////////////////////////////
KdTree::KdTree()
{
perm=NULL;
root=NULL;
bucketptr=NULL;
}

////////////////////////////////////////////////////////////////////
KdTree::~KdTree()
{
clear();
}

////////////////////////////////////////////////////////////////////
void KdTree::clear()
{
if (perm!=NULL)
  { delete(perm);delete(bucketptr); }
perm=NULL;bucketptr=NULL;
}

/////////////////////////////////////////////////////////////////////
void KdTree::build(PointSet *ps)
{
int i,j;

pset=ps;
clear();

// Ajustar el tamanio del arreglo igual a la cantidad de puntos del PointSet
perm=new int[pset->cnt];
bucketptr=new KdNode*[pset->cnt];

for (i=0;i<pset->cnt;i++)
  perm[i]=i;
root=buildr(0,pset->cnt-1);
root->father=0;
}

/////////////////////////////////////////////////////////////////////
KdNode *KdTree::buildr(int l,int u)
{
int m,i;
KdNode *p=new KdNode;

if (u-l+1<=CUTOFF)
  {
  p->bucket=1;
  p->lopt=l;
  p->hipt=p->hiptn=u;
  for (m=l;m<=u;m++)
    bucketptr[perm[m]]=p;
  }
else
  {
  p->bucket=0;
  p->cutdim=findMaxSpread(l,u);
  m=(l+u)/2;
  select(l,u,p->cutdim);
  p->cutval=pset->px(perm[m],p->cutdim);

  p->loson=buildr(l,m);
  p->loson->father=p;
  p->hison=buildr(m+1,u);
  p->hison->father=p;
  }
return p;
}

////////////////////////////////////////////////////////////////////
void KdTree::select(int primero,int ultimo,int d)
{
int i,*p;
float aux;
PQ<int> heap;
heap.init(ultimo-primero+1);
for (i=primero,p=&perm[i];i<=ultimo;i++,p++)
  heap.insert(pset->px(*p,d),*p);
for (i=primero,p=&perm[i];i<=ultimo;i++,p++)
  heap.remove(&aux,p);
}
//
//  Retorna la dimension en que esta mas disperso
//
////////////////////////////////////////////////////////////////////
int KdTree::findMaxSpread(int l,int u)
{
float MaxY = MINFLOAT;
float MinY = MAXFLOAT;
float MaxX = MINFLOAT;
float MinX = MAXFLOAT;
int i;
for (i=l;i<=u;i++)
  {
  if (pset->x(perm[i])>MaxX) MaxX=pset->x(perm[i]);
  if (pset->y(perm[i])>MaxY) MaxY=pset->y(perm[i]);
  if (pset->x(perm[i])<MinX) MinX=pset->x(perm[i]);
  if (pset->y(perm[i])<MinY) MinY=pset->y(perm[i]);
  }
return ((MaxY-MinY)>(MaxX-MinX));	// 1=y 0=x
}

////////////////////////////////////////////////////////////////////
int KdTree::nn(int j)
{
KdNode *p,*lastp;
float diff;
int test,cota;

nnptnum=-1;
nntarget=j;
nndist=MAXFLOAT;
p=bucketptr[nntarget];
rnn(p);
test=0;
while (1) {
  lastp=p;
  p=p->father;
  if (p==0)
    break;
  diff=pset->px(nntarget,p->cutdim)-p->cutval;
  if (diff<0)
    { cota=0;diff=-diff; }
  else
    cota=2;
  if (nndist>=diff)
    {
    if (lastp==p->loson)
      rnn(p->hison);
    else
      rnn(p->loson);
    }
  else				// este es el test que evita el uso de 
    {				// Cut Planes (faltan casos de los bordes)
    test|=(1<<p->cutdim+cota);
    if (test==0xf)
      break;
    }
  }
return nnptnum;
}

////////////////////////////////////////////////////////////////////
void KdTree::rnn(KdNode *p)
{
float dist,diff;
if (p->bucket)
  {
  for (int i=p->lopt;i<=p->hiptn;i++)
    if (perm[i]!=nntarget) // && !pset->deleted(perm[i]))
      {
      dist=pset->dist(perm[i],nntarget);
      if (dist<nndist) 
        {
        nndist=dist;
        nnptnum=perm[i];
        }
      }
  }
else
  {
  diff=pset->px(nntarget,p->cutdim)-p->cutval;
  if (diff<0)
    {
    rnn(p->loson);
    if (nndist>=-diff)
      rnn(p->hison);
    }
  else
    {
    rnn(p->hison);
    if (nndist>=diff)
      rnn(p->loson);  
    }
  }
}

////////////////////////////////////////////////////////////////////
int KdTree::nnBallFirst(int pt,float r)
{
float diff;

ballList.clear();
ballListD.clear();

nndist=r;
nntarget=pt;
KdNode *lastp,*p=bucketptr[nntarget];
rnnBall(p);
while (1) {
  lastp=p;
  p=p->father;
  if (p==0)
    break;
  diff=pset->px(nntarget,p->cutdim)-p->cutval;
  if (lastp==p->loson) 
    {
    if (nndist>=-diff)
      rnnBall(p->hison);
    }
  else
    {
    if (nndist>=diff)
      rnnBall(p->loson);
    }
  }
return nnBall();
}

////////////////////////////////////////////////////////////////////
void KdTree::rnnBall(KdNode *p)
{
float dist,diff;
if (p->bucket)
  {
  for (int i=p->lopt;i<=p->hiptn;i++)
    if (perm[i]!=nntarget) // && !pset->deleted(perm[i]))
      {
      dist=pset->dist(perm[i],nntarget);
      if (dist<nndist) 
        {
        ballList.push(perm[i]);
        ballListD.push(dist);
        }
      }
  }
else
  {
  diff=pset->px(nntarget,p->cutdim)-p->cutval;
  if (diff<0)
    {
    rnnBall(p->loson);
    if (nndist>=-diff)
      rnnBall(p->hison);
    }
  else
    {
    rnnBall(p->hison);
    if (nndist>=diff)
      rnnBall(p->loson);  
    }
  }
}

////////////////////////////////////////////////////////////////////
int KdTree::nnBall()
{
if (ballList.empty())
  return -1;
nnptnum=ballList.pop();
nndist=ballListD.pop();
return nnptnum;
}

////////////////////////////////////////////////////////////////////
void KdTree::nnBallPQ(int pt,float r,PQ<int> &pq)
{
float diff;

nndist=r;
nntarget=pt;
KdNode *lastp,*p=bucketptr[nntarget];
rnnBallPQ(p,pq);
while (1) {
  lastp=p;
  p=p->father;
  if (p==0)
    break;
  diff=pset->px(nntarget,p->cutdim)-p->cutval;
  if (lastp==p->loson) 
    {
    if (nndist>=-diff)
      rnnBallPQ(p->hison,pq);
    }
  else
    {
    if (nndist>=diff)
      rnnBallPQ(p->loson,pq);
    }
  }
}

////////////////////////////////////////////////////////////////////
void KdTree::rnnBallPQ(KdNode *p,PQ<int> &pq)
{
float dist,diff;
if (p->bucket)
  {
  for (int i=p->lopt;i<=p->hiptn;i++)
    if (perm[i]!=nntarget) // && !pset->deleted(perm[i]))
      {
      dist=pset->dist(perm[i],nntarget);
      if (dist<nndist)
        pq.insert(dist,perm[i]);
      }
  }
else
  {
  diff=pset->px(nntarget,p->cutdim)-p->cutval;
  if (diff<0)
    {
    rnnBallPQ(p->loson,pq);
    if (nndist>=-diff)
      rnnBallPQ(p->hison,pq);
    }
  else
    {
    rnnBallPQ(p->hison,pq);
    if (nndist>=diff)
      rnnBallPQ(p->loson,pq);  
    }
  }
}

////////////////////////////////////////////////////////////////////
void KdTree::deletePnt(int n)        // borrar un punto
{
KdNode *p=bucketptr[n];
int i;
for (i=p->lopt;i<=p->hiptn;i++)
  if (perm[i]==n)
    {
    swap(perm[i],perm[p->hiptn]);
    p->hiptn--;
    break;
    }
}

////////////////////////////////////////////////////////////////////
void KdTree::undeletePnt(int n)      // desborrar un punto
{
KdNode *p=bucketptr[n];
int i;
for (i=p->hiptn;i<=p->hipt;i++)
  if (perm[i]==n)
    {
    p->hiptn++;
    swap(perm[i],perm[p->hiptn]);
    break;
    }
}

////////////////////////////////////////////////////////////////////
void KdTree::undeleteAll()         // desborrar todo
{
undeleteAll(root);
}

////////////////////////////////////////////////////////////////////
void KdTree::undeleteAll(KdNode *p)
{
if (p->bucket)
  p->hiptn=p->hipt;
else
  {
  undeleteAll(p->hison);
  undeleteAll(p->loson);
  }
}
