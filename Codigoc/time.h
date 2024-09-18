/////////////////////////////////////////////////////////////////////
//
//  Clase Tiempo
//
/////////////////////////////////////////////////////////////////////
#ifndef TIME_H
#define TIME_H

#include <sys/time.h>

class Time {
private:
  timeval ti;
  char str[20];

public:
  Time();
  Time(long s,long ms);

  void now();                   // retorna el tiempo actual
  void set(long s,long ms)
    { ti.tv_sec=s;ti.tv_usec=ms; }

  //  De orden basicas

  int operator<(Time&);
  int operator>(Time&);
  int operator==(Time&);

  //  De orden derivadas

  int operator!=(Time &t)
    { return !(*this==t); };
  int operator<=(Time &t)
    { return (*this<t || *this==t); };
  int operator>=(Time &t)
    { return (*this>t || *this==t); };

  // Algebraicas

  Time operator+(Time &t);
  Time operator-(Time t);

  char *toStr();
};

#endif
