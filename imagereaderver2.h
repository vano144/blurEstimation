#ifndef IMAGEREADERVER2_H
#define IMAGEREADERVER2_H

#include <QImage>

class imagereaderVer2
{
public:
    imagereaderVer2();
    imagereaderVer2(int width, int height, unsigned int *imgPtr);
    unsigned int getT(int x, int y);
    unsigned int get(int x, int y);
    void show(const int mode);
    void checkTranspose();

    QImage img;
    int width;
    int height;
    unsigned int * imgPtr;
    unsigned int * imgPtrT;
};

#endif // IMAGEREADERVER2_H

#include <QImage>
#include <immintrin.h>
