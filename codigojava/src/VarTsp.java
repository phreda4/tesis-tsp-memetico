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
import java.lang.*;

//////////////////////////////////////////////////////////////////////
public class VarTsp extends Frame implements ComponentListener, ActionListener, VarAni {

    private String name;    // nombre de la variable

    private int indexNow;   // indice actual

    private Vector time = new Vector();
    private Vector line = new Vector();
    private Vector timeLine = new Vector();

    private static final Font fuente = new Font("Helvetica", Font.BOLD, 8);
    //
//      Limites de los puntos
//
    private static float xmax;
    private static float xmin;
    private static float ymax;
    private static float ymin;
    //
//      Nombre de la instancia (.tsp)
//
    private static String fileName = "";
    //
//      Ciudades
//
    private static int cntCiudades;
    private static float[][] ciudades;

    //private GButton btnZoomAll=new GButton("zoomall.gif");
//private GButton btnZoomIn=new GButton("zoomin.gif");
//private GButton btnZoomOut=new GButton("zoomout.gif");
    private GButton btnSavePS = new GButton("saveps.gif");
    private GButton btnCityOnOff = new GButton("cityoo.gif");
    private GButton btnCityNros = new GButton("citynr.gif");

    private CanvasTsp canvasTsp = new CanvasTsp();

    //
//      Inicializar Ventana
//
//////////////////////////////////////////////////////////////////////
    public VarTsp(String n) {
        name = n;
        setTitle(name);
        setBackground(Color.lightGray);

        Panel p1 = new Panel(new FlowLayout(FlowLayout.LEFT));
//p1.add(btnZoomAll);
//p1.add(btnZoomIn);
//p1.add(btnZoomOut);
        p1.add(btnSavePS);
        p1.add(btnCityOnOff);
        p1.add(btnCityNros);

        setLayout(new BorderLayout(2, 2));
        add("North", p1);
        add("Center", canvasTsp);

        pack();
        addComponentListener(this);
        btnSavePS.addActionListener(this);
        btnCityOnOff.addActionListener(this);
        btnCityNros.addActionListener(this);
        indexNow = -1;
    }

    //
//      Cargar instancia y ajustar limites
//
//////////////////////////////////////////////////////////////////////
    public static void loadInstance() {
        int token;
        cntCiudades = 0;
        xmax = Float.MIN_VALUE;
        xmin = Float.MAX_VALUE;
        ymax = Float.MIN_VALUE;
        ymin = Float.MAX_VALUE;
        try {
            InputStream is = new FileInputStream(fileName);
            Reader r = new BufferedReader(new InputStreamReader(is));
            StreamTokenizer stok = new StreamTokenizer(r);
            //stok.eolIsSignificant(true);
            stok.commentChar('#');
            stok.wordChars('_', '_');

            boolean salir = false;
            do {
                do {
                    token = stok.nextToken();
                } while (stok.sval == null);
                if (stok.sval.equals("DIMENSION")) {
                    stok.nextToken();
                    stok.nextToken();
                    cntCiudades = (int) stok.nval;
                } else if (stok.sval.equals("EDGE_WEIGHT_TYPE")) {
                    stok.nextToken();
                    stok.nextToken();
                } else if (stok.sval.equals("COMMENT")) {
                    stok.nextToken();
                } else if (stok.sval.equals("NODE_COORD_SECTION")) {
                    salir = true;
                }
            } while (!salir);

            int i, l;
            float xf, yf;

            ciudades = new float[cntCiudades][2];

            for (i = 0; i < cntCiudades; i++) {
                stok.nextToken();
                l = (int) stok.nval;
                stok.nextToken();
                xf = (float) stok.nval;
                stok.nextToken();
                yf = (float) stok.nval;

                ciudades[i][0] = xf;
                ciudades[i][1] = yf;  // no se respeta numeracion l

                if (xf < xmin) xmin = xf;
                if (xf > xmax) xmax = xf;
                if (yf < ymin) ymin = yf;
                if (yf > ymax) ymax = yf;
            }
            is.close();
        } catch (IOException e) {
            System.err.println("Error en PointSet");
            System.err.println(e);
            System.exit(0);
        }
    }

    //////////////////////////////////////////////////////////////////////
    public String getName()  // Obtener el nombre de la variable
    {
        return name;
    }

    //////////////////////////////////////////////////////////////////////
    public void addEvn(Tiempo t, StreamTokenizer s) // Agregar evento
    {
        int token, a, b, c, d;
        try {
            token = s.nextToken();

            if (s.sval.equals("instance")) {
                token = s.nextToken();
                if (!s.sval.equals("xxx")) {
                    fileName = s.sval;
                    loadInstance();
                }
                canvasTsp.init(cntCiudades);
                canvasTsp.adjust(ciudades, xmin, xmax, ymin, ymax);
            } else if (s.sval.equals("clear")) {
                time.addElement(new Tiempo(t));
                line.addElement(new Integer(timeLine.size()));
                timeLine.addElement(new Byte((byte) 0));
            } else if (s.sval.equals("add")) {
                time.addElement(new Tiempo(t));
                line.addElement(new Integer(timeLine.size()));
                token = s.nextToken();
                a = (int) s.nval;
                token = s.nextToken();
                b = (int) s.nval;
                timeLine.addElement(new Byte((byte) 1));
                timeLine.addElement(new Integer(a));
                timeLine.addElement(new Integer(b));
            } else if (s.sval.equals("del")) {
                time.addElement(new Tiempo(t));
                line.addElement(new Integer(timeLine.size()));
                token = s.nextToken();
                a = (int) s.nval;
                token = s.nextToken();
                b = (int) s.nval;
                timeLine.addElement(new Byte((byte) 2));
                timeLine.addElement(new Integer(a));
                timeLine.addElement(new Integer(b));
            } else if (s.sval.equals("opt2")) {
                time.addElement(new Tiempo(t));
                line.addElement(new Integer(timeLine.size()));
                token = s.nextToken();
                a = (int) s.nval;
                token = s.nextToken();
                b = (int) s.nval;
                token = s.nextToken();
                c = (int) s.nval;
                token = s.nextToken();
                d = (int) s.nval;
                timeLine.addElement(new Byte((byte) 3));
                timeLine.addElement(new Integer(a));
                timeLine.addElement(new Integer(b));
                timeLine.addElement(new Integer(c));
                timeLine.addElement(new Integer(d));
            } else if (s.sval.equals("opt4")) {
                time.addElement(new Tiempo(t));
                line.addElement(new Integer(timeLine.size()));
                timeLine.addElement(new Byte((byte) 4));
                token = s.nextToken();
                a = (int) s.nval;
                token = s.nextToken();
                b = (int) s.nval;
                token = s.nextToken();
                c = (int) s.nval;
                token = s.nextToken();
                d = (int) s.nval;
                timeLine.addElement(new Integer(a));
                timeLine.addElement(new Integer(b));
                timeLine.addElement(new Integer(c));
                timeLine.addElement(new Integer(d));
                token = s.nextToken();
                a = (int) s.nval;
                token = s.nextToken();
                b = (int) s.nval;
                token = s.nextToken();
                c = (int) s.nval;
                token = s.nextToken();
                d = (int) s.nval;
                timeLine.addElement(new Integer(a));
                timeLine.addElement(new Integer(b));
                timeLine.addElement(new Integer(c));
                timeLine.addElement(new Integer(d));
            }

        } catch (IOException e) {
            System.err.println(e);
            System.exit(-1);
        }
    }

    //////////////////////////////////////////////////////////////////////
    public void endEvn()  // No hay mas eventos
    {
        time.addElement(new Tiempo(25, 0, 0, 0));
    }

    //////////////////////////////////////////////////////////////////////
    public void setTime(Tiempo t)  // ir al tiempo t
    {
        int j, i = indexNow;
        while (i >= 0 && t.less((Tiempo) time.elementAt(i))) {
            pasoi(i);
            i--;
        }
        while (!t.less((Tiempo) time.elementAt(i + 1))) {
            i++;
            paso(i);
        }
        if (i != indexNow) {
            canvasTsp.repaint();
            indexNow = i;
        }
    }

    //////////////////////////////////////////////////////////////////////
    public Tiempo getNextTime() {
        if (indexNow > -1 && indexNow >= time.size() - 2)
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
    private void paso(int i) {
        int a, b, c, d, e, f, g, h;
        int j = ((Integer) line.elementAt(i)).intValue();
        switch (((Byte) timeLine.elementAt(j)).intValue()) {
            case 0:
                canvasTsp.clear();
                break;
            case 1:
                a = ((Integer) timeLine.elementAt(j + 1)).intValue();
                b = ((Integer) timeLine.elementAt(j + 2)).intValue();
                canvasTsp.addEdge(a, b);
                break;
            case 2:
                a = ((Integer) timeLine.elementAt(j + 1)).intValue();
                b = ((Integer) timeLine.elementAt(j + 2)).intValue();
                canvasTsp.delEdge(a, b);
                break;
            case 3:
                a = ((Integer) timeLine.elementAt(j + 1)).intValue();
                b = ((Integer) timeLine.elementAt(j + 2)).intValue();
                c = ((Integer) timeLine.elementAt(j + 3)).intValue();
                d = ((Integer) timeLine.elementAt(j + 4)).intValue();
                canvasTsp.addEdge(a, c);
                canvasTsp.addEdge(b, d);
                canvasTsp.delEdge(a, b);
                canvasTsp.delEdge(c, d);
                break;
            case 4:
                a = ((Integer) timeLine.elementAt(j + 1)).intValue();
                b = ((Integer) timeLine.elementAt(j + 2)).intValue();
                c = ((Integer) timeLine.elementAt(j + 3)).intValue();
                d = ((Integer) timeLine.elementAt(j + 4)).intValue();
                e = ((Integer) timeLine.elementAt(j + 5)).intValue();
                f = ((Integer) timeLine.elementAt(j + 6)).intValue();
                g = ((Integer) timeLine.elementAt(j + 7)).intValue();
                h = ((Integer) timeLine.elementAt(j + 8)).intValue();
                canvasTsp.addEdge(a, f);
                canvasTsp.addEdge(b, e);
                canvasTsp.addEdge(c, h);
                canvasTsp.addEdge(d, g);
                canvasTsp.delEdge(a, b);
                canvasTsp.delEdge(c, d);
                canvasTsp.delEdge(e, f);
                canvasTsp.delEdge(g, h);
                break;
        }
    }

    //////////////////////////////////////////////////////////////////////
    private void pasoi(int i) {
        int a, b, c, d, e, f, g, h;
        int j = ((Integer) line.elementAt(i)).intValue();
        switch (((Byte) timeLine.elementAt(j)).intValue()) {
            case 0:
                canvasTsp.clear();  // habria que redibujar todo
                break;
            case 2:
                a = ((Integer) timeLine.elementAt(j + 1)).intValue();
                b = ((Integer) timeLine.elementAt(j + 2)).intValue();
                canvasTsp.addEdge(a, b);
                break;
            case 1:
                a = ((Integer) timeLine.elementAt(j + 1)).intValue();
                b = ((Integer) timeLine.elementAt(j + 2)).intValue();
                canvasTsp.delEdge(a, b);
                break;
            case 3:
                a = ((Integer) timeLine.elementAt(j + 1)).intValue();
                b = ((Integer) timeLine.elementAt(j + 2)).intValue();
                c = ((Integer) timeLine.elementAt(j + 3)).intValue();
                d = ((Integer) timeLine.elementAt(j + 4)).intValue();
                canvasTsp.delEdge(a, c);
                canvasTsp.delEdge(b, d);
                canvasTsp.addEdge(a, b);
                canvasTsp.addEdge(c, d);
                break;
            case 4:
                a = ((Integer) timeLine.elementAt(j + 1)).intValue();
                b = ((Integer) timeLine.elementAt(j + 2)).intValue();
                c = ((Integer) timeLine.elementAt(j + 3)).intValue();
                d = ((Integer) timeLine.elementAt(j + 4)).intValue();
                e = ((Integer) timeLine.elementAt(j + 5)).intValue();
                f = ((Integer) timeLine.elementAt(j + 6)).intValue();
                g = ((Integer) timeLine.elementAt(j + 7)).intValue();
                h = ((Integer) timeLine.elementAt(j + 8)).intValue();
                canvasTsp.delEdge(a, f);
                canvasTsp.delEdge(b, e);
                canvasTsp.delEdge(c, h);
                canvasTsp.delEdge(d, g);
                canvasTsp.addEdge(a, b);
                canvasTsp.addEdge(c, d);
                canvasTsp.addEdge(e, f);
                canvasTsp.addEdge(g, h);
                break;
        }
    }

    //
//  Interface para Action Listener
//
//////////////////////////////////////////////////////////////////////
    public void actionPerformed(ActionEvent e) {
        Object source = e.getSource();
        if (source == btnSavePS) {
            FileDialog d = new FileDialog(this, "Grabar Fig", FileDialog.SAVE);
            d.setFile("*.fig");
            d.show();
            String f = d.getFile();
            if (f != null)
                canvasTsp.saveFig(f);
        } else if (source == btnCityOnOff) {
            canvasTsp.changePnt();
            canvasTsp.repaint();
        } else if (source == btnCityNros) {
            canvasTsp.changeNros();
            canvasTsp.repaint();
        }

    }

    //
//  Interface para component listener
//
//////////////////////////////////////////////////////////////////////
    public void componentMoved(ComponentEvent e) {
    }

    public void componentHidden(ComponentEvent e) {
    }

    public void componentResized(ComponentEvent e) {
        canvasTsp.adjust(ciudades, xmin, xmax, ymin, ymax);
    }

    public void componentShown(ComponentEvent e) {
    }

}
