RegOrdP aux;	// auxiliar para cambio
long central;	// es un dato

void Escena::quickSortP(short primero,short ultimo)
{
short i=primero;
short j=ultimo;
central=poligonos[(primero+ultimo)>>1].dist;
do {
 while (poligonos[i].dist>central) i++;
 while (poligonos[j].dist<central) j--;
 if (i<=j)
   {
   aux=poligonos[i];poligonos[i]=poligonos[j];poligonos[j]=aux;
   i++;j--;
   }
} while (i<=j);
if (primero<j)
  quickSortP(primero,j);
if (i<ultimo)
  quickSortP(i,ultimo);
}
