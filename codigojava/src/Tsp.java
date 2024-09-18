//////////////////////////////////////////////////////////////////////
//
//  Tsp.java
//  Interface grafica del tsp
//
//////////////////////////////////////////////////////////////////////

import java.awt.*;
import java.awt.event.*;

//////////////////////////////////////////////////////////////////////
public class Tsp extends Frame implements WindowListener, ActionListener {

    private Button btnRes = new Button("Resolver Instancia");
    private Button btnGen = new Button("Generar instancias al azar");
    private Button btnVer = new Button("Ver una animacion");
    private Button btnSol = new Button("Ver una solucion");
    private Button btnSal = new Button("Salir de TSP");
    private TspRes tspRes = new TspRes();
    private TspGen tspGen = new TspGen(this);

    //////////////////////////////////////////////////////////////////////
    public Tsp() {
        setTitle("Tsp");
        setBackground(Color.lightGray);

        Panel p1 = new Panel(new GridLayout(5, 1));
        p1.add(btnRes);
        p1.add(btnGen);
        p1.add(btnVer);
        p1.add(btnSol);
        p1.add(btnSal);

        setLayout(new BorderLayout());
        add(new Label(""), "East");
        add(new Label(""), "West");
        add(new Label(""), "North");
        add(p1, "Center");
        add(new Label("Interfaz grafica de TSP."), "South");

        pack();
        addWindowListener(this);
        btnRes.addActionListener(this);
        btnGen.addActionListener(this);
        btnVer.addActionListener(this);
        btnSol.addActionListener(this);
        btnSal.addActionListener(this);
        Dimension d = getSize();
        Toolkit tk = getToolkit();
        Dimension dp = tk.getScreenSize();
        setLocation((dp.width / 2) - (d.width / 2), (dp.height / 2) - (d.height / 2));
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

    public void windowClosing(WindowEvent event) {
        System.exit(0);
    }


    //
//  Interface para Action Listener
//
//////////////////////////////////////////////////////////////////////
    public void actionPerformed(ActionEvent e) {
        Object source = e.getSource();
        if (source == btnRes) {
            tspRes.show();
        } else if (source == btnGen) {
            tspGen.show();
        } else if (source == btnVer) {
            FileDialog d = new FileDialog(this, "Cargar animaci�n", FileDialog.LOAD);
//  d.setFilter("*.anm");
            d.show();
            String f = d.getFile();
            if (f != null) {
                setVisible(false);
                Anima anima = new Anima();
                anima.init(f, this);
                anima.show();
            }
        } else if (source == btnSol) {
            FileDialog d = new FileDialog(this, "Cargar soluci�n", FileDialog.LOAD);
//  d.setFilter("*.sol");
            d.show();
//  String instance=d.getFile();
            String solution = d.getFile();
//  if (f!=null)
//    {
            setVisible(false);
            // reemplayar .sol por .tsp
//    f.Pos(".tsp"),4,".sol"
            VarTsp varTsp = new VarTsp(solution);
            varTsp.loadInstance();
//    varTsp.loadSolution(solution);
            varTsp.show();
//    }
        } else if (source == btnSal) {
            System.exit(0);
        }
    }

    //////////////////////////////////////////////////////////////////////
    public static void main(String[] args) {
        Tsp tsp = new Tsp();
        tsp.show();
    }

}
