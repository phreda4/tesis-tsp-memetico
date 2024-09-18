//////////////////////////////////////////////////////////////////////
//
//      Perilla
//
//////////////////////////////////////////////////////////////////////

import java.awt.*;
import java.awt.image.*;
import java.awt.event.*;

//////////////////////////////////////////////////////////////////////
public class Perilla extends Component implements MouseMotionListener {

    private static Color ON = new Color(255, 0, 0);
    private static Color OFF = new Color(200, 0, 0);

    private boolean drag;
    private int state;
    private int minS = 0, maxS = 1;
    private int diameter = 0;
    private int cos[], sin[];
    private ActionListener actionListener;

    //////////////////////////////////////////////////////////////////////
    public Perilla(int d, int mi, int ma) {
        diameter = d;
        minS = mi;
        maxS = ma;
        state = minS;
        cos = new int[maxS - minS + 1];
        sin = new int[maxS - minS + 1];
        float angle = 125 * (float) Math.PI / 180;
        float dif = (320 * (float) Math.PI / 180) / (maxS - minS + 1);
        d = d / 2;
        for (int i = 0; i < maxS - minS + 1; i++, angle += dif) {
            cos[i] = (int) (d + (d * 0.6 * (float) Math.cos(angle))) - 3;
            sin[i] = (int) (d + (d * 0.6 * (float) Math.sin(angle))) - 3;
        }
        drag = false;
        addMouseMotionListener(this);
    }

    //////////////////////////////////////////////////////////////////////
    public void setValue(int v) {
        state = v;
        if (state > maxS) state = maxS;
        if (state < minS) state = minS;
        repaint();
    }

    //////////////////////////////////////////////////////////////////////
    public int getValue() {
        return state;
    }

    //////////////////////////////////////////////////////////////////////
    public Dimension getPreferredSize() {
        return new Dimension(diameter + 1, diameter + 1);
    }

    //////////////////////////////////////////////////////////////////////
    public Dimension getMinimumSize() {
        return new Dimension(10, 10);
    }

    //////////////////////////////////////////////////////////////////////
    public void paint(Graphics g) {
        g.setColor(Color.lightGray);
        g.fillOval(0, 0, diameter, diameter);
        g.setColor(drag ? ON : OFF);
        int xx = cos[state - minS], yy = sin[state - minS];
        g.fillOval(xx, yy, 6, 6);
        g.setColor(Color.gray);
        g.drawArc(0, 0, diameter, diameter, 225, 180);
        g.drawArc(xx, yy, 6, 6, 45, 180);
//int medio=diameter/2;
//g.drawRect(medio,medio,1,1);
        g.setColor(Color.white);
        g.drawArc(0, 0, diameter, diameter, 45, 180);
        g.drawArc(xx, yy, 6, 6, 225, 180);
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
    public void mouseDragged(MouseEvent e) {
        int x, y, st;
        double alfa;

        x = e.getX() - diameter / 2;
        y = e.getY() - diameter / 2;
        if (x < 0)
            alfa = (Math.PI / 2) + Math.atan((double) y / x);
        else
            alfa = (Math.PI * 3 / 2) + Math.atan((double) y / x);
        alfa = (alfa / (320 * Math.PI / 180));
        alfa = alfa - 0.0625;
        if (alfa >= 0 && alfa <= 1) {
            st = (int) (alfa * (maxS - minS + 1)) + minS;
            if (st != state) {
                state = st;
                if (actionListener != null)
                    actionListener.actionPerformed(new ActionEvent(this, ActionEvent.ACTION_PERFORMED, getName()));
                repaint();
            }
        }
    }

    //////////////////////////////////////////////////////////////////////
    public void mouseMoved(MouseEvent e) {
    }

}
