//////////////////////////////////////////////////////////////////////
//
// Clase para elegir la forma de la solucion
//
//////////////////////////////////////////////////////////////////////

import java.awt.*;
import java.awt.event.*;

/////////////////////////////////////////////////////////////////////////
class TspSol extends Dialog implements WindowListener, ActionListener {

    private TextField metodo = new TextField("", 20);
    private Button btnAce = new Button("Aceptar");
    private Button btnSal = new Button("Salir");
    private List lsCon = new List(6);
    private List lsOpt = new List(6);
    private Button btnAgrCo = new Button("+");
    private Button btnAgrOp = new Button("+");

    /////////////////////////////////////////////////////////////////////////
    public TspSol(Frame owner) {
        super(owner, "Elegir metodo para resolver", true);
        setBackground(Color.lightGray);

        Panel p1 = new Panel(new BorderLayout());
        p1.add(new Label("Construccion"), "North");
        p1.add(lsCon, "Center");
        p1.add(btnAgrCo, "North");

        Panel p2 = new Panel(new BorderLayout());
        p2.add(new Label("Optimizacion"), "North");
        p2.add(lsOpt, "Center");
        p2.add(btnAgrOp, "North");

        Panel p6 = new Panel(new FlowLayout());
        p6.add(p1);
        p6.add(p2);

        Panel p3 = new Panel(new BorderLayout());
        p3.add(new Label("Indique el metodo para resolver las instancias"), "North");
        p3.add(metodo, "South");

        Panel p4 = new Panel();
        p4.add(btnAce);
        p4.add(btnSal);

        setLayout(new BorderLayout());
        add(p3, "North");
        add(p6, "Center");
        add(p4, "South");

        pack();
        addWindowListener(this);
        metodo.addActionListener(this);
        btnAce.addActionListener(this);
        btnSal.addActionListener(this);
        lsCon.addActionListener(this);
        lsOpt.addActionListener(this);
        btnAgrCo.addActionListener(this);
        btnAgrOp.addActionListener(this);
        Dimension d = getSize();            // Centrar Frame
        Toolkit tk = getToolkit();
        Dimension dp = tk.getScreenSize();
        setLocation((dp.width / 2) - (d.width / 2), (dp.height / 2) - (d.height / 2));
        lsCon.add("SFC"); // addItem deprecated
        lsCon.add("NN");
        lsCon.add("MF");
        lsCon.add("MEME");
        lsOpt.add("2opt");
        lsOpt.add("lk");
        lsOpt.add("ilk");
    }

    /////////////////////////////////////////////////////////////////////////
    public String getSolve() {
        return metodo.getText();
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
        if (source == btnAce) {
            dispose();
        } else if (source == btnSal) {
            dispose();
        } else if (source == btnAgrCo) {
            if (!lsCon.getSelectedItem().equals(""))
                metodo.setText(lsCon.getSelectedItem());
        }
//else if (source==btnBorCo) {  }
        else if (source == btnAgrOp) {
            if (!lsOpt.getSelectedItem().equals("")) {
                String str;
                str = metodo.getText();
                if (str.equals(""))
                    str = str + "sfc";
                metodo.setText(str + "+" + lsOpt.getSelectedItem());
            }
        }
//else if (source==btnBorOp) {  }
    }
}
