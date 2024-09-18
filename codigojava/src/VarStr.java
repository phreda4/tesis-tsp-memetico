//////////////////////////////////////////////////////////////////////
//
//      Anima.java
//      Clase principal encargada de coordinar las acciones 
//
//////////////////////////////////////////////////////////////////////

import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.util.*;
import java.sql.Time;
import java.lang.*;

//////////////////////////////////////////////////////////////////////
public class VarStr extends Frame implements VarAni {

    private String name;
    private int indexNow;
    private java.awt.List lstStr = new java.awt.List(10);
    private Vector time;

    //////////////////////////////////////////////////////////////////////
    public VarStr(String n) {
        name = n;
        setTitle(name);
        setBackground(Color.lightGray);

        setLayout(new BorderLayout(2, 2));
        add("Center", lstStr);

        pack();
        setSize(300, 100);
        indexNow = 0;
        time = new Vector();
    }

    //////////////////////////////////////////////////////////////////////
    public String getName() {
        return name;
    }

    //////////////////////////////////////////////////////////////////////
    public void addEvn(Tiempo t, StreamTokenizer s) {
        try {
            int token = s.nextToken();
        } catch (IOException e) {
            System.err.println(e);
            System.exit(-1);
        }
        lstStr.add(t.toString() + "-" + s.sval);
        time.addElement(new Tiempo(t));
    }

    //////////////////////////////////////////////////////////////////////
    public void endEvn() {
        time.addElement(new Tiempo(25, 0, 0, 0));
    }

    //////////////////////////////////////////////////////////////////////
    public Tiempo getNextTime() {
        if (indexNow == time.size())
            return (Tiempo) time.elementAt(indexNow);
        return (Tiempo) time.elementAt(indexNow + 1);
    }

    //////////////////////////////////////////////////////////////////////
    public Tiempo getPrevTime() {
        if (indexNow == 0)
            return (Tiempo) time.elementAt(indexNow);
        return (Tiempo) time.elementAt(indexNow - 1);
    }

    //////////////////////////////////////////////////////////////////////
    public void setTime(Tiempo t) {
        int i = indexNow;
        while (i >= 0 && t.less((Tiempo) time.elementAt(i)))
            i--;
        while (!t.less((Tiempo) time.elementAt(i + 1)))
            i++;
        if (i != indexNow) {
            indexNow = i;
            lstStr.select(indexNow);
        }
    }


}
