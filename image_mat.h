// Image matrix
// Designer: Deng LiWei
// Date: 2021/03
// Description: A matrix object for store image raw data whatever the format
#pragma once
#include <stdint.h>

typedef struct 
{
    int width;
    int height;

    uint8_t *pData;
}ImageMat;

ImageMat* NewImageMat(int width, int height);
void DestoryImageMat(ImageMat* mat);

uint32_t GetPixel(ImageMat* mat, int x, int y);
void SetPixel(ImageMat* mat, int x, int y, uint32_t color);

void ImageMatRGBtoYUV(ImageMat* mat);
void ImageMatYUVtoRGB(ImageMat* mat);
void ImageMatRGBtoGray(ImageMat* mat);

void ImageMatGraytoFullGray(ImageMat* mat);

uint32_t CopyMat(ImageMat* dest, ImageMat* src);
