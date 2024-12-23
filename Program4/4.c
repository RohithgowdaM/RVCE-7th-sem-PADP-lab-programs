#include <stdio.h>
#include <stdlib.h>
#include <gd.h>
#include <omp.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <in.png> <out.png>\n", argv[0]);
        exit(1);
    }

    FILE *ifp = fopen(argv[1], "rb");
    if (!ifp) {
        fprintf(stderr, "Error: Cannot open input file.\n");
        exit(1);
    }

    FILE *ofp = fopen(argv[2], "wb");
    if (!ofp) {
        fprintf(stderr, "Error: Cannot open output file.\n");
        fclose(ifp);
        exit(1);
    }

    double t = omp_get_wtime();

    gdImagePtr img = gdImageCreateFromPng(ifp);
    if (!img) {
        fprintf(stderr, "Error: Cannot create image from input.\n");
        fclose(ifp);
        fclose(ofp);
        exit(1);
    }

    int w = gdImageSX(img);
    int h = gdImageSY(img);

    gdImagePtr outImg = gdImageCreateTrueColor(w, h);

    #pragma omp parallel for collapse(2) // Parallelize both loops
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            int color = gdImageGetPixel(img, x, y);
            int avgColor = (gdImageRed(img, color) + gdImageGreen(img, color) + gdImageBlue(img, color)) / 3;
            gdImageSetPixel(outImg, x, y, gdImageColorAllocate(outImg, avgColor, avgColor, avgColor));
        }
    }

    gdImagePng(outImg, ofp);
    gdImageDestroy(img);
    gdImageDestroy(outImg);

    fclose(ifp);
    fclose(ofp);

    t = omp_get_wtime() - t;
    printf("Time: %f seconds\n", t);

    return 0;
}
