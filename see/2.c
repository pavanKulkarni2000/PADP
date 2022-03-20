#include<omp.h>
#include<stdio.h>
#include<stdlib.h>


int threads[]={1,2,4,8};
int size[]={500, 800, 1000, 1200};
int **A, **B, **C;

double matrixMultiply(int t,int n){
    double start=omp_get_wtime();
    #pragma omp parallel for num_threads(threads[t]) collapse(2)
    for(int i=0;i<size[n];i++)
        for(int j=0;j<size[n];j++)
        {
            C[i][j]=0;
            for(int k=0;k<size[n];k++)
                C[i][j]+=A[i][j]*B[i][j];
        }
    return omp_get_wtime()-start;
}
void main(){

    A=(int**)malloc(sizeof(int*)*2000);
    B=(int**)malloc(sizeof(int*)*2000);
    C=(int**)malloc(sizeof(int*)*2000);
    for(int i=0;i<2000;i++){
        A[i]=(int*)malloc(sizeof(int)*2000);
        B[i]=(int*)malloc(sizeof(int)*2000);
        C[i]=(int*)malloc(sizeof(int)*2000);
        for(int j=0;j<2000;j++){
            A[i][j]=rand();
            B[i][j]=rand();
        }
    }
    
    printf("%10s"," ");
    for(int i=0;i<4;i++)
        printf("%10d",threads[i]);
    
    printf("\n");
    for(int n=0;n<4;n++){
        printf("%10i",size[n]);
        for(int t=0;t<4;t++)
            printf("%10f",matrixMultiply(t,n));
        printf("\n");
    }
}