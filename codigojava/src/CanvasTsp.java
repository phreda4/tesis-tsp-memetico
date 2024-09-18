//////////////////////////////////////////////////////////////////////
//
//      CanvasTsp.java
//      Vista de un tsp
//
//////////////////////////////////////////////////////////////////////

import java.awt.*;
import java.util.*;
import java.io.*;
import java.lang.*;

//////////////////////////////////////////////////////////////////////
class Edge {  // Clase para guardar los arcos
    public int a, b;

    public Edge(int i, int j) {
        a = i;
        b = j;
    }
}

//////////////////////////////////////////////////////////////////////
class CanvasTsp extends Canvas {

    private float xmin, ymin, xmax, ymax;      // limites de los puntos

    private int cntCitys;   // Cantidad de ciudades
    private float cc[][];
    private int[] x;                // Eje X de los puntos
    private int[] y;                // Eje Y de los puntos

    private Hashtable arcos;        // Arcos dibujados

    private boolean puntos;         // Dibuja puntos ?
    private boolean nros;         // Dibuja puntos ?

    //////////////////////////////////////////////////////////////////////
    CanvasTsp() {
        setBackground(Color.black);
    }

    //////////////////////////////////////////////////////////////////////
    public void paint(Graphics g) {
        Edge ed;
        Enumeration e = arcos.elements();
        g.setColor(Color.white);
        while (e.hasMoreElements())     // Dibujar Tour
        {
            ed = (Edge) e.nextElement();
            g.drawLine(x[ed.a], y[ed.a], x[ed.b], y[ed.b]);
        }
        if (puntos)                     // Si hay que dibujar...
        {
            g.setColor(Color.green);
            for (int a = 0; a < cntCitys; a++)  // Dibujar ciudades
                g.fillRect(x[a] - 1, y[a] - 1, 2, 2);
        }
        if (nros) {
            g.setColor(Color.blue);
            for (int a = 0; a < cntCitys; a++)  // Dibujar ciudades
                g.drawString("" + a, x[a] + 2, y[a] + 2);
        }
    }

    //////////////////////////////////////////////////////////////////////
    public Dimension getPreferredSize() {
        return new Dimension(200, 200);
    }

    //////////////////////////////////////////////////////////////////////
    public Dimension getMinimumSize() {
        return new Dimension(50, 50);
    }

    //////////////////////////////////////////////////////////////////////
    public void changePnt() {
        puntos = !puntos;
    }

    //////////////////////////////////////////////////////////////////////
    public void changeNros() {
        nros = !nros;
    }

    //////////////////////////////////////////////////////////////////////
    public void init(int c) {
        cntCitys = c;
        x = new int[cntCitys];
        y = new int[cntCitys];
        arcos = new Hashtable();
        puntos = true;
        nros = false;
    }

    //////////////////////////////////////////////////////////////////////
    public boolean notInit() {
        return (cntCitys == 0);
    }

    //
//      Ajustar coordenadas
//
//////////////////////////////////////////////////////////////////////
    public void adjust(float[][] citys, float x1, float x2, float y1, float y2) {
        cc = citys;
        Dimension d = getSize();
        xmin = x1;
        xmax = x2;
        ymin = y1;
        ymax = y2;
        float px = (d.width - 8) / (xmax - xmin);
        float py = (d.height - 8) / (ymax - ymin);
        for (int i = 0; i < cntCitys; i++) {
            x[i] = 4 + (int) ((citys[i][0] - xmin) * px);
            y[i] = d.height - (4 + (int) ((citys[i][1] - ymin) * py));
        }
    }

    //////////////////////////////////////////////////////////////////////
    public void adjustFig(int maxx, int maxy) {
        float px = (maxx - 8) / (xmax - xmin);
        float py = (maxy - 8) / (ymax - ymin);
        for (int i = 0; i < cntCitys; i++) {
            x[i] = 4 + (int) ((cc[i][0] - xmin) * px);
            y[i] = maxy - (4 + (int) ((cc[i][1] - ymin) * py));
        }
    }

    //////////////////////////////////////////////////////////////////////
    public final void clear() {
        arcos.clear();
    }

    //////////////////////////////////////////////////////////////////////
    public final void addEdge(int a, int b) {
        if (b < a) {
            int c = a;
            a = b;
            b = c;
        }
        arcos.put("" + a + ":" + b, new Edge(a, b));
    }

    //////////////////////////////////////////////////////////////////////
    public final void delEdge(int a, int b) {
        if (b < a) {
            int c = a;
            a = b;
            b = c;
        }
        arcos.remove("" + a + ":" + b);
    }

    private String header[] = {
            "#FIG 3.2",
            "Landscape",
            "Center",
            "Inches",
            "Letter",
            "100.00",
            "Single",
            "-2",
            "1200 2",
            ""};    // fin de header con ""

    //////////////////////////////////////////////////////////////////////
    public void saveFig(String name) {
        int i;
        adjustFig(10000, 10000);
        try {

            FileWriter out = new FileWriter(name);

            i = 0;                          // escribir cabecera
            do {
                out.write(header[i] + "\n");
                i++;
            } while (!header[i].equals(""));


            if (puntos)                     // Si hay que dibujar puntos...
                for (int a = 0; a < cntCitys; a++)  // Dibujar ciudades
                {
                    out.write("1 3 0 1 0 0 0 0 20 0.000 1 0.0000 " +
                            x[a] + " " + y[a] + " 30 30 " + x[a] + " " + y[a] + " " + (x[a] + 30) + " " + y[a] + "\n");
                }
            if (nros)
                for (int a = 0; a < cntCitys; a++)  // Dibujar numeros
                    out.write("4 0 0 0 0 0 12 0.0000 4 105 390 " + x[a] + " " + y[a] + " " + a + "\\001\n");

            Edge ed;
            Enumeration e = arcos.elements();
            while (e.hasMoreElements())     // Dibujar Tour
            {
                ed = (Edge) e.nextElement();
                out.write("2 1 0 1 0 7 0 0 -1 0.000 0 0 -1 0 0 2\n");
                out.write("\t" + x[ed.a] + " " + y[ed.a] + " " + x[ed.b] + " " + y[ed.b] + "\n");
            }

            out.flush();
            out.close();
        } catch (IOException e) {
            System.err.println(e);
        }
        adjust(cc, xmin, xmax, ymin, ymax);
    }

}
