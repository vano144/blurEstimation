#ifndef AVXIMGVER2_H
#define AVXIMGVER2_H

#include <QVector>
#include <imagereaderver2.h>

class avxImgVer2
{
public:
    avxImgVer2(imagereaderVer2 *imgReader);
    ~avxImgVer2();

    void fillHorVector(int x0 , int y0, QVector<unsigned int> &tmp, imagereaderVer2 & imgReader);
    void fillVerVector(int x0 , int y0, QVector<unsigned int> &tmp, imagereaderVer2 & imgReader);

    void fillHorVector(int x0 , int y0, QVector<int> &tmp, imagereaderVer2 & imgReader);
    void fillVerVector(int x0 , int y0, QVector<int> &tmp, imagereaderVer2 & imgReader);

    unsigned int sum(QVector<unsigned int> &vec);
    unsigned int sum(QVector<int> &vec);
    int multSecMinusOneAndSum(QVector<int> &vec);
    void countBvAndBh();
    float countQs();
    void countDandB();
    float avxVecSum(const __m256 & x);
    void countDandBAVx();

    void recalcLimits();

    imagereaderVer2 * imgReader;
    imagereaderVer2 * BhReader;
    imagereaderVer2 * BvReader;

    float * dIvPtr;
    float * dIhPtr;
    float * bIvPtr;
    float * bIhPtr;

    int kSize;
    int rightLimit;
    int leftLimit;
};

#endif // AVXIMGVER2_H
