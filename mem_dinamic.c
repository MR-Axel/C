/* Escribir un prog. en el que el usuario informa la cantidad de elementos que
va a ingresar y luego los valores de los elementos (enteros).
Los elementos deben almacenarse en un vector y luego en el archivo vector.dat.
No se debe limitar la cantidad de elementos.  */

#include <stdio.h>
#include <stdlib.h>

int main ()
{
  int cant=0, i;
  int *p, *aux;
  FILE *ar;
  ar= fopen ("vector.dat", "w");
  if (ar!=NULL)
  {
    printf("Cantidad de elementos a ingresar: ");
    scanf("%d", &cant );
    p = calloc (cant, sizeof(int));   //reservo la memoria con la cantidad ingresada
    int vec[cant];      //defino el vector con esa cantidad de elementos
    aux=p;          //uso una aux con valor p para mover y no perder el inicio del "vector"
    printf("Escriba los valores deseados: \n" );
    for (i = 0; i < cant; i++)        //empiezo a cargar valores a partir de esa direccion
    {
      scanf("%d", aux);
      vec[i]= *aux;       //y al mismo tiempo guardo ese dato en el vector
      aux++;
    }
    aux=p;      //vuelvo a darle la posicion original a aux
    printf("Los valores son: \n" );
    for (i = 0; i < cant; i++)
    {
      printf("%d ", vec[i] );      //muestro a ver si se guardó bien el vector
    }
    if (fwrite(vec, sizeof (vec[0]), cant, ar)!=cant)     //clásico de archivos, escribo en vector.dat
    {
      printf("\n Error al escribir.\n" );
    }
    else printf("\n Escritura ok. \n" );
    if (fclose(ar)!=0) {printf("Error al cerrar\n" );}
    else printf("\n Cerrado.\n" );
  }
  else
  {
    printf("Error al crear archivo.\n" );
  }
  free (p);
  free (aux);
  return 0;
}
