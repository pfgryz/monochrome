#include "image.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern unsigned int monochrome(ImageInfo *imageInfo,
                           unsigned int x1,
                           unsigned int y1,
                           unsigned int x2,
                           unsigned int y2,
                           unsigned int threshold);

int main(int argc, char **argv) {
    // Print help information
    if (argc == 2 && !strcmp(argv[1], "--help")) {
        printf("Monochrome Program\n"
               "\tConverts the given rectangle to a monochrome image and saves to file\n"
               "> Usage:\n"
               "\t<inputFilename> <outputFilename> <x1> <y1> <x2> <y2> <threshold>\n"
               "> Requirements:\n"
               "\t(x1, y1, y1, y2, threshold) are integer arguments\n"
               "\tx1 < x2 < imageWidth & y1 < y2 < imageHeight\n"
               "\t0 < threshold < 255\n");
        return 0;
    }

    if (argc != 8) {
        printf("Invalid number of arguments. Use /monochrome --help/ to get more information\n");
        return 0;
    }

    char *tail;
    char *inputFilename = argv[1];
    char *outputFilename = argv[2];
    unsigned int other[5];

    // Load arguments (x1, y1, x2, y2, threshold)
    for (int i = 0; i < 5; i++)
    {
        long result = strtol(argv[3 + i], &tail, 10);
        if (*tail != '\0' || result < 0)
        {
            printf("Invalid type of arguments. Use /monochrome --help/ to get more information\n");
            return 0;
        }
        other[i] = result;
    }

    // x1 >= x2 || y1 >= y2 (check --help)
    if (other[0] >= other[2] || other[1] >= other[3]) {
        printf("Invalid points. Use /monochrome --help/ to get more information\n");
        return 0;
    }

    // threshold > 255
    if (other[4] > 255) {
        printf("Invalid threshold. Use /monochrome --help/ to get more information\n");
        return 0;
    }

    // read bitmap
    ImageInfo *imageInfo = readBMP(inputFilename);
    if (imageInfo == NULL) {
        printf("Program terminated\n");
        return 0;
    }

    // x2 > width || y2 > height
    if (other[2] > imageInfo->width || other[3] > imageInfo->height) {
        printf("Points outside of the image. Use /monochrome --help/ to get more information\n");
        return 0;
    }

    // monochrome
    unsigned int result = monochrome(imageInfo, other[0], other[1], other[2], other[3], other[4]);

    // save bitmap
    unsigned int error = saveBMP(outputFilename, imageInfo);
    if (error == 1) {
        printf("Cannot open a file for output bitmap\n");
        return 0;
    } else if (error == 2 || error == 3) {
        printf("Cannot save bitmap to output file\n");
        return 0;
    }

    return 0;
}