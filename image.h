#ifndef IMAGE_H
#define IMAGE_H

#include <stdint.h>

#pragma pack(push, 1)

typedef struct {
    uint16_t id;
    uint32_t fileSize;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offset;
    uint32_t headerSize;
    int32_t width;
    int32_t height;
    int16_t planes;
    int16_t bitsPerPixel;
    uint32_t compression;
    uint32_t imageSize;
    int32_t xPixelsPerMeter;
    int32_t yPixelPerMeter;
    uint32_t colors;
    uint32_t importantColors;
} BMPHeader;

#pragma pack(pop)

typedef struct {
    unsigned int width;
    unsigned int height;
    unsigned int bytesInLine;
    unsigned char *image;
} ImageInfo;

void freeImage(ImageInfo *imageInfo);

ImageInfo *readBMP(const char *filename);

int saveBMP(const char *filename, ImageInfo *imageInfo);

#endif // IMAGE_H