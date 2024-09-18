////////////////////////////////////////////////////////////////////////
////
////  Ver Animacion
////
////////////////////////////////////////////////////////////////////////
//import java.awt.*;
//import java.awt.event.*;
//
///////////////////////////////////////////////////////////////////////////
//class TspVer extends Frame implements WindowListener,ActionListener {
//
//private Button btnGen=new Button("Comenzar");
//private Button btnSal=new Button(" Salir  ");
//private TextField tfAni=new TextField("",30);
//private Button btnArc=new Button("()");
//
///////////////////////////////////////////////////////////////////////////
//public TspVer()
//{
//setTitle("Animacion de TSP");
//setBackground(Color.lightGray);
//
//Panel p1=new Panel();
//p1.add(new Label("Solucion:"));
//
//Panel p2=new Panel();
//p2.add(tfAni);
//p2.add(btnArc);
//
//Panel p3=new Panel(new GridLayout(2,1));
//p3.add(p1);
//p3.add(p2);
//
//Panel p4=new Panel();
//p4.add(btnGen);
//p4.add(btnSal);
//
//setLayout(new BorderLayout());
//add(p3,"Center");
//add(p4,"South");
//
//pack();
//addWindowListener(this);
//btnGen.addActionListener(this);
//btnSal.addActionListener(this);
//Dimension d=getSize();
//Toolkit tk=getToolkit();
//Dimension dp=tk.getScreenSize();
//setLocation((dp.width/2)-(d.width/2),(dp.height/2)-(d.height/2));
//}
////
////  Interface para Window Listener
////
////////////////////////////////////////////////////////////////////////
//public void windowClosed(WindowEvent event) {}
//public void windowDeiconified(WindowEvent event) {}
//public void windowIconified(WindowEvent event) {}
//public void windowActivated(WindowEvent event) {}
//public void windowDeactivated(WindowEvent event) {}
//public void windowOpened(WindowEvent event) {}
//public void windowClosing(WindowEvent event)
//{
//dispose();
//}
//
////
////  Interface para Action Listener
////
////////////////////////////////////////////////////////////////////////
//public void actionPerformed(ActionEvent e)
//{
//Object source=e.getSource();
//if (source==btnGen)
//  {
//  VarTsp varTsp(tfAni.text);
//  varTsp.loadInstance();
//  varTsp.show();
//  }
//else if (source==btnArc)
//  {
//  FileDialog d=new FileDialog(this,"Carga Solucion ",FileDialog.LOAD);
//  d.setFile("*.fig");
//  d.show();
//  String f=d.getFile();
//  if (f!=null)
//    canvasTsp.saveFig(f);
//  }
//else if (source==btnSal)
//  { dispose(); }
//}
//
//}