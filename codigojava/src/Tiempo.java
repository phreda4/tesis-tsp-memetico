//////////////////////////////////////////////////////////////////////
//
//      Tiempo.java
//      Clase para manerar el tiempo
//
//////////////////////////////////////////////////////////////////////

import java.lang.*;

//////////////////////////////////////////////////////////////////////
class Tiempo {

    private final static String zeros = "000000";
    private int h, m, s, ms;

    //////////////////////////////////////////////////////////////////////
    public Tiempo() {
        h = m = s = ms = 0;
    }

    //////////////////////////////////////////////////////////////////////
    public Tiempo(int h1, int m1, int s1, int ms1) {
        h = h1;
        m = m1;
        s = s1;
        ms = ms1;
    }

    //////////////////////////////////////////////////////////////////////
    public Tiempo(Tiempo t) {
        h = t.h;
        m = t.m;
        s = t.s;
        ms = t.ms;
    }

    //////////////////////////////////////////////////////////////////////
    public int getH() {
        return h;
    }

    //////////////////////////////////////////////////////////////////////
    public int getM() {
        return m;
    }

    //////////////////////////////////////////////////////////////////////
    public int getS() {
        return s;
    }

    //////////////////////////////////////////////////////////////////////
    public int getMs() {
        return ms;
    }

    //////////////////////////////////////////////////////////////////////
    public void set(int h1, int m1, int s1, int ms1) {
        h = h1;
        m = m1;
        s = s1;
        ms = ms1;
    }

    //////////////////////////////////////////////////////////////////////
    public void set(Tiempo t) {
        set(t.h, t.m, t.s, t.ms);
    }

    //////////////////////////////////////////////////////////////////////
    public void add(int h1, int m1, int s1, int ms1) {
        int resto;
        ms += ms1;
        resto = ms / 1000000;
        ms = ms % 1000000;
        s += s1 + resto;
        resto = s / 60;
        s = s % 60;
        m += m1 + resto;
        resto = m / 60;
        m = m % 60;
        h += h1 + resto;
        h = h % 24;                     // el ultimo resto se pierde
    }

    //////////////////////////////////////////////////////////////////////
    public void add(Tiempo t) {
        add(t.h, t.m, t.s, t.ms);
    }

    //////////////////////////////////////////////////////////////////////
    public boolean equals(Tiempo t) {
        return (h == t.h && m == t.m && s == t.s && ms == t.ms);
    }

    //////////////////////////////////////////////////////////////////////
    public boolean less(Tiempo t) {
        if (h < t.h)
            return true;
        if (h == t.h) {
            if (m < t.m)
                return true;
            else if (m == t.m) {
                if (s < t.s)
                    return true;
                else if (s == t.s) {
                    if (ms < t.ms)
                        return true;
                }
            }
        }
        return false;
    }

    //////////////////////////////////////////////////////////////////////
    public int getInteger() {
        return ms + s * 1000 + m * (60 * 1000) + h * (60 * 60 * 1000);
    }

    //////////////////////////////////////////////////////////////////////
    public String toString() {
        return new String(str(h, 2) + ":" + str(m, 2) + ":" + str(s, 2) + ":" + str(ms, 6));
    }

    //////////////////////////////////////////////////////////////////////
    private String str(long v, int l) {
        String s = String.valueOf(v);
        return zeros.substring(0, l - s.length()) + s;
    }

}
