#include "mpi.h"
#include <time.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cmath>
#include <iomanip>
using namespace std;
using std::setw;


float Perimetro (float a[2], float b[2], float c[2], float y){
  y = y + sqrt((b[0] - a[0])*(b[0] - a[0] ) + (b[1] - a[1])*(b[1] - a[1]));
  //printf("El distancia es : %f \n", y);
  y = y + sqrt((c[0] - b[0])*(c[0] - b[0] ) + (c[1] - b[1])*(c[1] - b[1]));
  //printf("El PERIMETRO es : %f \n", y);
  y = y + sqrt((a[0] - c[0])*(a[0] - c[0] ) + (a[1] - c[1])*(a[1] - c[1]));
  //printf("El PERIMETRO es : %f \n", y);
  return y;
}

void ObtenerPuntos(float matriz[5000][3]){
  FILE *fichero;
  fichero = fopen("puntos","r");
  if (fichero==NULL){
    printf( "No se puede abrir el fichero.\n" );
    system("pause");
    exit (EXIT_FAILURE);
  }
  int i = 0;
  while (1){
    if (feof(fichero))
      break;
    fscanf (fichero, "%f %f %f\n", &matriz[i][0], &matriz[i][1], &matriz[i][2]);
    i++;
   }
   fclose(fichero);
}

void leerTriangulos(float x, float matriz[5000][3], int li, int lf){
  FILE *fichero;
  fichero = fopen("triangulos","r");
  fseek(fichero,li,SEEK_SET);
  if (fichero==NULL){
    printf( "No se puede abrir el fichero.\n" );
    exit (EXIT_FAILURE);
  }
  int i = 0;
  for (li; li<=lf; li++){
    if (feof(fichero));
      break;
    int a,b,c;
    fscanf (fichero, "%d %d %d\n", &a, &b, &c);
    //printf("Los puntos son : %d %d %d\n", a,b,c);
    float A[2],B[2],C[2];
    A[0]=matriz[a-1][1];
    //printf("El X1 es : %f \n", A[0]);
    A[1]=matriz[a-1][2];
    //printf("El Y1 es : %f \n", A[1]);
    B[0]=matriz[b-1][1];
    //printf("El X2 es : %f \n", B[0]);
    B[1]=matriz[b-1][2];
    //printf("El Y2 es : %f \n", B[1]);
    C[0]=matriz[c-1][1];
    //printf("El X3 es : %f \n", C[0]);
    C[1]=matriz[c-1][2];
    //printf("El Y3 es : %f \n", C[1]);
    x = Perimetro(A, B, C, x);
    i++;
   }
   fclose(fichero);
   printf("La suma de todos los perimetros es : %f \n", x);
}
 
int main(int argc, char *argv[]){
  MPI_Status estado;
  int status,rank_actual,cant_proce;
  int limite[2];
  float puntos[5000][3];

  MPI_Init(&argc, &argv);  // Inicio de MPI   
  MPI_Comm_size(MPI_COMM_WORLD, &cant_proce);  // numero de procesadores 
  MPI_Comm_rank(MPI_COMM_WORLD, &rank_actual); // rango de procesadores 

  if(rank_actual == 0){//procesador 0 comienza a trabajar
    for(int i=0; i<cant_proce; i++){
      ObtenerPuntos(puntos);
      limite[0] = (9665/cant_proce);//limite inferior (dependiendo de la cantidad de proce dividimos los numeros a procesar) 
      limite[1] = (9665/cant_proce);//limite superior 
      status = MPI_Send(limite, 2, MPI_INT, i, 0, MPI_COMM_WORLD);//envio limites
      status = MPI_Send(&puntos[5000][3], 5000*3, MPI_INT, i, 0, MPI_COMM_WORLD);
    }

    status = MPI_Recv(limite, 2, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &estado);//recibo limite para el proce 0
    status = MPI_Recv(&puntos[5000][3], 5000*3, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &estado);
    float perimetro=0;
    leerTriangulos(perimetro, puntos, limite[0],limite[1]);
  }
  else{//para los demás procesadores 
    status = MPI_Recv(limite, 2, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &estado);//recibo limite para el proce 0
    status = MPI_Recv(&puntos[5000][3], 5000*3, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &estado);
    float perimetro=0;
    leerTriangulos(perimetro, puntos, limite[0],limite[1]);
  }
  MPI_Finalize();
}
