//////////////////////////////////////////////////////////////////////
//
//      VarNum.java
//      Animacion de variable numerica
//
//////////////////////////////////////////////////////////////////////

import java.awt.*;
import java.io.*;
import java.util.*;
import java.lang.*;

//////////////////////////////////////////////////////////////////////
public class VarNum extends Frame implements VarAni {

    private String name;            // Nombre de la variable
    private int indexNow;

    private Vector nros = new Vector();       // Numeros
    private Vector time = new Vector();       // tiempo del numero

    private float media, desvio, minimo, maximo;  // Informacion estadistica

    private GButton btnZoomAll = new GButton("zoomall.gif");
    private GButton btnZoomIn = new GButton("zoomin.gif");
    private GButton btnSavePS = new GButton("saveps.gif");
    private Label lblMedia = new Label("NC");
    private Label lblDesvio = new Label("NC");

    //
//      Inicializar Ventana
//
//////////////////////////////////////////////////////////////////////
    public VarNum(String n) {
        name = n;
        media = 0;
        maximo = Float.MIN_VALUE;
        minimo = Float.MAX_VALUE;
        setTitle(name);
        setBackground(Color.lightGray);

        Panel p1 = new Panel(new FlowLayout(FlowLayout.LEFT, 2, 2));
        p1.add(btnZoomAll);
        p1.add(btnZoomIn);
        p1.add(btnSavePS);

        Panel p2 = new Panel();
        p2.add(new Canvas());

        Panel p3 = new Panel(new FlowLayout(0, 0, FlowLayout.LEFT));
        p3.add(new Label("Media:"));
        p3.add(lblMedia);
        p3.add(new Label(" Desvio:"));
        p3.add(lblDesvio);

        setLayout(new BorderLayout(2, 2));
        add("North", p1);
        add("Center", p2);
        add("South", p3);

        pack();
    }

    //
//  Metodos para la interface VarAni
//
//////////////////////////////////////////////////////////////////////
    public String getName() {
        return name;
    }

    //////////////////////////////////////////////////////////////////////
    public void addEvn(Tiempo t, StreamTokenizer s) {
        Float n = new Float(s.nval);
        float fv = n.floatValue();
        if (fv < minimo) minimo = fv;
        if (fv > maximo) maximo = fv;
        nros.addElement(n);
        time.addElement(new Tiempo(t));
        media += fv;
    }

    //////////////////////////////////////////////////////////////////////
    public void endEvn() {
        nros.trimToSize();
        if (nros.size() > 0)
            media = media / nros.size();
        int i;
        float fv;
        desvio = 0;
        for (i = 0; i < nros.size(); i++) {
            fv = ((Float) nros.elementAt(i)).floatValue();
            desvio += Math.pow((double) fv - media, (double) 2);
        }
        desvio = (float) Math.sqrt((double) desvio) / nros.size();
        lblMedia.setText("" + media);
        lblDesvio.setText("" + desvio);
    }

    //////////////////////////////////////////////////////////////////////
    public void setTime(Tiempo t) {

    }

    //////////////////////////////////////////////////////////////////////
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
    public static void main(String[] args) {
        VarNum varNum = new VarNum("Numero");
        varNum.show();
/*
varNum.addEvn(0,"3");
varNum.addEvn(2,"5");
varNum.addEvn(3,"66");
varNum.addEvn(4,"77");
varNum.addEvn(5,"88");
varNum.addEvn(6,"99");
varNum.endEvn();
*/

    }

}
