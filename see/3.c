#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#include<math.h>
#include<string.h>

double sieve1(int n){
    int *composite=(int*)malloc((n+1)*sizeof(int));
    memset(composite,0,sizeof(int)*(n+1));
    int lim=sqrt(n);
    int count=0;

    double t=omp_get_wtime();
    for(int i=2;i<=lim;i++)
        if(!composite[i]){
            count++;
            for(int j=i*i;j<=n;j+=i)
                composite[j]=1;
        }
    for(int i=lim+1;i<=n;i++)
        if(!composite[i]) count++;
    t=omp_get_wtime()-t;

    printf("%f\n",t);
}

double sieve2(int n){
    int lim=sqrt(n);
    int *composite=(int*)malloc(sizeof(int)*(lim+1));
    memset(composite,0,sizeof(int)*(lim+1));
    int count=0;
    int *primes=(int*)malloc(lim*sizeof(int));
    int np=0;

    double t=omp_get_wtime();
    for(int i=2;i<=lim;i++)
    if(!composite[i])
    {
        count++;
        primes[np++]=i;
        for(int j=i*i;j<=lim;j+=i)
            composite[j]=1;
    }

    for(int l=lim;l<n;l+=lim){
        int h=l+lim;
        if(h>n) h=n;
        memset(composite,0,sizeof(int)*(lim+1));
        for(int i=0;i<np;i++)
        {
            int j=(l/primes[i]+1)*primes[i];
            for(;j<=h;j+=primes[i])
            composite[j-l]=1;
        }
        for(int i=l+1;i<=h;i++)
        if(!composite[i-l]) count++;
    }
    t=omp_get_wtime()-t;
    printf("%f\n",t);
}

double sieve3(int n){
    int lim=sqrt(n);
    int *primes=(int*)malloc(sizeof(int)*(lim+1));
    int np=0;
    int count=0;

    double t=omp_get_wtime();
    #pragma omp parallel num_threads(8) 
    {
        int *composite=(int*)malloc((lim+1)*sizeof(int));
        memset(composite,0,sizeof(int)*(lim+1));
        #pragma omp single
        for(int i=2;i<=lim;i++)
                if(!composite[i]){
                    primes[np++]=i;
                    count++;
                    for(int j=i*i;j<=lim;j+=i)
                        composite[j]=1;
                }
        #pragma omp for reduction(+:count)
        for(int l=lim;l<n;l+=lim){
            int h=l+lim;
            if(h>n) h=n;
            memset(composite,0,sizeof(int)*(lim+1));
            for(int j=0;j<np;j++){
                int k=(l/primes[j]+1)*primes[j];
                for(;k<=h;k+=primes[j])
                    composite[k-l]=1;
            }
            for(int i=l+1;i<=h;i++)
                if(!composite[i-l])
                    count++;
        }
        free(composite);

    }
    t=omp_get_wtime()-t;
    printf("%f\n",t);
    free(primes);
}
void main(){
    sieve1(200000);
    sieve2(200000);
    sieve3(200000);
}