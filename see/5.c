#include<mpi.h>
#include<stdio.h>

void main(int argc,char **argv){
    MPI_Init(&argc,&argv);
    int np,rank;
    MPI_Comm_size(MPI_COMM_WORLD,&np);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    int size=1024;
    char msg[size];
    MPI_Status st;
    switch(rank){
        case 0:
        for(int i=1;i<np;i++){
            MPI_Recv(msg,size,MPI_CHAR,i,0,MPI_COMM_WORLD,&st);
            printf("rank%d: %s\n",i,msg);
        }
        break;
        
        case 1:
        sprintf(msg,"HELLO");
        MPI_Send(msg,size,MPI_CHAR,0,0,MPI_COMM_WORLD);
        break;

        case 2:
        sprintf(msg,"CSE");
        MPI_Send(msg,size,MPI_CHAR,0,0,MPI_COMM_WORLD);
        break;

        case 3:
        sprintf(msg,"RVCE");
        MPI_Send(msg,size,MPI_CHAR,0,0,MPI_COMM_WORLD);
        break;
    }
    MPI_Finalize();
}