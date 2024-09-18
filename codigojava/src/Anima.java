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
public class Anima extends Frame implements WindowListener, ActionListener, Runnable {
    //
// Fuentes de letra
//
    private static Font fontTime = new Font("Courier", Font.BOLD, 14);
    private static Font fontCB = new Font("Helvetica", Font.BOLD, 12);

    private static int paso[] = {0, 1, 5, 10, 20, 40, 80, 100, 200, 400, 500};
    private int step;
    private static Tiempo timeZero = new Tiempo(0, 0, 0, 0);
    private Tiempo timeNow, timeStep, timeMax;
    private boolean corriendo;        // estado de corrida (play o stop)
    private volatile Thread control;    // Thread

    private GButton btnPlay = new GButton("play.gif");
    private GButton btnPlayR = new GButton("playr.gif");
    private GButton btnFirst = new GButton("first.gif");
    private GButton btnStop = new GButton("stop.gif");
    private GButton btnLast = new GButton("last.gif");

    private Label txtTime = new Label("00:00:00:000000", Label.CENTER);
    private Perilla perPer = new Perilla(30, 0, 10);
    private Scrollbar scrTime = new Scrollbar(Scrollbar.HORIZONTAL);

    private Vector variables = new Vector();
    private Tsp tsp;        // Para retornar al programa principal

    //
//      Inicializar Ventana
//
//////////////////////////////////////////////////////////////////////
    public Anima() {
        setTitle("Animador");
        setResizable(false);
        setBackground(Color.lightGray);

        Panel p1 = new Panel();
        p1.add(new Label("Time:"));
        p1.add(txtTime);
        txtTime.setFont(fontTime);

        Panel p2 = new Panel();
        p2.add(btnFirst);
        p2.add(btnPlayR);
        p2.add(btnStop);
        p2.add(btnPlay);
        p2.add(btnLast);
        p2.add(perPer);
        perPer.setValue(1);

        Panel panelCon = new Panel();
        panelCon.setLayout(new BorderLayout(0, 0));
        panelCon.add("North", p2);
        panelCon.add("Center", p1);
        panelCon.add("South", scrTime);

        setLayout(new BorderLayout(2, 2));
        add("Center", panelCon);

        pack();
        addWindowListener(this);
        btnFirst.addActionListener(this);
        btnLast.addActionListener(this);
        btnStop.addActionListener(this);
        btnPlay.addActionListener(this);
        btnPlayR.addActionListener(this);
        perPer.addActionListener(this);

        timeNow = new Tiempo();
        timeStep = new Tiempo(0, 0, 0, 10);
        timeMax = new Tiempo();
        step = 10;
// Centrar Ventana
        Dimension d = getSize();
        Toolkit tk = getToolkit();
        Dimension dp = tk.getScreenSize();
        setLocation((dp.width / 2) - (d.width / 2), dp.height - 2 * d.height - 1);
        corriendo = false;
        control = new Thread(this);
        control.start();
    }

    //
//  Interface para Window Listener
//
//////////////////////////////////////////////////////////////////////
    public void windowClosed(WindowEvent event) {
    }

    public void windowDeiconified(WindowEvent event) {
    }

    public void windowIconified(WindowEvent event) {
    }

    public void windowActivated(WindowEvent event) {
    }

    public void windowDeactivated(WindowEvent event) {
    }

    public void windowOpened(WindowEvent event) {
    }

    public synchronized void windowClosing(WindowEvent event) {
        control = null;
        notify();
        for (int i = 0; i < variables.size(); i++)
            ((Frame) variables.elementAt(i)).dispose();
        dispose();
        tsp.setVisible(true);
    }

    //
//  Interface para Action Listener
//
//////////////////////////////////////////////////////////////////////
    public synchronized void actionPerformed(ActionEvent e) {
        Object source = e.getSource();
        if (source == btnFirst) {
            corriendo = false;
            setTime(timeZero);
        } else if (source == btnLast) {
            corriendo = false;
            setTime(timeMax);
        } else if (source == btnStop)
            corriendo = false;
        else if (source == btnPlay) {
            if (step == 0)
                setNext();
            else {
                timeStep.set(0, 0, 0, step);
                corriendo = true;
                notify();
            }
        } else if (source == btnPlayR) {
            if (step == 0)
                setPrev();
            else {
                timeStep.set(0, 0, 0, -step);
                corriendo = true;
                notify();
            }
        } else if (source == perPer) {
            step = paso[perPer.getValue()];
            if (step == 0)
                corriendo = false;
        }
    }

    //////////////////////////////////////////////////////////////////////
    public void init(String name, Tsp t) {
        tsp = t;
        VarAni vani;
        int token, i;
        try {
            InputStream is = new FileInputStream(name);
            Reader r = new BufferedReader(new InputStreamReader(is));
            StreamTokenizer stok = new StreamTokenizer(r);
            while ((token = stok.nextToken()) != stok.TT_EOF) {
                timeMax.add(0, 0, 0, (int) stok.nval);
                token = stok.nextToken();
                if (stok.sval.equals("String"))           // Definicion de una variable string
                {
                    token = stok.nextToken();
                    variables.addElement(new VarStr(stok.sval));
                } else if (stok.sval.equals("Number"))      // Definicion de una variable numerica
                {
                    token = stok.nextToken();
                    variables.addElement(new VarNum(stok.sval));
                } else if (stok.sval.equals("Tsp"))         // Definicion de una variable tour
                {
                    token = stok.nextToken();
                    variables.addElement(new VarTsp(stok.sval));
                } else                                      // Orden a una variable ya definida
                {
                    for (i = 0; i < variables.size(); i++)        // Buscar variable que corresponda
                    {
                        vani = (VarAni) variables.elementAt(i);
                        if (stok.sval.equals(vani.getName())) {
                            vani.addEvn(timeMax, stok);
                            break;
                        }
                    }
                }
            }
            is.close();
        } catch (IOException e) {
            System.err.println(e);
            System.exit(0);
        }
        variables.trimToSize();
        Object o;
        Dimension d;
        int x = 0, y = 0, maxHeight = 0;
        Toolkit tk = getToolkit();
        Dimension dp = tk.getScreenSize();
        for (i = 0; i < variables.size(); i++) {
            o = variables.elementAt(i);
//  if (((VarAni)o).vacio())	// borrar las variables vacias
//    {
//    variables.delete(i);i--;
//    }
//  else
//    {
            ((VarAni) o).endEvn();
            ((Frame) o).show(); // Crear una ventana para cada variable
            d = ((Frame) o).getSize();
            if (d.height > maxHeight)
                maxHeight = d.height;
            ((Frame) o).setLocation(x, y);
            x += d.width;
            if (x > dp.width) {
                x = 0;
                y += maxHeight;
                maxHeight = 0;
            }
        }
//  }
        scrTime.setMinimum(0);
        scrTime.setMaximum(timeMax.getInteger());
    }

    //////////////////////////////////////////////////////////////////////
    private void setNext() {
        Tiempo t = new Tiempo(25, 0, 0, 0);
        Tiempo aux = new Tiempo();
        for (int i = 0; i < variables.size(); i++) {
            aux = ((VarAni) variables.elementAt(i)).getNextTime();
            if (aux.less(t))
                t.set(aux);
        }
        setTime(t);
    }

    //////////////////////////////////////////////////////////////////////
    private void setPrev() {
        Tiempo t = new Tiempo();
        Tiempo aux = new Tiempo();
        for (int i = 0; i < variables.size(); i++) {
            aux = ((VarAni) variables.elementAt(i)).getPrevTime();
            if (t.less(aux))
                t.set(aux);
        }
        setTime(t);
    }

    //////////////////////////////////////////////////////////////////////
    private void setTime(Tiempo t) {
        Object o;
        timeNow.set(t);
        txtTime.setText(timeNow.toString());
        scrTime.setValue(timeNow.getInteger());
        for (int i = 0; i < variables.size(); i++) {
            o = variables.elementAt(i);
            ((VarAni) o).setTime(timeNow);
        }
    }

    //////////////////////////////////////////////////////////////////////
    public void run() {
        int i;
        Thread thisThread = Thread.currentThread();
        while (control == thisThread)
            try {
                if (!corriendo) {
                    synchronized (this) {
                        while (!corriendo && control == thisThread) wait();
                    }
                }
                txtTime.setText(timeNow.toString());
                scrTime.setValue(timeNow.getInteger());
                for (i = 0; i < variables.size(); i++)
                    ((VarAni) variables.elementAt(i)).setTime(timeNow);
                timeNow.add(timeStep);
                if (timeNow.less(timeZero)) {
                    setTime(timeZero);
                    corriendo = false;
                }
                if (timeMax.less(timeNow)) {
                    setTime(timeMax);
                    corriendo = false;
                }
            } catch (InterruptedException e) {
            }
    }
}
