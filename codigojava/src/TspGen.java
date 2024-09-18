//////////////////////////////////////////////////////////////////////
//
//  Generar instancias al azar
//
//////////////////////////////////////////////////////////////////////

import java.awt.*;
import java.awt.event.*;
import java.lang.*;
import java.io.*;

/////////////////////////////////////////////////////////////////////////
class TspGen extends Dialog implements WindowListener, ActionListener {

    private Button btnGen = new Button("Generar");
    private Button btnSal = new Button("Salir");
    private TextField tfCiu = new TextField("", 5);
    private TextField tfIns = new TextField("", 5);
    private Label lblEstado = new Label("");

    /////////////////////////////////////////////////////////////////////////
    public TspGen(Frame owner) {
        super(owner, "Generador de instancias", true);
        setBackground(Color.lightGray);

        Panel p1 = new Panel();
        p1.add(new Label("Cantidad de ciudades:"));
        p1.add(tfCiu);

        Panel p2 = new Panel();
        p2.add(new Label("Cantidad de instancias:"));
        p2.add(tfIns);

        Panel p4 = new Panel(new GridLayout(2, 1));
        p4.add(p1);
        p4.add(p2);

        Panel p3 = new Panel();
        p3.add(btnGen);
        p3.add(btnSal);

        setLayout(new BorderLayout());
        add(p4, "Center");
        add(p3, "South");

        pack();
        addWindowListener(this);
        btnGen.addActionListener(this);
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
        dispose();
    }

    //
//  Interface para Action Listener
//
//////////////////////////////////////////////////////////////////////
    public void actionPerformed(ActionEvent e) {
        Object source = e.getSource();
        Integer i = new Integer(0);
        if (source == btnGen) {
            try {
                i.parseInt(tfCiu.getText());
                i.parseInt(tfCiu.getText());
            } catch (NumberFormatException ex) {
                System.out.println("Error ingreso");
                return;
            }
            System.out.println("Crear instancias..");
            try {
                String[] progarray = new String[3];
                Runtime rt = Runtime.getRuntime();
                progarray[0] = "geni";
                progarray[1] = "-i" + tfIns.getText();
                progarray[2] = "-c" + tfCiu.getText();
                Process pr = rt.exec(progarray);
                int status;
                status = pr.waitFor();
            } catch (IOException ex) {
                System.err.println("exec error:" + ex);
            } catch (InterruptedException ex) {
                System.err.println("error:" + ex);
            }
            System.out.println("Fin Instancias...");
            dispose();
        } else if (source == btnSal) {
            dispose();
        }
    }

}
