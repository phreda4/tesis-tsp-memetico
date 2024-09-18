/////////////////////////////////////////////////////////
//
//      Interface para animar variables
//
/////////////////////////////////////////////////////////

import java.io.*;

interface VarAni {
    public String getName();

    public void addEvn(Tiempo t, StreamTokenizer s);

    public void endEvn();

    public void setTime(Tiempo t);

    public Tiempo getNextTime();

    public Tiempo getPrevTime();
}
