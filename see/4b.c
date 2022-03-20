#include<stdio.h>
#include<omp.h>
#include<gd.h>
char * input="input.png";
char *output="output.png";
void gray(){
    FILE* inputFile=fopen(input,"r");
    gdImagePtr img=gdImageCreateFromPng(inputFile);
    int W=gdImageSX(img);
    int H=gdImageSY(img);
    #pragma omp parallel for collapse(2) schedule(static,1) num_threads(8)
    for(int x=0;x<W;x++)
    for(int y=0;y<H;y++)
    {
        int c=gdImageGetPixel(img,x,y);
        int avg=(gdImageRed(img,c)+gdImageGreen(img,c)+gdImageBlue(img,c))/3;
        switch(omp_get_thread_num()%4){
            case 0:c=gdImageColorAllocate(img,avg,avg,avg);break;
            case 1:c=gdImageColorAllocate(img,avg,0,0);break;
            case 2:c=gdImageColorAllocate(img,0,avg,0);break;
            case 3:c=gdImageColorAllocate(img,0,0,avg);break;
        }
        gdImageSetPixel(img,x,y,c);
    }
    FILE *outputFile =fopen(output,"w+");
    gdImagePng(img,outputFile);
    fclose(outputFile);
    gdImageDestroy(img);
}

void main(){
    gray();
}