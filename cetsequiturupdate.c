#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#include "estructuras.h" 
#include "cetsequitur.h"
#include "archivos.h"
#include "visualiza.h"

main(int argc, char* argv[])
{
   // Declaracion de variables
   struct simbolo *S;
   FILE *fp;

   char   *nom_archivo;
   char   *nom_salida;
   char   *aux_salida;
   int    rango_ini, rango_fin;
   int    i,j;
   
   // Primer parámetro nombre del archivo que contiene los datos
   // a analizar con sequitur.
   nom_archivo = (char *)malloc (TAM_NOM* sizeof(char));
   strcpy(nom_archivo,argv[1]);
   
   // Segundo parametro nombre del archivo donde se encuentra 
   // la estructura a actualizar
   nom_salida = (char *)malloc (TAM_NOM* sizeof(char));
   strcpy(nom_salida,argv[2]);
   aux_salida = (char *)malloc (TAM_NOM* sizeof(char));
   strcpy(aux_salida ,argv[2]);
   
   // Tercer y cuarto parámetro rango de elementos que debe leer
   // Tercer parámetro rango inicial
   rango_ini = atoi(argv[3]);
   // Cuarto parámetro rango final
   rango_fin = atoi(argv[4]);
   
   
   S = recupera_reglas(nom_salida);
   
   //printf("\n \n ...   REGLAS 2   ...");
   S = Leer_Archivo(S,nom_archivo,rango_ini,rango_fin);
   
   i = strlen(nom_salida);
   j = i - 4;
   strncpy(aux_salida,nom_salida,j);
   aux_salida[j] = '\0';
   //strcat(aux_salida,"update");
   
   ver_reglas(S,aux_salida);
   
   guardar_reglas(S,aux_salida);
}


