#include <iostream>
#include <cstdlib>
#include <gd.h>
#include <omp.h>

using namespace std;

int main(int argc, char *argv[]) {
    if (argc != 3) {
        cout << "Usage: " << argv[0] << " <in.png> <out.png>" << endl;
        exit(1);
    }

    FILE *ifp = fopen(argv[1], "rb");
    if (!ifp) {
        cerr << "Error: Cannot open input file." << endl;
        exit(1);
    }

    FILE *ofp = fopen(argv[2], "wb");
    if (!ofp) {
        cerr << "Error: Cannot open output file." << endl;
        fclose(ifp);
        exit(1);
    }

    double t = omp_get_wtime();

    gdImagePtr img = gdImageCreateFromPng(ifp);
    if (!img) {
        cerr << "Error: Cannot create image from input." << endl;
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
    cout << "Time: " << t << " seconds" << endl;

    return 0;
}
