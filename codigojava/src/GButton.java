//////////////////////////////////////////////////////////////////////
//
//      Boton grafico
//
//////////////////////////////////////////////////////////////////////

import java.awt.*;
import java.awt.image.*;
import java.awt.event.*;

//////////////////////////////////////////////////////////////////////
public class GButton extends Component implements MouseListener {

    Image Imagen = null;
    boolean pressed = false;
    boolean in = false;
    int width = 0, height = 0;
    String label;
    ActionListener actionListener;

    //////////////////////////////////////////////////////////////////////
    public GButton(Image im) {
        MediaTracker mt = new MediaTracker(this);
        mt.addImage(im, 1);
        try {
            mt.waitForAll();
        } catch (InterruptedException e) {
            System.out.println("Error cargando imagen para boton " + this);
        }
        width = im.getWidth(this);
        height = im.getHeight(this);
        Imagen = im;
        pressed = false;
        in = false;
        addMouseListener(this);
    }

    //////////////////////////////////////////////////////////////////////
    public Dimension getPreferredSize() {
        return new Dimension(width + 2, height + 2);
    }

    //////////////////////////////////////////////////////////////////////
    public Dimension getMinimumSize() {
        return new Dimension(10, 10);
    }

    //////////////////////////////////////////////////////////////////////
    public GButton(String name) {
        this(Toolkit.getDefaultToolkit().getImage(name));
        label = name;
    }

    //////////////////////////////////////////////////////////////////////
    public void paint(Graphics g) {
        if (pressed)
            g.drawImage(Imagen, 1, 1, this);
        else
            g.drawImage(Imagen, 0, 0, this);
        if (in) {
            g.setColor(!pressed ? Color.white : Color.gray);
            g.drawLine(0, height, 0, 0);
            g.drawLine(0, 0, width, 0);
            g.setColor(pressed ? Color.white : Color.gray);
            g.drawLine(0, height, width, height);
            g.drawLine(width, height, width, 1);
        }
    }

    //////////////////////////////////////////////////////////////////////
    public boolean imageUpdate(Image img, int flags, int x, int y, int width, int height) {
        if ((flags & ERROR) == 0) {
            if ((flags & ALLBITS) != 0)
                repaint();
        }
        return ((flags & ALLBITS) == 0);
    }

    //////////////////////////////////////////////////////////////////////
    public void addActionListener(ActionListener listener) {
        actionListener = AWTEventMulticaster.add(actionListener, listener);
        enableEvents(AWTEvent.MOUSE_EVENT_MASK);
    }

    //////////////////////////////////////////////////////////////////////
    public void removeActionListener(ActionListener listener) {
        actionListener = AWTEventMulticaster.remove(actionListener, listener);
    }

    //////////////////////////////////////////////////////////////////////
    public void mousePressed(MouseEvent e) {
        pressed = true;
        repaint();
    }

    //////////////////////////////////////////////////////////////////////
    public void mouseReleased(MouseEvent e) {
        if (pressed == true) {
            pressed = false;
            repaint();
        }
    }

    //////////////////////////////////////////////////////////////////////
    public void mouseEntered(MouseEvent e) {
        in = true;
        repaint();
    }

    //////////////////////////////////////////////////////////////////////
    public void mouseExited(MouseEvent e) {
        in = false;
        pressed = false;
        repaint();
    }

    //////////////////////////////////////////////////////////////////////
    public void mouseClicked(MouseEvent e) {
        if (actionListener != null)
            actionListener.actionPerformed(new ActionEvent(this, ActionEvent.ACTION_PERFORMED, label));
    }

}
