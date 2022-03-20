#include<omp.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

double dt;

float pi[4][4];

double monteCarlo(int threads, long n){
    int ct=0;
    dt=omp_get_wtime();
    #pragma omp parallel num_threads(threads) reduction(+:ct)
    {
        unsigned int seed = time(NULL);
        float x,y;
        #pragma omp for
        for(long i=0;i<n;i++){
            x=(double)rand_r(&seed)/(double)RAND_MAX;
            y=(double)rand_r(&seed)/(double)RAND_MAX;
            if(x*x+y*y<=1)
                ct++;
        }
    }
    dt=omp_get_wtime()-dt;
    int i=0,j=0;
    switch(n){
        case (int)1e7: i=0;break;
        case (int)2e7: i=1;break;
        case (int)4e7: i=2;break;
        case (int)8e7: i=3;break;
    }
    switch(threads){
        case 1: j=0;break;
        case 2: j=1;break;
        case 4: j=2;break;
        case 8: j=3;break;
    }
    pi[i][j]=(double)ct*4.0/(double)n;
    return dt;
}

void main(){
    int threads[]={1,2,4,8};
    long n[]={1e7,2e7,4e7,8e7};

    printf("%10s"," ");
    for(int i=0;i<4;i++)
        printf("%10d",threads[i]);
    
    printf("\n");
    for(int i=0;i<4;i++){
        printf("%10li",n[i]);
        for(int j=0;j<4;j++)
            printf("%10f",monteCarlo(threads[j],n[i]));
        printf("\n");
    }

    printf("Pi estimates\n");
    printf("%10s"," ");
    for(int i=0;i<4;i++)
        printf("%10d",threads[i]);
    
    printf("\n");
    for(int i=0;i<4;i++){
        printf("%10li",n[i]);
        for(int j=0;j<4;j++)
            printf("%10f",pi[i][j]);
        printf("\n");
    }
}