#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<omp.h>
#include<ctype.h>

char *searchWords[12]={"the","can","put","look","mark","illuminate","carry","high","tower","missy","character","man"};
char *files[4]={"10kb.txt","500kb.txt","1mb.txt","2mb.txt"};
int threads[4]={1,2,4,8};

void readWord(FILE* file,char buf[],int *n){
    char c;
    while(!feof(file)&&!isalpha((c=fgetc(file))));
    (*n)=0;
    while(!feof(file)&&!isalpha(c)){
        buf[(*n)++]=c;
        c=fgetc(file);
    }
    buf[(*n)]='\0';
}

double searchAll(char *fileName, int thread){
    int frequency[12];
    double t=omp_get_wtime();
    #pragma omp parallel for num_threads(thread)
    for(int i=0;i<12;i++){
        frequency[i]=0;
        FILE *file=fopen(fileName,"r");
        char buf[100];
        int n;
        while(!feof(file)){
            readWord(file,buf,&n);
            if(strcmp(searchWords[i],buf)==0)
                frequency[i]++;
        }
    }
    t=omp_get_wtime()-t;
    return t;
}

void main(){
    printf("%10s","Threads:");
    for(int i=0;i<4;i++)
    printf("%10d",threads[i]);
    printf("\n");
    for(int j=0;j<4;j++){
        printf("%10s",files[j]);
        for(int i=0;i<4;i++)
            printf("%10f",searchAll(files[j],threads[i]));
        printf("\n");
    }
}