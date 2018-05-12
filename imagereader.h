#ifndef IMAGEREADER_H
#define IMAGEREADER_H

#include <QImage>
#include <immintrin.h>

class ImageReader
{
public:
    ImageReader();
    ImageReader(int width, int height, float *imgPtr);
    float getT(int x, int y);
    float get(int x, int y);
    void show(const int mode);
    void checkTranspose();

    QImage img;
    int width;
    int height;
    float * imgPtr;
    float * imgPtrT;

};

#endif // IMAGEREADER_H
