#include<gd.h>
#include<stdio.h>
#include<omp.h>

int threads[]={1,2,4,8};
const char * input="input.png";
const char * output="output.png";

double gray(int t){
    FILE* inputImg=fopen(input,"r");
    gdImagePtr img=gdImageCreateFromPng(inputImg);
    int W=gdImageSX(img);
    int H=gdImageSY(img);
    double dt=omp_get_wtime();
    #pragma omp parallel for num_threads(8) collapse(2) schedule(guided,(t+1)*5)
    for(int x=0;x<W;x++)
    for(int y=0;y<H;y++)
    {
        int c=gdImageGetPixel(img,x,y);
        int r=gdImageRed(img,c);
        int g=gdImageGreen(img,c);
        int b=gdImageBlue(img,c);
        c=(r+g+b)/3;
        c=gdImageColorAllocate(img,c,c,c);
        gdImageSetPixel(img,x,y,c);
    }
    dt=omp_get_wtime()-dt;
    FILE* outputImg=fopen(output,"w+");
    gdImagePng(img,outputImg);
    fclose(inputImg);
    fclose(outputImg);
    gdImageDestroy(img);
    return dt;
}

void main(){

    printf("%15s","chunk size:");
    for(int i=0;i<4;i++)
    printf("%10d",(i+1)*5);
    printf("\n");

    printf("%15s","time:");
    for(int i=0;i<4;i++)
    printf("%10f",gray(i));
    printf("\n");
}