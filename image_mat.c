// Image matrix
// Designer: Deng LiWei
// Date: 2021/03
// Description: A matrix object for store image raw data whatever the format
#include <stdint.h>
#include <stdlib.h>
#include <memory.h>
#include <malloc.h>

#include "image_mat.h"

ImageMat* NewImageMat(int width, int height)
{
    ImageMat* mat = (ImageMat *)malloc(sizeof(ImageMat));

    mat->width = width;
    mat->height = height;

    mat->pData = (uint8_t*)malloc(sizeof(uint32_t) * width * height);

    return mat;
}

void DestoryImageMat(ImageMat* mat)
{
    if (mat != NULL)
    {
        if (mat->pData != NULL)
        {
            free(mat->pData);
        }

        free(mat);
    }
}

uint32_t GetPixel(ImageMat* mat, int x, int y)
{
    if ((mat != NULL) && (mat->pData != NULL))
    {
        if ((x < mat->width) && (y < mat->height) && (x >= 0) && (y >= 0))
        {
            return *(uint32_t*)(mat->pData + (y * mat->width + x) * sizeof(uint32_t));
        }
    }

    return 0x000000000;
}

void SetPixel(ImageMat* mat, int x, int y, uint32_t color)
{
    if ((mat != NULL) && (mat->pData != NULL))
    {
        if ((x < mat->width) && (y < mat->height) && (x >= 0) && (y >= 0))
        {
            *(uint32_t*)(mat->pData + (y * mat->width + x) * sizeof(uint32_t)) = color;
        }
    }
}

void ImageMatRGBtoYUV(ImageMat* mat)
{
    for (int py = 0; py < mat->height; py++)
    {
        for (int px = 0; px < mat->width; px++)
        {
            uint32_t color = *(uint32_t*)(mat->pData + (py * mat->width + px) * sizeof(uint32_t));
            int r = (color & 0x000000FF) >> 0;
            int g = (color & 0x0000FF00) >> 8;
            int b = (color & 0x00FF0000) >> 16;
            uint8_t a = (color & 0xFF000000) >> 24;
            
            uint8_t y = 0.299 * r + 0.587 * g + 0.114 * b;
            uint8_t u = -0.169 * r - 0.331 * g + 0.500 * b + 128;
            uint8_t v = 0.500 * r - 0.419 * g - 0.081 * b + 128;

            color = (a << 24) | (y << 16) | (u << 8) | v;
            *(uint32_t*)(mat->pData + (py * mat->width + px) * sizeof(uint32_t)) = color;
        }
    }
}

void ImageMatYUVtoRGB(ImageMat* mat)
{
    for (int py = 0; py < mat->height; py++)
    {
        for (int px = 0; px < mat->width; px++)
        {
            uint32_t color = *(uint32_t*)(mat->pData + (py * mat->width + px) * sizeof(uint32_t));
            int v = (color & 0x000000FF) >> 0;
            int u = (color & 0x0000FF00) >> 8;
            int y = (color & 0x00FF0000) >> 16;
            uint8_t a = (color & 0xFF000000) >> 24;
            
            int sr = y + 1.403 * (v - 128);
            int sg = y - 0.343 * (u - 128) - 0.714 * (v - 128);
            int sb = y + 1.770 * (u - 128);

            uint8_t r = sr < 0 ? 0 : (sr > 255 ? 255 : sr);
            uint8_t g = sg < 0 ? 0 : (sg > 255 ? 255 : sg);
            uint8_t b = sb < 0 ? 0 : (sb > 255 ? 255 : sb);

            color = (a << 24) | (b << 16) | (g << 8) | r;
            *(uint32_t*)(mat->pData + (py * mat->width + px) * sizeof(uint32_t)) = color;
        }
    }
}

void ImageMatRGBtoGray(ImageMat* mat)
{
    for (int y = 0; y < mat->height; y++)
    {
        for (int x = 0; x < mat->width; x++)
        {
            uint32_t color = *(uint32_t*)(mat->pData + (y * mat->width + x) * sizeof(uint32_t));
            uint8_t r = (color & 0x000000FF) >> 0;
            uint8_t g = (color & 0x0000FF00) >> 8;
            uint8_t b = (color & 0x00FF0000) >> 16;

            color = (r * 76 + g * 150 + b * 30) >> 8;
            *(uint32_t*)(mat->pData + (y * mat->width + x) * sizeof(uint32_t)) = color;
        }
    }
}

void ImageMatGraytoFullGray(ImageMat* mat)
{
    for (int y = 0; y < mat->height; y++)
    {
        for (int x = 0; x < mat->width; x++)
        {
            uint32_t color = *(uint32_t*)(mat->pData + (y * mat->width + x) * sizeof(uint32_t));
            uint8_t gray = color;

            color = (gray << 24) | (gray << 16) | (gray << 8) | gray;
            *(uint32_t*)(mat->pData + (y * mat->width + x) * sizeof(uint32_t)) = color;
        }
    }
}

uint32_t CopyMat(ImageMat* dest, ImageMat* src)
{
    if ((dest->height != src->height) || (dest->width != src->width))
    {
        return 0;
    }

    memcpy(dest->pData, src->pData, src->height * src->width * sizeof(uint32_t));

    return dest->height * dest->width;
}
