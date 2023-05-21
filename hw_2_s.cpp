#include "hw_2_h.h"
#include <iostream>
#include <cstring>
Img::Img() {
    srcImg=new int[128];
    width=64;
    height=64;
    x_c=64;
    y_c=64;
}
Img::Img(int w, int h) {
    srcImg=new int [w*h];
    height=h;
    width=w;
    y_c=h/2;
    x_c=w/2;
}
Img::Img(const int *src, int w, int h) {
    this->srcImg=new int[w*h];
    width=w;
    height=h;
    x_c=w/2;
    y_c=h/2;
    for (int i = 0; i <w*h ; ++i) {
        this->srcImg[i]=src[i];
    }
}
Img::~Img(){
    delete[] srcImg;
    srcImg= nullptr;
}
ImageProcess::ImageProcess() {
    srcImg=new Img;
    processedImg=new Img;
    mask=new Img(3,3);
    for (int i = 0; i < mask->width*mask->width; ++i) {
        mask->srcImg[i]=0;
        if(i==1||i==3||i==4||i==5||i==7){
            mask->srcImg[i]=1;
        }
    }
}
ImageProcess::ImageProcess(int w, int h) {
    srcImg=new Img(w,h);
    processedImg=new Img(w,h);
    mask=new Img(3,3);
    for (int i = 0; i < mask->width*mask->width; ++i) {
        mask->srcImg[i]=0;
        if(i==1||i==3||i==4||i==5||i==7){
            mask->srcImg[i]=1;
        }
    }
}
ImageProcess::ImageProcess(const char *fileName) {
    FILE* flog=fopen(fileName,"r");
    int ver;
    int ger;
    fscanf(flog,"%d\t%d",&ver,&ger);
    srcImg=new Img(ver,ger);
    processedImg=new Img(ver,ger);
    char* buffer = new char[ver*ger];
    char buf;
    int k=0;
    while (fscanf(flog,"%c",&buf)!=EOF) {
        if(buf!='\n'&& buf!='\t'){
            buffer[k]=buf;
            k++;
        }
    }
    buffer[ver*ger]='\0';
    for (int i = 0; i < srcImg->width*srcImg->height; ++i) {
        srcImg->srcImg[i]=buffer[i]-'0';
    }
    fclose(flog);
    mask= new Img(3,3);
    for (int i = 0; i < mask->width*mask->width; ++i) {
        mask->srcImg[i]=0;
        if(i==1||i==3||i==4||i==5||i==7){
            mask->srcImg[i]=1;
        }
    }
}
ImageProcess::ImageProcess(const Img *img) {
    srcImg=new Img(img->width,img->height);
    processedImg=new Img(img->width,img->height);
    mask= new Img(3,3);
    for (int i = 0; i < mask->width*mask->width; ++i) {
        mask->srcImg[i]=0;
        if(i==1||i==3||i==4||i==5||i==7){
            mask->srcImg[i]=1;
        }

    for (int i = 0; i < img->width*img->height; ++i) {
        srcImg->srcImg[i]=img->srcImg[i];

        }
    }
}
ImageProcess::~ImageProcess() {
    delete srcImg;
    delete processedImg;
    delete mask;
    mask= nullptr;
    srcImg= nullptr;
    processedImg= nullptr;

}

int ImageProcess::updateMask(const Img &mask) {
    this->mask->srcImg=new int[mask.width*mask.height];
    this->mask->width=mask.width;
    this->mask->height=mask.height;
    for (int i = 0; i < mask.width*mask.height; ++i) {
        this->mask->srcImg[i]=mask.srcImg[i];
    }
    return 0;
}

int ImageProcess::updateSrcImg() {
    Img* buf;
    buf=this->srcImg;
    this->srcImg=this->processedImg;
    this->processedImg=buf;
    return 0;
}

int ImageProcess::dilatation(int srcImg) {
    int* buffer=new int[this->srcImg->height*this->srcImg->height];
    for (int i = 0; i <this->srcImg->height*this->srcImg->height ; ++i) {
        buffer[i]=this->srcImg->srcImg[i];
    }
    int buf_xc=this->srcImg->x_c;
    int buf_yc=this->srcImg->y_c;
    int fir_buf=this->srcImg->width;
    int sec_buf=this->srcImg->height;
    delete this->srcImg;
    this->srcImg=new Img(fir_buf+2*buf_xc,sec_buf+2*buf_yc);
    int g=0;
    for (int i = 0,k=0; k < this->srcImg->height; i+=this->srcImg->width,k++) {
        for (int j = i,l=0; l <this->srcImg->height ; ++j,l++) {
            if(j==this->srcImg->height*this->srcImg->width){
                break;
            }
            if(k<buf_yc||k>sec_buf||l<buf_xc||l>fir_buf){
                this->srcImg->srcImg[j]=0;
            }else{
                this->srcImg->srcImg[j]=buffer[g];
                g++;
            }
        }
    }
    for (int i = 0; i < this->srcImg->height*this->srcImg->width; i+=this->srcImg->width) {
        for (int j = i,f=0; f<this->srcImg->width; ++j,f++) {
            if(mask->srcImg[mask->width*(mask->y_c+1)-mask->x_c]== this->srcImg->srcImg[j]){
                if(this->srcImg->width&1) {
                    for (int l = 0, y = j - mask->y_c * mask->width - this->srcImg->width - mask->width;
                         l <= mask->width; ++l) {
                        int k=0;
                        for (int x = y; k < mask->width; ++k,x++) {
                            if (mask->srcImg[k] != this->srcImg->srcImg[x]) {
                                buffer[x-6*buf_xc+k++]=1;
                            }
                        }
                        y += this->srcImg->width - mask->width;
                    }
                }
                else{
                    for (int l = 0, y = j - mask->y_c * mask->width - 2 * (this->srcImg->width - mask->width);
                         l < mask->width; ++l) {
                        for (int k = 0, x = y; k < mask->width; ++k) {
                            if (mask->srcImg[k] != this->srcImg->srcImg[x]) {
                                buffer[x]=1;
                            }
                        }
                        y += this->srcImg->width - mask->width;
                    }
                }

            }

        }
    }
    this->srcImg= new Img(fir_buf,sec_buf);
    for (int i = 0; i <this->srcImg->height*this->srcImg->width; ++i) {
        this->srcImg->srcImg[i]=buffer[i];
        std::cout<<buffer[i]<<" ";
    }
    FILE* flog = fopen("result.txt", "w");
    for (int i = 0,k=0; k < this->srcImg->height; i+=this->srcImg->width,k++) {
        for (int j = i,l=0; l <this->srcImg->width ; ++j,l++) {
                fprintf(flog,"%d",this->srcImg->srcImg[j]);
        }
        fprintf(flog,"\n");

    }
    fclose(flog);
    return 0;
}
//
// Created by User on 20.05.2023.
//
