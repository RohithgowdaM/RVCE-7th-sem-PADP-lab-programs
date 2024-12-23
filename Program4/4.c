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

    // Print the image size (width and height)
    printf("Image size: %d x %d pixels\n", w, h);

    gdImagePtr outImg = gdImageCreateTrueColor(w, h);

    // Parallelize the image processing with OpenMP
    #pragma omp parallel for schedule(dynamic, 100) collapse(2)
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            int color = gdImageGetPixel(img, x, y);
            int avgColor = (gdImageRed(img, color) + gdImageGreen(img, color) + gdImageBlue(img, color)) / 3;

            // Assign a unique color for each thread
            int threadId = omp_get_thread_num();
            int threadColor = (threadId * 10) % 256;

            // Set the pixel to the grayscale value (black and white) and apply the thread-specific color
            gdImageSetPixel(outImg, x, y, gdImageColorAllocate(outImg, avgColor + threadColor, avgColor + threadColor, avgColor + threadColor));
        }
    }

    gdImagePng(outImg, ofp);
    gdImageDestroy(img);
    gdImageDestroy(outImg);

    fclose(ifp);
    fclose(ofp);

    t = omp_get_wtime() - t;
    // Print the processing time along with the image size
    printf("Time: %f seconds\n", t);

    return 0;
}
