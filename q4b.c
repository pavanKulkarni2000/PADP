#include<stdio.h>
#include<gd.h>
#include<omp.h>

char *input_file;
char *output_file;

double gray(int threads){

    
    FILE *fp;
    if(!(fp=fopen(input_file,"r"))){
        printf("unable to open input file.\n");
        exit(0);
    }
    gdImagePtr ptr=gdImageCreateFromPng(fp);
    
    int w=gdImageSX(ptr);
    int h=gdImageSY(ptr);
    double t=omp_get_wtime();  
    omp_set_num_threads(4*threads);
    #pragma omp parallel for collapse(2) schedule(dynamic,1) firstprivate(ptr)
    for(int x=0;x<w;x++)
        for(int y=0;y<h;y++){
            int color=gdImageGetPixel(ptr,x,y);
            int r=gdImageRed(ptr,color);
            int g=gdImageGreen(ptr,color);
            int b=gdImageBlue(ptr,color);
            int avg=(r+g+b)/3;
            switch (omp_get_thread_num()%4)
            {
            case 0:
                color=gdImageColorAllocate(ptr,0,0,0);
                gdImageSetPixel(ptr,x,y,color);
                break;
            case 1:
                color=gdImageColorAllocate(ptr,avg,0,0);
                gdImageSetPixel(ptr,x,y,color);
                break;
            case 2:
                color=gdImageColorAllocate(ptr,0,avg,0);
                gdImageSetPixel(ptr,x,y,color);
                break;
            case 3:
                color=gdImageColorAllocate(ptr,0,0,avg);
                gdImageSetPixel(ptr,x,y,color);
                break;
            }
        }
    
    if(!(fp=fopen(output_file,"w+"))){
        printf("Unable to open file to write\n");
        exit(0);
    }
    gdImagePng(ptr,fp);
    fclose(fp);
    gdImageDestroy(ptr);
    return omp_get_wtime()-t;
}



int main(int argc,char *argv[]){
    if(argc<3){
        printf("Require input and output file.\n");
        exit(0);
    }else{
        input_file=argv[1];
        output_file=argv[2];
    }
    printf("%10s%10d%10d%10d%10d\n","threads",1,2,4,8);
    printf("%10s","time");
    for(int i=0;i<4;i++)
    printf("%10.6f",gray(1<<i));
    printf("\n");
}