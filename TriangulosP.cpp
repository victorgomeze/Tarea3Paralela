#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctime>

unsigned TINICIO, TFINAL;



int Lineas(char fichero[], int linea)//buscar una linea en el archivo deseado
{
    int lugar;
    FILE *datos=fopen(fichero,"r");
    int contar=linea-1;
    fseek(datos, 0, SEEK_SET);//fseek situa el cursor en un lugar deseado
    prueba:     
    if (contar>0)  
    { 
    while (fgetc (datos) != '\n');
        contar--;
        goto prueba;
    }
    lugar=ftell(datos);//fteel para determinar el lugar del cursor de escritura/lectura
    fclose(datos);
    return lugar;

}
void Puntos(int triangulo, char fichero[], float punto[2]){//busca el valor de las cordenadas de los puntos de un triangulo
    int lado;
    float x, y;
    FILE *datos=fopen(fichero,"r");
    int lugar;
    lugar=Lineas("puntos",triangulo);
    fseek(datos,lugar,SEEK_SET);
    fscanf(datos,"%d %f %f", &lado,&x,&y);
    if (lado==triangulo){
        punto[0]=x;
        punto[1]=y;
    }
    fclose(datos);
}

float Perimetro (float A[2],float B[2],float C[2])//Calcula el perimetro entre los 3 puntos
{
    float ladoA,ladoB,ladoC;
    ladoA= sqrt((A[0]-B[0])*(A[0]-B[0])+(A[1]-B[1])*(A[1]-B[1]));
    ladoB= sqrt((B[0]-C[0])*(B[0]-C[0])+(B[1]-C[1])*(B[1]-C[1]));
    ladoC= sqrt((A[0]-C[0])*(A[0]-C[0])+(A[1]-C[1])*(A[1]-C[1]));
    return ladoA+ladoB+ladoC;
}


float Triangulos(char fichero[],int linea, int triangulo[3])//Obtiene los puntos del archivo triangulos
{
    float perimetro, punto1[2], punto2[2], punto3[2];
    FILE *datos = fopen(fichero,"r");
    if(!feof(datos)){
        int lugar;
        lugar=Lineas("triangulos",linea); 
        fseek(datos,lugar,SEEK_SET);
        fscanf(datos,"%d %d %d\n",&triangulo[0],&triangulo[1],&triangulo[2]);
        Puntos(triangulo[0],"puntos",punto1);
        Puntos(triangulo[1],"puntos",punto2);
        Puntos(triangulo[2],"puntos",punto3);
        perimetro=Perimetro(punto1,punto2,punto3);        
        fclose(datos);
        return perimetro;
    }
    else{
        return 0;
    }
}


int main(int argc, char* argv[])
{   
    TINICIO=clock();
    int rank_Actual, cant_Proce;
    int triangulo[3]={0,0,0};
    int A=0;
    int B=0;
    float C=0;
    float per=0;
    float perimetro_Total=0;
    MPI_Init(&argc,&argv);
    MPI_Comm_size( MPI_COMM_WORLD, &cant_Proce ); //cantidad de proces
    MPI_Comm_rank( MPI_COMM_WORLD, &rank_Actual );     // rank actual  
    if(9665%cant_Proce==0){//para hacer la diferencia entre los pares e impares
        A=9665/cant_Proce;
    }
    else{   
        A=9665/cant_Proce;
        B=9665%cant_Proce;
    }
    if(rank_Actual == 0){
        for(int i=1;i<=A+B;i++){
            per=Triangulos("triangulos",i,triangulo);
            C+=per;
            
        }
        MPI_Reduce(&C,&perimetro_Total,1,MPI_REAL,MPI_SUM,0,MPI_COMM_WORLD);//mpi_reduce reduce un valor de un grupo de procesos en un único proceso
        printf("%f\n",perimetro_Total);
    }
    else{
        for(int i =A*rank_Actual+B+1;i<=A*rank_Actual+A+B;i++){
            per=Triangulos("triangulos",i,triangulo);
            C+=per;
            
        }
        MPI_Reduce(&C,&perimetro_Total,1,MPI_REAL,MPI_SUM,0,MPI_COMM_WORLD);          
    }
    MPI_Finalize();
    TFINAL=clock();
    double time = (double(TFINAL-TINICIO)/CLOCKS_PER_SEC);
    printf("El tiempo de ejecucion es: %f\n" ,time);
    return 0;  
}
