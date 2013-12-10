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
   struct simbolo *p,*q,*r;
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
   
   // Segundo parametro nombre del archivo de salida, sera un archivo binario
   nom_salida = (char *)malloc (TAM_NOM* sizeof(char));
   strcpy(nom_salida,argv[2]);
   aux_salida = (char *)malloc (TAM_NOM* sizeof(char));
   strcpy(aux_salida ,argv[2]);
   strcat(aux_salida,".txt");
   
   // Tercer y cuarto parámetro rango de elementos que debe leer
   // Tercer parámetro rango inicial
   rango_ini = atoi(argv[3]);
   // Cuarto parámetro rango final
   rango_fin = atoi(argv[4]);
     
   p = crear(p,"S");
   p = Leer_Archivo(p,nom_archivo,rango_ini,rango_fin);
   
   //printf("\n \n ...   REGLAS   ...");
   //Abrir archivo y guardar reglas
   ver_reglas(p,nom_salida);
   
   guardar_reglas(p,nom_salida);
   
   //libera_estructura(p);
   //printf("\n \n Simbolos totales %d \n \n ",p->total);
}


