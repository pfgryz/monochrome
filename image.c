#include "image.h"
#include <stdio.h>
#include <stdlib.h>

void freeImage(ImageInfo *imageInfo) {
    if (imageInfo == NULL)
        return;

    if (imageInfo->image != NULL)
        free(imageInfo->image);

    free(imageInfo);
}

ImageInfo *readBMP(const char *filename) {
    FILE *file = fopen(filename, "rb");

    if (file == NULL) {
        printf("Cannot open input file\n");
        return NULL;
    }

    BMPHeader bmpHeader;
    if (fread((void *) &bmpHeader, sizeof(bmpHeader), 1, file) != 1) {
        printf("Cannot read bitmap header\n");
        fclose(file);
        return NULL;
    }

    if (bmpHeader.id != 0x4d42) {
        printf("Invalid file type.\n");
        fclose(file);
        return NULL;
    }

    if (bmpHeader.planes != 1 || bmpHeader.bitsPerPixel != 24 || bmpHeader.compression != 0) {
        printf("Invalid bitmap type. Required 24-bit bitmap without compression\n");
        fclose(file);
        return NULL;
    }

    ImageInfo *imageInfo = (ImageInfo *) malloc(sizeof(ImageInfo));
    if (imageInfo == NULL) {
        printf("Cannot allocate memory\n");
        fclose(file);
        return NULL;
    }

    imageInfo->image = NULL;
    imageInfo->height = abs(bmpHeader.height);
    imageInfo->width = bmpHeader.width;
    imageInfo->bytesInLine = imageInfo->width * 3;
    if (imageInfo->bytesInLine % 4 != 0)
        imageInfo->bytesInLine += 4 - (imageInfo->bytesInLine % 4);

    if (fseek(file, bmpHeader.offset, SEEK_SET) != 0) {
        printf("Cannot operate on file\n");
        fclose(file);
        freeImage(imageInfo);
        return NULL;
    }

    imageInfo->image = (unsigned char *) malloc(bmpHeader.imageSize);
    if (imageInfo->image == 0) {
        printf("Cannot allocate memory\n");
        fclose(file);
        freeImage(imageInfo);
        return NULL;
    }
    if (fread(imageInfo->image, 1, bmpHeader.imageSize, file) != bmpHeader.imageSize) {
        printf("Cannot read file\n");
        fclose(file);
        freeImage(imageInfo);
        return NULL;
    }

    fclose(file);
    return imageInfo;
}

int saveBMP(const char *filename, ImageInfo *imageInfo) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL)
        return 1;

    unsigned int lineBytes = (int) imageInfo->bytesInLine;
    unsigned int imageSize = lineBytes * imageInfo->height;

    BMPHeader bmpHeader = {
            0x4D42,
            sizeof(bmpHeader) + imageSize,
            0, 0,
            sizeof(bmpHeader),
            40,
            (int32_t) imageInfo->width,
            (int32_t) imageInfo->height,
            1,
            24,
            0,
            (uint32_t) imageSize,
            11811,
            11811,
            0,
            0
    };

    if (fwrite(&bmpHeader, sizeof(bmpHeader), 1, file) != 1) {
        fclose(file);
        return 2;
    }

    if (fwrite(imageInfo->image, 1, bmpHeader.imageSize, file) != bmpHeader.imageSize) {
        fclose(file);
        return 3;
    }

    fclose(file);
    return 0;
}