/////////////////////////////////////////////////////////////////////
//
//  Clase Tiempo
//
/////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include "time.h"

/////////////////////////////////////////////////////////////////////
Time::Time()
{
ti.tv_sec=0;
ti.tv_usec=0;
}

/////////////////////////////////////////////////////////////////////
Time::Time(long s,long us)
{
ti.tv_sec=s;
ti.tv_usec=us;
}

/////////////////////////////////////////////////////////////////////
void Time::now()
{
gettimeofday(&ti,NULL);
}

/////////////////////////////////////////////////////////////////////
int Time::operator<(Time &t)
{
return (ti.tv_sec<t.ti.tv_sec)?1:
       (ti.tv_sec==t.ti.tv_sec && ti.tv_usec<t.ti.tv_usec)?1:0;
}

/////////////////////////////////////////////////////////////////////
int Time::operator>(Time &t)
{
return (ti.tv_sec>t.ti.tv_sec)?1:
       (ti.tv_sec==t.ti.tv_sec && ti.tv_usec>t.ti.tv_usec)?1:0;
}

/////////////////////////////////////////////////////////////////////
int Time::operator==(Time &t)
{
return (ti.tv_sec==t.ti.tv_sec && ti.tv_usec==t.ti.tv_usec);
}

/////////////////////////////////////////////////////////////////////
Time Time::operator+(Time &t)
{
Time n(ti.tv_sec+t.ti.tv_sec,ti.tv_usec+t.ti.tv_usec);
if (n.ti.tv_usec>1000000)
  {
  ldiv_t c=ldiv(n.ti.tv_usec,1000000);
  n.ti.tv_usec-=1000000*c.quot;
  n.ti.tv_sec+=c.quot;
  }
return n;
}

/////////////////////////////////////////////////////////////////////
Time Time::operator-(Time t)
{
if (ti.tv_usec<t.ti.tv_usec)
  {
  int nsec=(t.ti.tv_usec-ti.tv_usec)/1000000+1;
  t.ti.tv_usec-=1000000*nsec;
  t.ti.tv_sec+=nsec;
  }
if (ti.tv_usec-t.ti.tv_usec>1000000)
  {
  int nsec=(t.ti.tv_usec-ti.tv_usec)/1000000;
  t.ti.tv_usec+=1000000*nsec;
  t.ti.tv_sec-=nsec;
  }
return Time(ti.tv_sec-t.ti.tv_sec,ti.tv_usec-t.ti.tv_usec);
}

/////////////////////////////////////////////////////////////////////
char *Time::toStr()
{
sprintf(str,"%4i:%-6i",ti.tv_sec,ti.tv_usec);
return str;
}
