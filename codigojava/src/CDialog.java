//////////////////////////////////////////////////////////////////////
//
// Dialogo Comun
//
//////////////////////////////////////////////////////////////////////

import java.awt.*;
import java.awt.event.*;

/////////////////////////////////////////////////////////////////////////
class CDialog extends Dialog implements WindowListener, ActionListener {

    private Button btnOk = new Button(" Ok ");

    public CDialog(Frame parent, String msg, boolean b) {
        super(parent, "Mensaje", b);
        Panel p = new Panel();
        p.add(btnOk);
        add(new Label(msg), "Center");
        add(p, "South");
        pack();
        addWindowListener(this);
        btnOk.addActionListener(this);

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
        if (source == btnOk) {
            dispose();
        }
    }

}
