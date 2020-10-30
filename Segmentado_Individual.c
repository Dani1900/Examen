#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#define NUM_THREADS 350

int main()
{
    FILE *image, *outputImagebw, *outputImagein, *outputImagebl,*lecturas;
    //unsigned char* imgIni = (unsigned char*)malloc(226241*3*sizeof(unsigned char));
    //unsigned char* imgRes = (unsigned char*)malloc(226241*3*sizeof(unsigned char));
    image = fopen("f1.bmp","rb");          //Imagen original a transformar
    outputImagebw = fopen("f1_BWSeg.bmp","wb");    //Imagen transformada
    outputImagein = fopen("f1_INSeg.bmp","wb");    //Imagen transformada
    outputImagebl = fopen("f1_BLSeg.bmp","wb");
    long ancho,anchoTotal;
    int ancho2, extra=0;
    long alto;
    long dimension;
    int ventana=21;
    double mascara=ventana*ventana;
    unsigned char suma;
    int dif=0;
    unsigned char r, g, b,promedio;               //Pixel
    //int j =0;
    double tiempo, t2, t1;
    dif=ventana/2;
    
   
    
    unsigned char xx[54];
    for(int i=0; i<54; i++) {
      xx[i] = fgetc(image);
      fputc(xx[i], outputImagebw);
      fputc(xx[i], outputImagein);
      fputc(xx[i], outputImagebl);
        //Copia cabecera a nueva imagen
    }
    ancho =(long)xx[21]*16777216+(long)xx[20]*65536+(long)xx[19]*256+(long)xx[18];
    alto =(long)xx[25]*16777216+(long)xx[24]*65536+(long)xx[23]*256+(long)xx[22];
    
    

    ancho2=(int)ancho*3;
     
    if (ancho2%4!=0){
        ancho2=(ancho2)/4;
        ancho2=((ancho2)*4)+4;
        extra=ancho2-ancho*3;
    }
    anchoTotal=(ancho*3)+extra;
      
    dimension= (alto*anchoTotal)+1;
    unsigned char* arr_in = (unsigned char*)malloc((dimension)*sizeof(unsigned char));
    unsigned char* arr_inv = (unsigned char*)malloc((dimension)*sizeof(unsigned char));
    unsigned char* arr_bl = (unsigned char*)malloc((dimension)*sizeof(unsigned char));
    
 
    int j= 0;
    while(!feof(image)){
    *(arr_in + j) = fgetc(image);

       j++;
     }
 
//
  int count = 0;
//
    t1 = omp_get_wtime();
   #pragma omp parallel
   
   {
    #pragma omp sections
       {
       #pragma omp section
       for (int i =0; i<((dimension-1)/4); i+=3){
           b = *(arr_in + i);
           g = *(arr_in + i + 1);
           r = *(arr_in + i + 2);
           unsigned char pixel = 0.21*r+0.72*g+0.07*b;
           *(arr_in + i) = pixel;
           *(arr_in + i + 1) = pixel;
           *(arr_in + i + 2) = pixel;
           count += 3;
           if(count == anchoTotal){
               i+=2;
               count = 0;
           }
       }
           #pragma omp section
           for (int i =i<((dimension-1)/4); i<(2*((dimension-1)/4)); i+=3){
               b = *(arr_in + i);
               g = *(arr_in + i + 1);
               r = *(arr_in + i + 2);
               unsigned char pixel = 0.21*r+0.72*g+0.07*b;
               *(arr_in + i) = pixel;
               *(arr_in + i + 1) = pixel;
               *(arr_in + i + 2) = pixel;
               count += 3;
               if(count == anchoTotal){
                   i+=2;
                   count = 0;
               }
           }
           
           
           #pragma omp section
           for (int i =(2*((dimension-1)/4)); i<(3*((dimension-1)/4)); i+=3){
               b = *(arr_in + i);
               g = *(arr_in + i + 1);
               r = *(arr_in + i + 2);
               unsigned char pixel = 0.21*r+0.72*g+0.07*b;
               *(arr_in + i) = pixel;
               *(arr_in + i + 1) = pixel;
               *(arr_in + i + 2) = pixel;
               count += 3;
               if(count == anchoTotal){
                   i+=2;
                   count = 0;
               }
           }
           
           #pragma omp section
           for (int i =(3*((dimension-1)/4)); i<(dimension-1); i+=3){
               b = *(arr_in + i);
               g = *(arr_in + i + 1);
               r = *(arr_in + i + 2);
               unsigned char pixel = 0.21*r+0.72*g+0.07*b;
               *(arr_in + i) = pixel;
               *(arr_in + i + 1) = pixel;
               *(arr_in + i + 2) = pixel;
               count += 3;
               if(count == anchoTotal){
                   i+=2;
                   count = 0;
               }
           }
                      
       }
       
       int j= 0;
       while(!feof(image)){
       *(arr_inv + j) = *(arr_in + j);
       *(arr_bl + j) = *(arr_in + j);
          j++;
        }
       
       #pragma omp sections
       
       {
        #pragma omp section
       for(int i = 0; i < (alto/4); i++){
           for(int j = 0; j < (anchoTotal); j+= 3){
               *(arr_inv + (i*(anchoTotal))+j) = *(arr_in + (i*(anchoTotal))+ (anchoTotal) - j);
               *(arr_inv + (i*(anchoTotal))+j + 1) = *(arr_in + (i*(anchoTotal))+ (anchoTotal )- j - 1);
               *(arr_inv + (i*(anchoTotal))+j + 2) = *(arr_in + (i*(anchoTotal))+ (anchoTotal) - j - 2);
           }
       }
        #pragma omp section
        for(int i = (alto/4); i < (2*(alto/4)); i++){
            for(int j = 0; j < (anchoTotal); j+= 3){
                *(arr_inv + (i*(anchoTotal))+j) = *(arr_in + (i*(anchoTotal))+ (anchoTotal) - j);
                *(arr_inv + (i*(anchoTotal))+j + 1) = *(arr_in + (i*(anchoTotal))+ (anchoTotal)- j - 1);
                *(arr_inv + (i*(anchoTotal))+j + 2) = *(arr_in + (i*(anchoTotal))+ (anchoTotal) - j - 2);
                     }
                 }
        #pragma omp section
        for(int i = (2*(alto/4)); i < (3*(alto/4)); i++){
            for(int j = 0; j < (anchoTotal); j+= 3){
                *(arr_inv + (i*(anchoTotal))+j) = *(arr_in + (i*(anchoTotal))+ (anchoTotal) - j);
                *(arr_inv + (i*(anchoTotal))+j + 1) = *(arr_in + (i*(anchoTotal))+ (anchoTotal )- j - 1);
                *(arr_inv + (i*(anchoTotal))+j + 2) = *(arr_in + (i*(anchoTotal))+ (anchoTotal) - j - 2);
                   }
               }
        #pragma omp section
        for(int i = (3*(alto/4)); i < alto; i++){
            for(int j = 0; j < (anchoTotal); j+= 3){
                *(arr_inv + (i*(anchoTotal))+j) = *(arr_in + (i*(anchoTotal))+ (anchoTotal) - j);
                *(arr_inv + (i*(anchoTotal))+j + 1) = *(arr_in + (i*(anchoTotal))+ (anchoTotal) - j - 1);
                *(arr_inv + (i*(anchoTotal))+j + 2) = *(arr_in + (i*(anchoTotal))+ (anchoTotal) - j - 2);
                    }
                }
       }
       
       #pragma omp sections
       {
        #pragma omp section
        for(int i = dif; i < ((alto/4)); i++){
            for(int j = (dif*3); j < ((anchoTotal)-(dif*3)-3); j+= 3){
                    promedio=0;
                    for(int x = (i-dif); x < (i+dif+1); x++){  //alto
                    for(int y = (j-(dif*3)); y < (j+(dif*3)+3); y+= 3){   //ancho
                    
                       promedio+=( *(arr_inv+(x*(anchoTotal))+y))/9;
                        
                    }
                    }
                    
                    *(arr_bl + (i*(anchoTotal))+j) = promedio;          //b
                    *(arr_bl + (i*(anchoTotal))+j + 1) = promedio;  //g
                    *(arr_bl + (i*(anchoTotal))+j + 2) = promedio; //r
                }
            }
        #pragma omp section
        for(int i = ((alto/4)); i < ((2*(alto/4))); i++){
            for(int j = (dif*3); j < ((anchoTotal)-(dif*3)-3); j+= 3){
                    promedio=0;
                    for(int x = (i-dif); x < (i+dif+1); x++){  //alto
                    for(int y = (j-(dif*3)); y < (j+(dif*3)+3); y+= 3){   //ancho
                       
                        promedio+=( *(arr_inv+(x*(anchoTotal))+y))/9;
                           
                    }
                    }
                       
                    *(arr_bl + (i*(anchoTotal))+j) = promedio;          //b
                    *(arr_bl + (i*(anchoTotal))+j + 1) = promedio;  //g
                    *(arr_bl + (i*(anchoTotal))+j + 2) = promedio; //r
                }
            }
        #pragma omp section
        for(int i = ((2*(alto/4))); i < ((3*(alto/4))); i++){
            for(int j = (dif*3); j < ((anchoTotal)-(dif*3)-3); j+= 3){
                    promedio=0;
                    for(int x = (i-dif); x < (i+dif+1); x++){  //alto
                    for(int y = (j-(dif*3)); y < (j+(dif*3)+3); y+= 3){   //ancho
                            
                        promedio+=( *(arr_inv+(x*(anchoTotal))+y))/9;
                                  
                    }
                    }
                              
                    *(arr_bl + (i*(anchoTotal))+j) = promedio;          //b
                    *(arr_bl + (i*(anchoTotal))+j + 1) = promedio;  //g
                    *(arr_bl + (i*(anchoTotal))+j + 2) = promedio; //r
                }
            }
           
           #pragma omp section
                  for(int i = ((3*(alto/4))); i < (alto-dif-1); i++){
                      for(int j = (dif*3); j < ((anchoTotal)-(dif*3)-3); j+= 3){
                              promedio=0;
                              for(int x = (i-dif); x < (i+dif+1); x++){  //alto
                              for(int y = (j-(dif*3)); y < (j+(dif*3)+3); y+= 3){   //ancho
                              
                                 promedio+=( *(arr_inv+(x*(anchoTotal))+y))/9;
                                  
                              }
                              }
                              
                              *(arr_bl + (i*(anchoTotal))+j) = promedio;          //b
                              *(arr_bl + (i*(anchoTotal))+j + 1) = promedio;  //g
                              *(arr_bl + (i*(anchoTotal))+j + 2) = promedio; //r
                          }
                      }
       }
   }
    

    for(int i = 0; i < (dimension-1); i++){
        fputc(*(arr_in + i), outputImagebw);
        fputc(*(arr_inv + i), outputImagein);
        fputc(*(arr_bl + i), outputImagebl);
 
    }
     t2 = omp_get_wtime();
    tiempo = t2 -t1;
         printf("tiempo = (%lf)\n",tiempo);

    free(arr_in);
    free(arr_inv);
    free(arr_bl);
    fclose(image);
    fclose(outputImagebw);
    fclose(outputImagein);
    fclose(outputImagebl);
    return 0;
}
