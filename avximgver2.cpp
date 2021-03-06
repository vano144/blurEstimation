#include "avximgver2.h"
#include "imagereaderver2.h"
#include <math.h>
#include <QDebug>
#include "config.h"
#include <immintrin.h>

avxImgVer2::avxImgVer2(imagereaderVer2 * imgReader) : imgReader(imgReader)
{
    this->imgReader = imgReader;
    this->kSize = Config::kSize;
    this->recalcLimits();
}

void avxImgVer2::recalcLimits()
{
    this->rightLimit = kSize/2;
    this->leftLimit = rightLimit;
    if (this->kSize % 2 == 0)
        this->rightLimit -= 1;
}

avxImgVer2::~avxImgVer2() {
    delete this->BvReader;
    delete this->BhReader;
    delete this->imgReader;
    delete[] this->bIhPtr;
    delete[] this->dIhPtr;
    delete[] this->dIvPtr;
    delete[] this->bIvPtr;
}

void avxImgVer2::fillHorVector(int x0 , int y0, QVector<unsigned int> &tmp, imagereaderVer2 & imgReader)
{
    for (int x = x0 - this->leftLimit; x <= x0 + rightLimit; x++)
    {
        tmp.append(imgReader.get(x, y0));
    }
}

void avxImgVer2::fillVerVector(int x0 , int y0, QVector<unsigned int> &tmp, imagereaderVer2 & imgReader)
{
    for (int x = x0 - this->leftLimit; x <= x0 + this->rightLimit; x++)
    {
        tmp.append(imgReader.getT(x, y0));
    }
}

void avxImgVer2::fillHorVector(int x0 , int y0, QVector<int> &tmp, imagereaderVer2 & imgReader)
{
    for (int x = x0 - this->leftLimit; x <= x0 + rightLimit; x++)
    {
        tmp.append(imgReader.get(x, y0));
    }
}

void avxImgVer2::fillVerVector(int x0 , int y0, QVector<int> & tmp, imagereaderVer2 & imgReader)
{
    for (int x = x0 - this->leftLimit; x <= x0 + this->rightLimit; x++)
    {
        tmp.append(imgReader.getT(x, y0));
    }
}

unsigned int avxImgVer2::sum(QVector<unsigned int> &vec)
{
    unsigned int value = 0;
    for (int i = 0; i < vec.size(); i++)
    {
        value += vec.at(i);
    }
    return value;
}

unsigned int avxImgVer2::sum(QVector<int> &vec)
{
    unsigned int value = 0;
    for (int i = 0; i < vec.size(); i++)
    {
        value += vec.at(i);
    }
    return value;
}

int avxImgVer2::multSecMinusOneAndSum(QVector<int> &vec)
{
    vec[1] *= -1;
    return this->sum(vec);
}

void avxImgVer2::countBvAndBh()
{
    QVector<int> * tmp = new QVector<int>;
    unsigned int * bvPtr = new unsigned int [this->imgReader->width * this->imgReader->height];
    unsigned int * bhPtr = new unsigned int [this->imgReader->width * this->imgReader->height];
    unsigned int value = 0;

    QImage a(this->imgReader->width, this->imgReader->height, QImage::Format_RGB32);
    QImage b(this->imgReader->height, this->imgReader->width, QImage::Format_RGB32);

    for (int y = 0; y < this->imgReader->height; y++)
    {
        for (int x = 0; x < this->imgReader->width; x++)
        {
            this->fillHorVector(x, y, *tmp, *(this->imgReader));
            value = this->sum(*tmp) / this->kSize;
            bvPtr[y*imgReader->width + x] = value;

            tmp->clear();
            int a1 = (int)value;
            if (a1 > 255)
            {
                a1 = 255;
            }

            this->fillVerVector(y, x, *tmp, *(this->imgReader));
            value = this->sum(*tmp) / this->kSize;
            bhPtr[x*imgReader->height + y] = value;
            tmp->clear();


            int a2 = (int)value;
            if (a2 > 255)
            {
                a2 = 255;
            }

            a.setPixelColor(x , y, QColor(a1, a1, a1));
            b.setPixelColor(y , x, QColor(a2, a2, a2));
        }
    }

    a.save("/Users/vano144/Downloads/Bv.jpg" );
    b.save("/Users/vano144/Downloads/Bh.jpg" );
    this->BvReader = new imagereaderVer2(imgReader->width, imgReader->height, bvPtr);
    this->BhReader = new imagereaderVer2(imgReader->width, imgReader->height, bhPtr);
    this->BhReader->imgPtrT = bhPtr;
    delete tmp;
}

void avxImgVer2::countDandB()
{
    this->kSize = 2; // [1 -1]
    this->recalcLimits();
    QVector<int> * tmp = new QVector<int>;

    float * dIvPtr = new float [this->imgReader->width * this->imgReader->height];
    float * dIhPtr = new float [this->imgReader->width * this->imgReader->height];
    float * bIvPtr = new float [this->imgReader->width * this->imgReader->height];
    float * bIhPtr = new float [this->imgReader->width * this->imgReader->height];

    QImage a(this->imgReader->width, this->imgReader->height, QImage::Format_RGB32);
    QImage b(this->imgReader->height, this->imgReader->width, QImage::Format_RGB32);
    QImage c(this->imgReader->width, this->imgReader->height, QImage::Format_RGB32);
    QImage d(this->imgReader->height, this->imgReader->width, QImage::Format_RGB32);

    for (int y = 0; y < this->imgReader->height; y++)
    {
        for (int x = 0; x < this->imgReader->width; x++)
        {
            this->fillHorVector(x, y, *tmp, *(this->imgReader));
            unsigned int valueIv = std::abs(this->multSecMinusOneAndSum(*tmp));
            dIvPtr[y*imgReader->width + x] = valueIv;
            tmp->clear();

            this->fillVerVector(y, x, *tmp, *(this->imgReader));
            unsigned int valueIh = std::abs(this->multSecMinusOneAndSum(*tmp));
            dIhPtr[x*imgReader->height + y] = valueIh;
            tmp->clear();

            this->fillHorVector(x, y, *tmp, *(this->BvReader));
            unsigned int valueBv = std::abs(this->multSecMinusOneAndSum(*tmp));
            bIvPtr[y*imgReader->width + x] = valueBv;
            tmp->clear();

            this->fillVerVector(y, x, *tmp, *(this->BhReader));
            unsigned int valueBh = std::abs(this->multSecMinusOneAndSum(*tmp));
            bIhPtr[x*imgReader->height + y] = valueBh;
            tmp->clear();

            int a1 = valueIv;
            if (a1 > 255)
                a1 = 255;
            int a2 = valueIh;
            if (a2 > 255)
                a2 = 255;
            int a3 = valueBv;
            if (a3 > 255)
                a3 = 255;
            int a4 = valueBh;
            if (a4 > 255)
                a4 = 255;

            a.setPixelColor(x , y, QColor(a1, a1, a1));
            b.setPixelColor(y , x, QColor(a2, a2, a2));
            c.setPixelColor(x , y, QColor(a3, a3, a3));
            d.setPixelColor(y , x, QColor(a4, a4, a4));
        }
    }

    a.save("/Users/vano144/Downloads/Iv.jpg" );
    b.save("/Users/vano144/Downloads/Ih.jpg" );
    c.save("/Users/vano144/Downloads/bIv.jpg" );
    d.save("/Users/vano144/Downloads/bIh.jpg" );

    this->dIvPtr = dIvPtr;
    this->dIhPtr = dIhPtr;
    this->bIvPtr = bIvPtr;
    this->bIhPtr = bIhPtr;

    delete tmp;

}

//void avximgapi::fillHorPtr(int x0 , int y0, const int &indent, float ** tmf_avx, ImageReader & imgReader)
//{
//    (*tmf_avx)[indent] = imgReader.get(x0, y0 - 1);
//    (*tmf_avx)[indent + 1] = imgReader.get(x0, y0);
//}

//void avximgapi::fillVerPtr(int x0 , int y0, const int &indent, float ** tmf_avx, ImageReader & imgReader)
//{
//    (*tmf_avx)[indent] = imgReader.getT(x0, y0 - 1);
//    (*tmf_avx)[indent + 1] = imgReader.getT(x0, y0);
//}

inline void fillHorPtr(int x0 , int y0, const int &indent, float * tmf_avx, imagereaderVer2 & imgReader)
{
    tmf_avx[indent] = (float)imgReader.get(x0 - 1, y0);
    tmf_avx[indent + 1] = (float)imgReader.get(x0, y0);
}

inline void fillVerPtr(int x0 , int y0, const int &indent, float * tmf_avx, imagereaderVer2 & imgReader)
{
    tmf_avx[indent] = (float)imgReader.getT(x0 - 1, y0);
    tmf_avx[indent + 1] = (float)imgReader.getT(x0, y0);
}

void avxImgVer2::countDandBAVx()
{
    this->kSize = 2; // [1 -1]
    this->recalcLimits();
    float * dIvPtr = new float [this->imgReader->width * this->imgReader->height];
    float * dIhPtr = new float [this->imgReader->width * this->imgReader->height];
    float * bIvPtr = new float [this->imgReader->width * this->imgReader->height];
    float * bIhPtr = new float [this->imgReader->width * this->imgReader->height];
    float * tmpArr = new float [8];

    __m256 regstr;

    QImage a(this->imgReader->width, this->imgReader->height, QImage::Format_RGB32);
    QImage b(this->imgReader->height, this->imgReader->width, QImage::Format_RGB32);
    QImage c(this->imgReader->width, this->imgReader->height, QImage::Format_RGB32);
    QImage d(this->imgReader->height, this->imgReader->width, QImage::Format_RGB32);

    for (int y = 0; y < this->imgReader->height; y++)
    {
        for (int x = 0; x < this->imgReader->width; x++)
        {
            fillHorPtr(x, y, 0, tmpArr, *(this->imgReader));
            fillVerPtr(y, x, 2, tmpArr, *(this->imgReader));
            fillHorPtr(x, y, 4, tmpArr, *(this->BvReader));
            fillVerPtr(y, x, 6, tmpArr, *(this->BhReader));

//            __m256i regstr = _mm256_maskz_loadu_epi16((__m256i*)&tmpArr);
//            __m256i regstr = _mm256_loadu_si256((__m256*)tmpArr);
            regstr = _mm256_loadu_ps((float*)tmpArr);

            regstr = _mm256_hsub_ps(regstr, regstr);

            dIvPtr[y*imgReader->width + x] = fabs(regstr[0]);
            dIhPtr[y*imgReader->width + x] = fabs(regstr[1]);
            bIvPtr[y*imgReader->width + x] = fabs(regstr[4]);
            bIhPtr[y*imgReader->width + x] = fabs(regstr[5]);

            float a1 = fabs(regstr[0]);
            if (a1 > 255)
                a1 = 255;
            float a2 = fabs(regstr[1]);
            if (a2 > 255)
                a2 = 255;
            float a3 = fabs(regstr[4]);
            if (a3 > 255)
                a3 = 255;
            float a4 = fabs(regstr[5]);
            if (a4 > 255)
                a4 = 255;

            a.setPixelColor(x , y, QColor(a1, a1, a1));
            b.setPixelColor(y , x, QColor(a2, a2, a2));
            c.setPixelColor(x , y, QColor(a3, a3, a3));
            d.setPixelColor(y , x, QColor(a4, a4, a4));
        }
    }

    a.save("/Users/vano144/Downloads/Iv.jpg" );
    b.save("/Users/vano144/Downloads/Ih.jpg" );
    c.save("/Users/vano144/Downloads/bIv.jpg" );
    d.save("/Users/vano144/Downloads/bIh.jpg" );

    this->dIvPtr = dIvPtr;
    this->dIhPtr = dIhPtr;
    this->bIvPtr = bIvPtr;
    this->bIhPtr = bIhPtr;

    delete[] tmpArr;
}

float avxImgVer2::avxVecSum(const __m256 & x)
{
//    __m256 s = _mm256_hadd_ps(x,x);
//    return ((float*)&s)[0] + ((float*)&s)[2] + ((float*)&s)[4] + ((float*)&s)[6];
    // hiQuad = ( x7, x6, x5, x4 )
    const __m128 hiQuad = _mm256_extractf128_ps(x, 1);

    // loQuad = ( x3, x2, x1, x0 )
    const __m128 loQuad = _mm256_castps256_ps128(x);

    // sumQuad = ( x3 + x7, x2 + x6, x1 + x5, x0 + x4 )
    const __m128 sumQuad = _mm_add_ps(loQuad, hiQuad);

    // loDual = ( ignore it, ignore it, x1 + x5, x0 + x4 )
    const __m128 loDual = sumQuad;

    // hiDual = ( ignore it, ignore it, x3 + x7, x2 + x6 )
    const __m128 hiDual = _mm_movehl_ps(sumQuad, sumQuad);

    // sumDual = ( ignore it, ignore it, x1 + x3 + x5 + x7, x0 + x2 + x4 + x6 )
    const __m128 sumDual = _mm_add_ps(loDual, hiDual);

    // lo = ( ignore it, ignore it, ignore it, x0 + x2 + x4 + x6 )
    const __m128 lo = sumDual;

    // hi = ( ignore it, ignore it, ignore it, x1 + x3 + x5 + x7 )
    const __m128 hi = _mm_shuffle_ps(sumDual, sumDual, 0x1);

    // sum = ( ignore it, ignore it, ignore it, x0 + x1 + x2 + x3 + x4 + x5 + x6 + x7 )
    const __m128 sum = _mm_add_ss(lo, hi);
    return _mm_cvtss_f32(sum);
}

float avxImgVer2::countQs()
{
    __m256 sIvSumVec = _mm256_setzero_ps();
    __m256 sIhSumVec = _mm256_setzero_ps();
    __m256 sVvSumVec = _mm256_setzero_ps();
    __m256 sVhSumVec = _mm256_setzero_ps();
    __m256 dbhVec;
    __m256 dbvVec;
    __m256 dIhVec;
    __m256 dIvVec;
    __m256 diffDivAndDBv;
    __m256 diffDihAndDBh;
    __m256 minVec = _mm256_set1_ps(0);

    for (int i = 0; i < (this->imgReader->height * this->imgReader->width) / 8; i++)
    {
        dIhVec = _mm256_loadu_ps(&(this->dIhPtr[i*8]));
        dIvVec = _mm256_loadu_ps(&(this->dIvPtr[i*8]));
        dbhVec = _mm256_loadu_ps(&(this->bIhPtr[i*8]));
        dbvVec = _mm256_loadu_ps(&(this->bIvPtr[i*8]));

        diffDivAndDBv = _mm256_sub_ps(dIvVec, dbvVec);
        diffDihAndDBh = _mm256_sub_ps(dIhVec, dbhVec);

        sIvSumVec = _mm256_add_ps(sIvSumVec, dIvVec);
        sIhSumVec = _mm256_add_ps(sIhSumVec, dIhVec);
        sVvSumVec = _mm256_add_ps(sVvSumVec, _mm256_max_ps(diffDivAndDBv, minVec));
        sVhSumVec = _mm256_add_ps(sVhSumVec, _mm256_max_ps(diffDihAndDBh, minVec));

    }

    float sIv = this->avxVecSum(sIvSumVec);
    float sIh = this->avxVecSum(sIhSumVec);
    float sVv = this->avxVecSum(sVvSumVec);
    float sVh = this->avxVecSum(sVhSumVec);


//    qInfo() << "sIv: " << sIv;
//    qInfo() << "sVv: " << sVv;
//    qInfo() << "sIh: " << sIh;
//    qInfo() << "sVh: " << sVh;
    _mm256_zeroupper();
    return 1 - std::max((sIv - sVv) / sIv, (sIh - sVh) / sIh);
}
