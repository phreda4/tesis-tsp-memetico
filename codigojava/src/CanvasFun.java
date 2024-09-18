////////////////////////////////////////////////////////////////////////
////
////	CanvasFun.java
////	Canvas para una funcion
////
////////////////////////////////////////////////////////////////////////
//
//import java.awt.*;
//import java.util.*;
//import java.io.*;
//import java.lang.*;
//
////////////////////////////////////////////////////////////////////////
//class CanvasFun extends Canvas {
//
//    private float px, py;            // Constantes de proporcion
//    private float xmin, ymin, xmax, ymax;    // limites de los puntos
//
//    private Vector tiempos;
//    private Vector numeros;
//
//    //////////////////////////////////////////////////////////////////////
//    public void paint(Graphics g) {
//    }
//
//
//    //////////////////////////////////////////////////////////////////////
//    public void init(int c) {
//        cntCitys = c;
//        x = new int[cntCitys];
//        y = new int[cntCitys];
//        arcos = new Hashtable();
//        puntos = true;
//    }
//
//    //
////	Ajustar coordenadas
////
////////////////////////////////////////////////////////////////////////
//    public void adjust(City2D[] citys, float x1, float x2, float y1, float y2) {
//        Dimension d = size();
//        xmin = x1;
//        xmax = x2;
//        ymin = y1;
//        ymax = y2;
//        float px = (d.width - 8) / (xmax - xmin);
//        float py = (d.height - 8) / (ymax - ymin);
//        for (int i = 0; i < cntCitys; i++) {
//            x[i] = 4 + (int) ((citys[i].x - xmin) * px);
//            y[i] = 4 + (int) ((citys[i].y - ymin) * py);
//        }
//    }
//
//
//}
