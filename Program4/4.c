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

    // Different scheduling techniques to be tested: static, dynamic, guided, default
    #pragma omp parallel for schedule(dynamic, 100) collapse(2) // You can try other schedules like static, guided
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            int color = gdImageGetPixel(img, x, y);
            int avgColor = (gdImageRed(img, color) + gdImageGreen(img, color) + gdImageBlue(img, color)) / 3;

            // Here, we assign a unique color for each thread.
            int threadId = omp_get_thread_num();
            int threadColor = (threadId * 10) % 256; // Use thread ID for unique color

            // Set the pixel to the grayscale value (black and white) and assign a thread-specific color pattern
            gdImageSetPixel(outImg, x, y, gdImageColorAllocate(outImg, avgColor + threadColor, avgColor + threadColor, avgColor + threadColor));
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
