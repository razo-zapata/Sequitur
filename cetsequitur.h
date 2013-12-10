#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

/*************************************************************************
**   F U N C I O N E S                                                  **
*************************************************************************/
struct simbolo *sequitur(struct simbolo *S, char dato[TAM_INFO]);
struct simbolo *buscar_digrama(struct simbolo*S);
struct simbolo *crear_digrama(struct simbolo *S, struct simbolo *M);

struct simbolo *crear_regla(struct simbolo *S, struct simbolo *L);
struct simbolo *valida_regla(struct simbolo *S);
struct simbolo *intercambiar(struct simbolo *S, struct simbolo *R, struct simbolo *RC);

struct simbolo *reemplazar(struct simbolo *S,struct simbolo *L);
struct simbolo *vincula_simbolo(struct simbolo *S, struct simbolo *R, struct simbolo *B);
struct simbolo *remueve_simbolo(struct simbolo *S, struct simbolo *B);

struct simbolo *utilidad_reglas(struct simbolo *S);
struct simbolo *sustituir_contenido(struct simbolo *S,struct simbolo *M);
struct simbolo *reasignar_numeros(struct simbolo *S,struct simbolo *B);
void liberar_regla(struct simbolo *S);
   
void            guardar_reglas(struct simbolo *p, char *nom_archivo);
struct simbolo *recupera_reglas(char *nom_archivo);

struct simbolo *sequitur(struct simbolo *S, char dato[TAM_INFO])
{  
   // A L G O R I T M O  -- S E Q U I T U R
   struct simbolo *L;
   int    ban;
   ban = 1;
   // PASO 1
   // Para cada nuevo simbolo de entrada observado, agregarlo a la regla S.
   S = insertar(S,dato);
   
   // ban = 0;
   // PASO 2 Sequitur.
   // Cada vez que se hace una unión entre dos simbolos.
   while(ban != 0)
   {
      L   = buscar_digrama(S);
      ban = 0;
      // PASO 3
      // Si el nuevo digrama se repite en cualquier parte, y las repeticiones
      // "No se sobreponen"
      if(L != NULL)
      {
         // Ya existe el digrama
         // PASO 4 Si la ocurrencia es una regla
         if(L->numero > 0) // Es una regla
         {
            // PASO 5 Reemplazar el digrama con los simbolos de la regla
           S   = reemplazar(S,L);
           ban = 1;
         }
         else // PASO 6 - No es una regla
         {
            // PASO 7 Crear una nueva regla y reemplazar ambos digramas con los
            //        simbolos no terminales
            S   = crear_regla(S,L);
            //   PASO 7-1 No indicado en el articulo, pero implementado
            //            en el programa original de sequitur.
            S   = valida_regla(S);
            ban = 1;
         }
      }
      // PASO 8 - si no existe el digrama
         // Paso 9 - insertar digrama. La inserción se da indirectamente.
      
      // PASO 10 - Cada vez que es reemplazado un digrama por un 
      //           simbolo no terminal
      // PASO 11 - Si cualquier simbolo es un simbolo no terminal que 
      //           solo ocurre una vez en cualquier parte.
      // PASO 12 - Remover regla, sustituyendo su contendio en lugar
      //           de los otros simbolos no terminales
      S = utilidad_reglas(S);
      
   }
   free(L);
   return S;
}// FIN struct simbolo *sequitur(struct simbolo *S, char dato[TAM_INFO])
   
struct simbolo *buscar_digrama(struct simbolo *S)
{
   // S-> Estructura general
   // El simbolo que se ha insertado al último de la regla S,
   // debe ser buscado junto con el penúltimo a lo largo de la
   // regla S. Si se encuentra una repeticion que no se sobrepone
   // se debe agregar una regla. 
   // Si no se encuentra en S, buscarlo dentro de las reglas.   
   struct simbolo *L; // Regresa NULL si no se encontro la repeticion.
                      // Regresa S -> a*, b, c, d, a, b ó
                      // Regresa S -> (1), d, (1) , a, b
                      //         1 -> *a , b, c
                      // Regresa una regla, si el digrama encontrado es
                      // una regla completa.
   
   struct simbolo *D; // En esta parte se crea el digrama a buscar.
   struct simbolo *Aux1, *Aux2, *Aux3;
   int            b,ban;
   b = 0;
   
   // Ultimo par de simbolo insertados en S
   D = crear_digrama(S,S->su->sa);
   
   Aux1 = S;
   
   ban  = 1;
   
   L = NULL;
   while( (Aux1 != NULL) && (ban != 0))
   {
      //Aux2 = Aux1->ss;
      Aux2 = Aux1;
      while(Aux2->ss != NULL)
         Aux2 = Aux2->ss;
      //while( (Aux2->ss != NULL) && (ban != 0) )
      Aux2 = Aux2->sa;
      while( (Aux2->sa != NULL) && (ban != 0) )
      {
         Aux3 = crear_digrama(S,Aux2);
         
         ban = compara_digramas(D,Aux3);
         if(ban == 0) // Se encontro una repeticion del digrama
         {
            // Que el digrama encontrado no sea el mismo que se busca
            if(Aux2->id == S->su->sa->id)
               ban = 1;
            else
            {
               if(S->su->sa->id != Aux2->ss->id)
               {
                  // Detalle ******************
                  // (8) -82 != (3)  -82 -82 -82 = -82 -82 -82
                  // (3) -> -82 (8)  
                  // (8) -> -82 -82  
                  //ban = compara_elementos(S->su->sa,Aux2);
                  //if(ban == 0)
                     break;
               }
               else
                  ban = 1;
            }
         }
         //Aux2 = Aux2->ss;
         Aux2 = Aux2->sa;
         free(Aux3);
      }
      Aux1 = Aux1->sr;
   }
   if( (ban == 0) && (S->su->sa->id != Aux2->ss->id) )
   {
      // El digrama encontrado no es el mismo que se busca
      if(Aux2->id != S->su->sa->id)
      {
         // Los digramas no se deben sobreponer
         
         L = Aux2;
         // Si el digrama encontrado es una regla completa
         // Se debe regresar la regla.
         // Nos posicionamos en lo que podria ser el inicio de la regla
         if(Aux2->sa != NULL) 
         {  
            Aux1 = Aux2->sa; // Posible Regla
            if(Aux1->numero > 0)
            {
               // Aux1 tiene una regla
               // D    tiene el digrama bucado
               ban = compara_digregla(Aux1,D);
               if(ban == 0) // El digrama es igual a la regla
                  L = Aux1;
            }
         }  
      }
   }
   
   free(D);
   
   return L;
}// FIN struct simbolo *buscar_digrama(struct simbolo*S);
     
struct simbolo *crear_digrama(struct simbolo *S, struct simbolo *M)
{  
   // S-> Estructura general.
   struct simbolo *D; // En esta parte se crea el digrama a buscar.
                      // Si S-> (1) , b , c, (1), d
                      //    1-> a, b, c
                      // D -> a, b, c, d
   struct simbolo *Aux1,*Aux2,*Aux3,*Aux4;
   Aux1 = M;
   Aux2 = M->ss;
   
   D    = inicializa_simbolo(-2);
   // Primera parte digrama
   if(Aux1->regla == NULL)
   {
      strcpy(D->info,Aux1->info);
      Aux3 = D;
   }
   else
   {
      D    = expande_regla(Aux1->regla);
      Aux3 = D;
      while(Aux3->ss != NULL)
      {
         Aux3 = Aux3->ss;
      }
   }
   // Segunda parte digrama
   Aux4 = inicializa_simbolo(-2);
   if(Aux2->regla == NULL)
   {
      strcpy(Aux4->info,Aux2->info);
   }
   else
   {
      Aux4 = expande_regla(Aux2->regla);
   }
   Aux3->ss = Aux4;
   // No funciona aqui un free(Aux4);
   return D;
}// FIN struct simbolo *crear_digrama(struct simbolo *S)
   
struct simbolo *crear_regla(struct simbolo *S, struct simbolo *L)
{
   struct simbolo *Nuevaregla;
   struct simbolo *M;
   struct simbolo *Aux1, *Aux2, *Aux3, *Aux4;
   int            b;
   
   // Ir hasta la ultima regla
   M = S;
   while(M->sr != NULL)
      M = M->sr;
   
   // Generar nuevo simbolo
   S->nid++;
   S->consecutivo++;
   Nuevaregla = inicializa_simbolo(S->nid);
   Nuevaregla->numero = S->consecutivo;
   strcpy(Nuevaregla->info,"@");
   
   M->sr          = Nuevaregla; // Vincular con ultima regla
   Nuevaregla->ra = M;
   
   // Copiar su contenido
   // Los ultimos dos simbolos en la regla S pueden ser:
   // 1.- simbolo, simbolo    2.- simbolo, regla
   // 3.- regla   , simbolo   4.- regla   , regla
   
   // Ubicarse en el penultimo simbolo
   Aux1 = S->su->sa;
   Aux3 = Nuevaregla;
   b    = 0;
   while(Aux1 != NULL)
   {  
      // Nuevo elemento de la regla
      S->nid++;
      Aux4 = inicializa_simbolo(S->nid);

      if(Aux1->regla == NULL)
      {
         strcpy(Aux4->info,Aux1->info);
      }
      else
      {
         Aux2 = expande_regla(Aux1->regla);
         
         Aux4->regla = Aux1->regla;
         // decrementar regla Aux1->regla con Aux1.
         Aux1->regla = remueve_simbolo(Aux1->regla, Aux1);

         // incrementar regla Aux4->regla con Aux4
         Aux4->regla = vincula_simbolo(S, Aux4->regla, Aux4);
      }
      
      Aux3->ss = Aux4;
      Aux4->sa = Aux3;
      Aux3     = Aux4;
      Aux1     = Aux1->ss;
   }  
      
   // Crear ligas y remover la necesarias.
   // Quitar ultimo par de elementos en la regla S
   S->nid++;
   Aux1 = inicializa_simbolo(S->nid);
   Aux1->regla = Nuevaregla;
   Aux2 = S->su->sa; // penultimo     simbolo
   Aux3 = S->su;     // ultimo        simbolo
   Aux4 = Aux2->sa;  // antepenultimo simbolo
   Aux4->ss = Aux1;
   Aux1->sa = Aux4;
   S->su    = Aux1;
   Nuevaregla = vincula_simbolo(S, Nuevaregla, Aux1);
   
   free(Aux2);// libera penultimo simbolo
   free(Aux3);// libera ultimo simbolo
   
   while(L != NULL)
   {
      // Quitar par de elementos donde inicia L
      S->nid++;
      Aux1 = inicializa_simbolo(S->nid);
      Aux1->regla = Nuevaregla;
      Aux2  = L;         // Primer  elemento del digrama
      Aux3  = Aux2->ss;  // Segundo elemento del digrama
      Aux4  = Aux3->ss;  // Siguiente simbolo
      M     = L->sa;     // Simbolo anterior al digrama.
      
      // Enlaces
      M->ss    = Aux1; // Simbolo anterior al digrama se 
      Aux1->sa = M;    // enlaza con el nuevo simbolo.
      if(Aux4 != NULL) // Si el simbolo siguiente al digrama
      {                // no es nulo, se debe enlazar.
         Aux4->sa = Aux1;
         Aux1->ss = Aux4;
      }
      
      Nuevaregla = vincula_simbolo(S, Nuevaregla, Aux1);
      // Remover enlaces de reglas de Aux2 y Aux3 en caso de existir reglas
      if(Aux2->regla != NULL)
         Aux2->regla = remueve_simbolo(Aux2->regla,Aux2);
      if(Aux3->regla != NULL)
         Aux3->regla = remueve_simbolo(Aux3->regla,Aux3);
      
      L = NULL;
   }// FIN while( L != NULL)
   
   free(Aux2);
   free(Aux3);
   
   return S;
}// Fin de crear regla
   
struct simbolo *vincula_simbolo(struct simbolo *S, struct simbolo *R, struct simbolo *B)
{  
   // Esta función permite enlazar un simbolo a una regla, con la 
   // finalidad de saber a quienes direcciona una regla.
   // R -> regla a la que se desea unir el simbolo.
   // B -> Simbolo
   struct simbolo *Enlace;
   struct simbolo *recorre;
   
   S->nid++;
   Enlace      = inicializa_simbolo(S->nid);
   Enlace->ls  = NULL;
   Enlace->la  = NULL;
   Enlace->ss = B; // Aquí se  enlaza el simbolo.
   
   if(R->ls == NULL)
   {
      // Primer enlace.
      R->ls    = Enlace;
   }
   else // Ya hay simbolos vinculados a esta regla
   {
      // Ir hasta el ultimo enlace
      recorre = R->ls;
      while(recorre->ls != NULL)
         recorre = recorre->ls;
      recorre->ls = Enlace;
      Enlace->la  = recorre;
   }
   R->num_l++;
   return R;
}// Fin vinvula simbolo
   
struct simbolo *remueve_simbolo(struct simbolo *S, struct simbolo *B)
{
   // Esta función permite remover el vinculo que tiene un simbolo 
   // con una regla, con la finalidad de saber a quienes direcciona una regla.
   // S -> regla a la que se remover el simbolo.
   // B -> simbolo que se quiere remover
   struct simbolo *M, *Aux1, *Aux2, *Aux3;
   int            ban;
   ban = 0;
   
   S->num_l--;
   M = S->ls;
   if(M != NULL)
   {
      while( (M != NULL) && (ban == 0) )
      {
         if(S->ls->ss->id == B->id)
         {
            // Se quiere borrar el primer enlace
            Aux1  = S->ls;
            Aux2  = Aux1->ls;
            S->ls = NULL;
            if(Aux2 != NULL) // Hay mas enlaces
            {
               S->ls    = Aux2;
               Aux2->la = NULL;
            }
            free(Aux1);
            ban = 1;
         }
         else // Se quiere borrar otro enlace
         {
            if(M->ss->id == B->id) 
            {
               // Encontro el simbolo y debe eliminar el enlace
               Aux1 = M;
               Aux2 = M->ls;    // Link siguiente
               Aux3 = M->la;    // Link anterior
               if(Aux2 != NULL) // Hay mas enlaces hacia adelante
               {
                  Aux3->ls = Aux2;
                  Aux2->la = Aux3; 
               }
               else
               {
                  Aux3->ls = NULL;
               }
               free(Aux1);
               ban = 1;
            }
         }
         M = M->ls; // Siguiente link
      }
   }// FIN de if(B != NULL)
   
   return S;
}// Fin remueve simbolo
   
struct simbolo *reemplazar(struct simbolo *S,struct simbolo *L)
{
   // Esta función permite reemplazar el último digrama de la regla S
   // por la regla L.
   // S -> Estructura general
   // L -> Regla x la que se reemplaza último digrama
   struct simbolo *NuevoSimbolo;
   struct simbolo *Aux1, *Aux2, *Aux3;
   
   // Digrama a remplazar
   Aux1 = S->su->sa; // Penultimo     simbolo
   Aux2 = S->su;     // Ultimo        simbolo
   Aux3 = Aux1->sa;  // Antepenultimo simbolo
   if(Aux3 != NULL)
   {
      S->nid++;
      NuevoSimbolo = inicializa_simbolo(S->nid);
      NuevoSimbolo->regla = L;
      L = vincula_simbolo(S,L,NuevoSimbolo);
      
      // Ajustar contadores de reglas en caso de existir 
      // en Aux1 y Aux2
      if(Aux1->regla != NULL)
         Aux1->regla = remueve_simbolo(Aux1->regla,Aux1);
      if(Aux2->regla != NULL)
         Aux2->regla = remueve_simbolo(Aux2->regla,Aux2);
      free(Aux1);
      free(Aux2);
      Aux3->ss         = NuevoSimbolo;
      NuevoSimbolo->sa = Aux3;
      S->su            = NuevoSimbolo;
   }
   
   return S;
}// Fin reemplazar
   
struct simbolo *utilidad_reglas(struct simbolo *S)
{
   // Esta función va a asegurar que las reglas que han sido creadas, 
   // se utilicen más de una vez.
   // S-> CAC
   // A-> bc
   // B-> aA   Dado que B sólo se emplea una vez
   // C-> Bd
   //
   // Debe quedar:
   // S-> CAC     S->212
   // A->bc       1->bc
   // C->aAd      2->a1d
   // 
   // Entonces b debe ser sustituida en donde aparezca.
   // Implica :
   //    * modificar enlaces en reglas y/o S.
   //
   struct simbolo *M;
   
   M = S->sr;
   while(M != NULL) // Recorrer cada una de las reglas
   {
      if(M->num_l < 2) // Este parametro lo consideraremos variable
      {
         
         // Sustituir su contenido en donde aparece .
         S = sustituir_contenido(S,M);
      }
      
      M = M->sr;
   }
   
   
   return S;
}// FIN utilidad reglas
   
struct simbolo *sustituir_contenido(struct simbolo *S,struct simbolo *M)
{
   // Esta función permite sustituir el contenido de una regla 
   // en los lugares en donde aparece.
   // S -> Estrcutura primaria
   // M -> Regla para sustituir su contenido
   
   struct simbolo *B; // Recorre el contenido de la regla.
   struct simbolo *S1;
   struct simbolo *Aux1, *Aux2, *Aux3, *Aux4; // Recorrer enlaces
   int    b, ban;
   
   Aux1 = M->ls;
   while(Aux1 != NULL)
   {
      B    = M->ss;    // Inicio del contenido de la regla.
      Aux2 = Aux1->ss->sa; // Simbolo anterior.
      Aux3 = Aux1->ss->ss;
      b    = 0;
      
      while(B != NULL)
      {
         Aux4 = NULL;
         ban  = 0;
         // Copiar contenido de la regla.
         S->nid++;
         S1 = inicializa_simbolo(S->nid);
         if(B->regla == NULL)
         {
            strcpy(S1->info,B->info);
         }
         else
         {
            S1->regla = B->regla;
            S1->regla = remueve_simbolo(S1->regla,B);
            Aux4      = B;
            ban       = 1;
            S1->regla = vincula_simbolo(S,S1->regla,S1);
         }
         Aux2->ss = S1;
         S1->sa   = Aux2;
         Aux2     = S1;
         
         B = B->ss;
         //if(ban == 1)
         //   free(Aux4);
         
         b++;
      }
      
      // Enlazar Aux2 con el contenido siguiente.
      if(Aux3 != NULL)
      {
         Aux3->sa = Aux2; // Simbolo siguiente
         Aux2->ss = Aux3;
      }
      Aux1 = Aux1->ls;// Cada enlace
   }
   
   // Remover regla del conjunto de reglas
   B = S->sr;
   
   if(B->numero == M->numero) // La regla a remover es la primera
   {
      Aux1 = B->sr;
      if(Aux1 != NULL)
      {
         S->sr    = Aux1;
         Aux1->ra = S;
      }
      if(Aux1->sr != NULL)
      {
         S = reasignar_numeros(S,Aux1->sr);
         Aux1->numero--;
      }
      else
      {
         Aux1->numero--;
         S->consecutivo--;
      }
   }
   else // Encontrar la regla a remover, para ubicar antecesor y sucesor.
   {
      Aux1 = B;
      b    = 0;
      while( (B != NULL) && (b == 0) )
      {
         if(B->numero == M->numero)
         {
            b = 1; // Tenemos la posicion de la regla
            break;
         }
         Aux1 = B;
         B    = B->sr;
      }
      // Aux1 es el antecesor
      // Aux2 es el sucesor
      Aux2 = M->sr;
      
      Aux1->sr = Aux2;
      if(Aux2 != NULL)
      {
         Aux2->ra = Aux1;
      }
      S = reasignar_numeros(S,Aux2);
      liberar_regla(B);
   }
   
   
   return S;
}// FIN sustituir contenido
   
struct simbolo *reasignar_numeros(struct simbolo *S,struct simbolo *B)
{
   // ver_regla(B);
   struct simbolo *Aux1;
   
   Aux1 = B;
   while(Aux1 != NULL)
   {
      Aux1->numero--;
      Aux1 = Aux1->sr;
   }
   S->consecutivo--;
   return S;
}// FIN reasignar numeros
   
struct simbolo *valida_regla(struct simbolo *S)
{
   // S -> Estructura general.
   // PASO 7.1 De sequitur.
   // Despues de crear una regla y cuyo segundo elemento es una regla.
   // S   -> (1), C, D, (1), E, F, (2), X, Y, (2), Z, Y, G
   // (1) -> A, B
   // (2) -> G, H
   // 
   // Se agrega el simbolo H
   // S   -> (1), C, D, (1), E, F, (3), X, (2), Z, (2)
   // (1) -> A, B
   // (2) -> Y, (3)
   // (3) -> G, H
   struct simbolo *M;         // recorrer reglas
   struct simbolo *R;         // Ultima regla
   struct simbolo *Aux1, *Aux2;
   int            Distancia;  // Distancia entre las reglas
   int            NContenida; // Número de la regla contenida en otra.
   int            ban;

   // Ir hasta la ultima regla
   M = S;
   while(M->sr != NULL)
      M = M->sr;

   // Ir hasta el ultimo elemento de la regla
   R = M;
   M = M->ss; 
   while(M->ss != NULL)
      M = M->ss;
   if(M->regla != NULL)
   {
      Distancia = R->numero - M->regla->numero;
      if(Distancia > 1)
      {
         
         // Poner la ultima regla por encima de la regla a la que contiene.
         // Y recorrer la numeración de las reglas restantes.
         NContenida = M->regla->numero;
         // Encontrar la regla contenida
         Aux1 = S->sr;
         ban  = 1;
         while( (Aux1 != NULL) && (ban != 0) )
         {
            if(Aux1->numero == NContenida)
               ban = 0;
            
            Aux2 = Aux1;
            Aux1 = Aux1->sr;
         }
         if(ban == 0)
            S = intercambiar(S,R,Aux2);
      }
   }
   return S;
}

struct simbolo *intercambiar(struct simbolo *S, struct simbolo *R, struct simbolo *RC)
{
   struct simbolo *Aux1, *Aux2, *Aux3;
   
   Aux1 = RC->sr; // Regla siguiente a la contenida
   Aux2 = R->ra;  // Regla a anterior a la regla a mover
   
   // Mover enlaces
   R->sr    = Aux1;
   Aux1->ra = R;
   RC->sr   = R;
   R->ra    = RC;
   Aux2->sr = NULL;
   
   R->numero = RC->numero;
   
   // Mover numeros
   while(R != NULL)
   {
      R->numero++;
      R = R->sr;
   }
   
   return S;
}

void liberar_regla(struct simbolo *S)
{
   //printf("\n \n LIBERAR REGLA .... ");
   
   struct simbolo *R;
   struct simbolo *Aux1;
   R = S;
   while(R != NULL)
   {
      Aux1 = R;
      R    = R->ss;
      free(Aux1);
   }

   //ver_regla(S);
}
  
void guardar_reglas(struct simbolo *p, char *nom_archivo)
{  
   struct simbolo *S; // Apuntador para reglas
   struct simbolo *M; // Apuntador para simbolos
   FILE *fp;
   char *aux, *comando;
   S = p;
   
   aux = (char *)malloc (50* sizeof(char));
   strcpy(aux,nom_archivo);
   strcat(aux,".dat");
   system("rm -R TempoSeq.dat");
   if(S != NULL)
   {  
      //Abrir archivo y guardar estructura en binario
      fp = fopen("TempoSeq.dat", "wb");
      if (fp != NULL)
      {
         // Recorrer regla x regla
         while(S != NULL)
         {
            M = S;
            // Recorrer elemento x elemento de la regla
            while(M != NULL)
            {
               // Si apunta a una regla guardar el numero de regla
               if(M->regla != NULL)
                  M->num_l = M->regla->numero;
               fwrite (M, sizeof(struct simbolo), 1, fp);
               M = M->ss;
            }
            S = S->sr;
         }
         fclose(fp);
      }
      //printf("\n Archivo generado %s",aux);
      comando = (char *)malloc (50* sizeof(char));
      sprintf(comando,"cp TempoSeq.dat %s",aux);
      system(comando);
   }
   else
   {
      printf("\n LA ESTRUCTURA ESTA VACIA");
   }
}  

void libera_estructura(struct simbolo *p)
{  
   struct simbolo *S;     // Apuntador para reglas
   struct simbolo *M, *B, *Aux1; // Apuntador para simbolos
   S = p;
   
   if(S != NULL)
   {  
      // Recorrer regla x regla
      while(S != NULL)
      {
         M    = S;
         // Recorrer elemento x elemento de la regla
         B = M;
         while(B != NULL)
         {
            Aux1 = B;
            B    = B->ss;
            free(Aux1);
         }
         S = S->sr;
      }
   }
   else
   {
      printf("\n LA ESTRUCTURA ESTA VACIA");
   }
}

struct simbolo *recupera_reglas(char *nom_archivo)
{
   struct simbolo *S;
   struct simbolo *Aux1, *Aux2, *Aux3, *Aux4;
   FILE *fp;
   int    ban1, ban2, ban3, i;
   ban1 = 0;
   ban2 = 0;
   ban3 = 0;
   
   //printf("\n\n RECUPERANDO CONJUNTO DE REGLAS ");   
   
   //fp = fopen("Reglas.dat", "rb");
   fp = fopen(nom_archivo,"rb");
   
   if (fp != NULL)
   {
      while (!feof(fp))
      {
         if(ban1 == 0)
         {
            S = (struct simbolo *)malloc(sizeof(struct simbolo));
            fread(S, sizeof(struct simbolo), 1, fp);
            ban1 = 1;
            //printf("\n ID=%d, INFO=%s, NID=%d \n",S->id,S->info,S->nid);
            Aux1  = S;
            S->sr = NULL;
            S->ra = NULL;
         }
         else
         {
            Aux2 = (struct simbolo *)malloc(sizeof(struct simbolo));
            fread(Aux2, sizeof(struct simbolo), 1, fp);            
            //printf("\n ID = %d, INFO = %s, numero = %d, num_l = %d ", 
            //          Aux2->id,Aux2->info,Aux2->numero,Aux2->num_l);
            // Lectura de simbolos de una regla
            if(ban2 == 0)
            {
               // Ultimo elemento de una regla
               if(Aux2->ss == NULL)
               {
                  if(ban3 == 0)
                  {
                     ban3  = 1;
                     S->su = Aux2;
                  }
                  ban2 = 1;
               }
               Aux1->ss = Aux2;
               Aux2->sa = Aux1;
               Aux1     = Aux2;
            }
            else // Lectura de primer elemento de una regla
            {
               ban2 = 0;
               Aux1 = S;
               while(Aux1->sr != NULL)
                  Aux1 = Aux1->sr;
               Aux1->sr    = Aux2;
               Aux2->ra    = Aux1;
               Aux2->ls    = NULL; // Como no guarda bien los enlaces
               Aux2->la    = NULL; // quitar los que tenga
               Aux2->num_l = 0;
               Aux2->sr    = NULL;
               Aux1        = Aux2;
            }
         }
      }// Fin de while
      fclose(fp);
   }// Fin de IF
   
   // Dado que recupera algo como esto, se deben quitar enlaces
   /* no validos.
   (0) ----
   (0*)
   (1) ------
   (0*)
   (2) --------
   (0*)
   */
   Aux1 = S;
   Aux2 = S->sr;
   while(Aux1->sr != NULL)
   {
      // No es una regla valida
      if( (Aux2->id < 1) && (Aux2->ls == NULL) )// (0*)
      {
         Aux3 = Aux2->sr;
         if(Aux3 != NULL)
         {
            Aux1->sr = Aux3;
            Aux3->ra = Aux1;
         }
      }
      Aux3 = Aux1;
      Aux1 = Aux1->sr;
      Aux2 = Aux1->sr;
   }
   if(Aux1 != NULL)
      if(Aux1->id < 1)
         Aux3->sr = NULL;
   
   Aux1 = S;
   while(Aux1 != NULL)
   {
      Aux2 = Aux1->ss;
      while(Aux2 != NULL)
      {
         // Si el elemento tiene enlace con una regla
         if(Aux2->num_l > 0)
         {
            // Buscar regla
            Aux3 = S->sr;
            while(Aux3 != NULL)
            {
               if(Aux3->numero == Aux2->num_l)
                  break;
               Aux3 = Aux3->sr;
            }
            Aux2->regla = Aux3;
            Aux2->regla = vincula_simbolo(S,Aux2->regla,Aux2);
         }
         Aux2 = Aux2->ss;
      }
      Aux1 = Aux1->sr;
   }
   //printf("\n Fin de recuperación");
   
   return S;
}
   
//FIN
