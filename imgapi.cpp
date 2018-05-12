#include "imgapi.h"
#include "imagereader.h"
#include <math.h>
#include <QDebug>
#include "config.h"

imgapi::imgapi(ImageReader * imgReader) : imgReader(imgReader)
{
    this->imgReader = imgReader;
    this->kSize = Config::kSize;
}

imgapi::~imgapi() {
    delete this->BvReader;
    delete this->BhReader;
    delete this->imgReader;
}

void imgapi::fillHorVector(int x0 , int y0, QVector<float> &tmp, ImageReader & imgReader)
{
    for (int x = x0 - this->kSize/2; x < x0 + this->kSize/2; x++)
    {
        tmp.append(imgReader.get(x, y0));
    }
}

void imgapi::fillVerVector(int x0 , int y0, QVector<float> & tmp, ImageReader & imgReader)
{
    for (int x = x0 - this->kSize/2; x < x0 + this->kSize/2; x++)
    {
        tmp.append(imgReader.getT(x, y0));
    }
}

float imgapi::sum(QVector<float> &vec)
{
    float value = 0;
    for (int i = 0; i < vec.size(); i++)
    {
        value += vec.at(i);
    }
    return value;
}

float imgapi::multSecMinusOneAndSum(QVector<float> &vec)
{
    vec[1] *= -1;
    return this->sum(vec);
}

void imgapi::countBvAndBh()
{
    QVector<float> * tmp = new QVector<float>;
    float * bvPtr = new float [this->imgReader->width * this->imgReader->height];
    float * bhPtr = new float [this->imgReader->width * this->imgReader->height];
    float value = 0;
    for (int y = 0; y < this->imgReader->height; y++)
    {
        for (int x = 0; x < this->imgReader->width; x++)
        {
            this->fillHorVector(x, y, *tmp, *(this->imgReader));
            value = this->sum(*tmp);
            bvPtr[y*imgReader->width + x] = value;
            tmp->clear();

            this->fillVerVector(x, y, *tmp, *(this->imgReader));
            value = this->sum(*tmp);
            bhPtr[x*imgReader->height + y] = value;
            tmp->clear();
        }
    }
    this->BvReader = new ImageReader(imgReader->width, imgReader->height, bvPtr);
    this->BhReader = new ImageReader(imgReader->width, imgReader->height, bhPtr);
    delete tmp;
}

float imgapi::countQs()
{
    this->kSize = 2; // [1 -1]
    QVector<float> * tmp = new QVector<float>;

    float sIv = 0;
    float sIh = 0;
    float sVv = 0;
    float sVh = 0;

    for (int y = 0; y < this->imgReader->height; y++)
    {
        for (int x = 0; x < this->imgReader->width; x++)
        {
            this->fillHorVector(x, y, *tmp, *(this->imgReader));
            float valueIv = fabs(this->multSecMinusOneAndSum(*tmp));
            sIv += valueIv;
            tmp->clear();

            this->fillVerVector(x, y, *tmp, *(this->imgReader));
            float valueIh = fabs(this->multSecMinusOneAndSum(*tmp));
            sIh += valueIh;
            tmp->clear();

            this->fillHorVector(x, y, *tmp, *(this->BvReader));
            float valueBv = fabs(this->multSecMinusOneAndSum(*tmp));
            sVv += std::max((float)0.0, (valueIv - valueBv));
            tmp->clear();

            // because we write transpose
            this->fillHorVector(x, y, *tmp, *(this->BhReader));
            float valueBh = fabs(this->multSecMinusOneAndSum(*tmp));
            sVh += std::max((float)0.0, (valueIh - valueBh));
            tmp->clear();
        }
    }

    delete tmp;

    qInfo() << "sIv: " << sIv;
    qInfo() << "sVv: " << sVv;
    qInfo() << "sIh: " << sIh;
    qInfo() << "sVh: " << sVh;

    return 1 - std::max((sIv - sVv) / sIv, (sIh - sVh) / sIh);
}
