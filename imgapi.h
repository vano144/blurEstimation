#ifndef IMGAPI_H
#define IMGAPI_H

#include <QVector>
#include <imagereader.h>

class imgapi
{
public:
    imgapi(ImageReader * imgReader);
    ~imgapi();

    void fillHorVector(int x0 , int y0, QVector<float> &tmp, ImageReader & imgReader);
    void fillVerVector(int x0 , int y0, QVector<float> &tmp, ImageReader & imgReader);

    float sum(QVector<float> &vec);
    float multSecMinusOneAndSum(QVector<float> &vec);
    void countBvAndBh();
    float countQs();

    ImageReader * imgReader;
    ImageReader * BhReader;
    ImageReader * BvReader;

    int kSize;

};

#endif // IMGAPI_H
