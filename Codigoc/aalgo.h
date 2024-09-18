/////////////////////////////////////////////////////////////////////
//
//  Clase recolectora de datos para animacion de algoritmos
//
/////////////////////////////////////////////////////////////////////
#ifndef AALGO_H
#define AALGO_H

#include <stdarg.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

/////////////////////////////////////////////////////////////////////
class Aalgo {
  private:
    FILE *fout;
    struct timeval start,now;
    int activo;
  public:
    char string[100];
    Aalgo();
    ~Aalgo();
    void init(char *,char *);
    void out();
    void sub(struct timeval *,struct timeval *,struct timeval *);
    int run()
      { return activo; }
  };
  
/////////////////////////////////////////////////////////////////////  
extern Aalgo aalgo;  

#define ANIMA(x,arg...) {sprintf(aalgo.string,x,## arg);aalgo.out();}

#endif
