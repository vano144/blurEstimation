#ifndef AVXIMGAPI_H
#define AVXIMGAPI_H

#include <QVector>
#include <imagereader.h>

class avximgapi
{
public:
    avximgapi(ImageReader * imgReader);
    ~avximgapi();

    void fillHorVector(int x0 , int y0, QVector<float> &tmp, ImageReader & imgReader);
    void fillVerVector(int x0 , int y0, QVector<float> &tmp, ImageReader & imgReader);

    void fillHorVector(int x0 , int y0, float * tmp, ImageReader & imgReader);
    void fillVerVector(int x0 , int y0, float * tmp, ImageReader & imgReader);

    float sum(QVector<float> &vec);
    float sum(float * vec);
    float multSecMinusOneAndSum(QVector<float> &vec);
    void countBvAndBh();
    float countQs();
    void countBvAndBhCalloc();
    void countDandB();
    float avxVecSum(const __m256 & x);
    void countDandBAVx();
    void countDandBAVxCalloc();

    void recalcLimits();

    ImageReader * imgReader;
    ImageReader * BhReader;
    ImageReader * BvReader;

    float * dIvPtr;
    float * dIhPtr;
    float * bIvPtr;
    float * bIhPtr;

    size_t kSize;
    size_t rightLimit;
    size_t leftLimit;
    size_t isCalloc = 0;
};

#endif // AVXIMGAPI_H
