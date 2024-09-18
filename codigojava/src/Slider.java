////////////////////////////////////////////////////////////////////////
////
////	Slider
////
////////////////////////////////////////////////////////////////////////
//import java.awt.*;
//import java.awt.image.*;
//import java.awt.event.*;
//
////////////////////////////////////////////////////////////////////////
//public class Slider extends Component implements MouseMotionListener {
//
//boolean drag;
//int state;
//int minS=0,maxS=1;
//ActionListener actionListener;
//
////////////////////////////////////////////////////////////////////////
//public Slider(int mi,int ma)
//{
//minS=mi;maxS=ma;
//state=minS;
//drag=false;
//addMouseMotionListener(this);
////enableEvents(AWTEvent.MOUSE_EVENT_MASK);
//}
//
////////////////////////////////////////////////////////////////////////
//public void setValue(int v)
//{
//state=v;
//if (state>maxS) state=maxS;
//if (state<minS) state=minS;
//repaint();
//}
//
////////////////////////////////////////////////////////////////////////
//public int getValue()
//{
//return state;
//}
//
////////////////////////////////////////////////////////////////////////
//public Dimension getPreferredSize()
//{
//return new Dimension(80,20);
//}
//
////////////////////////////////////////////////////////////////////////
//public Dimension getMinimumSize()
//{
//return new Dimension(80,20);
//}
//
////////////////////////////////////////////////////////////////////////
//public void paint(Graphics g)
//{
//g.clearRect(1,1,78,18);
//g.draw3DRect(0,0,79,19,true);
//
//g.setColor(Color.lightGray);
//g.fillOval(0,0,diameter,diameter);
//g.setColor(drag?ON:OFF);
//int xx=cos[state-minS],yy=sin[state-minS];
//g.fillOval(xx,yy,6,6);
//g.setColor(Color.gray);
//g.drawArc(0,0,diameter,diameter,225,180);
//g.drawArc(xx,yy,6,6,45,180);
////int medio=diameter/2;
////g.drawRect(medio,medio,1,1);
//g.setColor(Color.white);
//g.drawArc(0,0,diameter,diameter,45,180);
//g.drawArc(xx,yy,6,6,225,180);
//}
//
////////////////////////////////////////////////////////////////////////
//public void addActionListener(ActionListener listener)
//{
//actionListener = AWTEventMulticaster.add(actionListener, listener);
//enableEvents(AWTEvent.MOUSE_EVENT_MASK);
//}
//
////////////////////////////////////////////////////////////////////////
//public void removeActionListener(ActionListener listener)
//{
//actionListener = AWTEventMulticaster.remove(actionListener, listener);
//}
//
////////////////////////////////////////////////////////////////////////
//public void mouseDragged(MouseEvent e)
//{
//int x,y,st;
//double alfa;
//
//if(actionListener != null)
//  actionListener.actionPerformed(new ActionEvent(this, ActionEvent.ACTION_PERFORMED,getName()));
//x=e.getX()-diameter/2;
//y=e.getY()-diameter/2;
//if (x<0)
//  alfa=(Math.PI/2)+Math.atan((double)y/x);
//else
//  alfa=(Math.PI*3/2)+Math.atan((double)y/x);
//alfa=(alfa/(320*Math.PI/180));
//alfa=alfa-0.0625;
//if (alfa>=0 && alfa<=1)
//  {
//  st=(int)(alfa*(maxS-minS+1))+minS;
//  if (st!=state)
//    {
//    state=st;
//    repaint();
//    }
//  }
//}
//
////////////////////////////////////////////////////////////////////////
//public void mouseMoved(MouseEvent e)
//{
//}
//
//}