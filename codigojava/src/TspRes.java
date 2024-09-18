//////////////////////////////////////////////////////////////////////
//
// Resolver instancias
//
//////////////////////////////////////////////////////////////////////

import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.util.Arrays;
import java.util.Collections;

/////////////////////////////////////////////////////////////////////////
class TspRes extends Frame implements WindowListener, ActionListener {

    private Button btnGen = new Button("Comenzar");
    private Button btnSal = new Button(" Salir ");
    private List lsIns = new List(6);
    private List lsMed = new List(6);
    private Button btnAgrIn = new Button("+");
    private Button btnBorIn = new Button("-");
    private Button btnAgrMe = new Button("+");
    private Button btnBorMe = new Button("-");
    private Checkbox cbAni = new Checkbox("Animacion", null, true);
    private Checkbox cbSol = new Checkbox("Solucion", null, false);
    private Checkbox cbEst = new Checkbox("Estadistica", null, true);

    /////////////////////////////////////////////////////////////////////////
    public TspRes() {
        setTitle("Resolver instancias");
        setBackground(Color.lightGray);

        Panel p11 = new Panel(new GridLayout(2, 1));
        p11.add(btnAgrIn);
        p11.add(btnBorIn);

        Panel p1 = new Panel(new BorderLayout());
        p1.add(new Label("Instancia/s:"), "North");
        p1.add(lsIns, "Center");
        p1.add(p11, "East");

        Panel p21 = new Panel(new GridLayout(2, 1));
        p21.add(btnAgrMe);
        p21.add(btnBorMe);

        Panel p2 = new Panel(new BorderLayout());
        p2.add(new Label("metodo/s:"), "North");
        p2.add(lsMed, "Center");
        p2.add(p21, "East");

        Panel p6 = new Panel(new FlowLayout(FlowLayout.CENTER));
        p6.add(p1);
        p6.add(p2);

        Panel p3 = new Panel();
        p3.add(cbAni);
        p3.add(cbSol);
        p3.add(cbEst);

        Panel p4 = new Panel();
        p4.add(btnGen);
        p4.add(btnSal);

        setLayout(new BorderLayout());
        add(p3, "North");
        add(p6, "Center");
        add(p4, "South");

        pack();
        addWindowListener(this);
        btnGen.addActionListener(this);
        btnSal.addActionListener(this);
        btnAgrIn.addActionListener(this);
        btnBorIn.addActionListener(this);
        btnAgrMe.addActionListener(this);
        btnBorMe.addActionListener(this);
        Dimension d = getSize();            // Centrar Frame
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

    //////////////////////////////////////////////////////////////////////
    private void addInstance() {
        FileDialog d = new FileDialog(this, "Agregar Instancia a resolver", FileDialog.LOAD);
        d.setFile("*.tsp");
        d.show();
        String f = d.getFile();
        if (f != null)
            lsIns.add(f);
    }

    //////////////////////////////////////////////////////////////////////
    private void delInstance() {
        int s = lsIns.getSelectedIndex();
        if (s == -1) return;
        lsIns.remove(s);
    }

    //////////////////////////////////////////////////////////////////////
    private void addSolve() {
        TspSol ts = new TspSol(this);
        ts.show();
        if (!ts.getSolve().equals(""))
            lsMed.add(ts.getSolve());
    }

    /////////////////////////////////////////////////////////////////////
    private void delSolve() {
        int s = lsMed.getSelectedIndex();
        if (s == -1) return;
        lsMed.remove(s);
    }

    //////////////////////////////////////////////////////////////////////
    private void solve() {
        if (lsIns.getItemCount() == 0) {
            CDialog d = new CDialog(this, "Falta indicar instancia/s", true);
            d.show();
            return;
        }
        if (lsMed.getItemCount() == 0) {
            CDialog d = new CDialog(this, "Falta indicar forma/s de resolver", true);
            d.show();
            return;
        }
        Runtime rt = Runtime.getRuntime();
        String[] cmd = new String[6];
        Process pr;
        int i, j, c = 3;
//        cmd[0] = "tsp";
        if (cbAni.getState())
            cmd[c++] = "-a";
        if (cbSol.getState())
            cmd[c++] = "-r";
        if (cbEst.getState())
            cmd[c++] = "-s";
        try {
            for (i = 0; i < lsIns.getItemCount(); i++)
                for (j = 0; j < lsMed.getItemCount(); j++) {
                    cmd[1] = lsIns.getItem(i);        // Instancia
                    cmd[2] = "-m" + lsMed.getItem(j);    // Metodo
                    System.out.println("Resolviendo...");

                    //customisations to specify working directory
                    File pathToExecutable = new File("../../nnCodigoc/tsp");
                    cmd[0] = pathToExecutable.getAbsolutePath();
                    String[] tmp = Arrays.stream(cmd).filter(each -> each != null).toArray(String[]::new);
                    ProcessBuilder builder = new ProcessBuilder(tmp);
                    builder.redirectErrorStream(true);
                    pr = builder.start();
                    // Ejecutar tsp
                    pr.waitFor();            // Esperar a que termine
                }
        } catch (IOException e) {
            System.err.println("exec error:" + e);
        } catch (InterruptedException ex) {
            System.err.println("error:" + ex);
        }
    }

    //
//  Interface para Action Listener
//
//////////////////////////////////////////////////////////////////////
    public void actionPerformed(ActionEvent e) {
        Object source = e.getSource();
        if (source == btnGen) {
            solve();
        } else if (source == btnAgrIn) {
            addInstance();
        } else if (source == btnBorIn) {
            delInstance();
        } else if (source == btnAgrMe) {
            addSolve();
        } else if (source == btnBorMe) {
            delSolve();
        } else if (source == btnSal) {
            dispose();
        }
    }

}
