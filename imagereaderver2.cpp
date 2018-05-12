#include "imagereaderver2.h".h"
#include "config.h"
#include <QImage>
#include <QDebug>
#include <math.h>
#include <immintrin.h>

inline void transpose(unsigned char *A, unsigned int *B, unsigned int *BT, const int height, const int width) {
    unsigned char value = 0;
    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++)
        {
            value = (int)A[y*width + x];
            B[y*width + x] = value;
            BT[x*height + y] = value;
        }
}

imagereaderVer2::imagereaderVer2()
{
    Config::info();
    QImage img = QImage(Config::imgPath);
    this->width = img.width();
    this->height = img.height();
    this->img = img.convertToFormat(QImage::Format_Grayscale8);
    this->imgPtrT = new unsigned int [this->width * this->height];
    this->imgPtr = new unsigned int [this->width * this->height];
    transpose(img.bits(),this->imgPtr, this->imgPtrT, this->height, this->width);
}

imagereaderVer2::imagereaderVer2(int width, int height, unsigned int *imgPtr)
{
    this->width = width;
    this->height = height;
    this->imgPtr = imgPtr;
}

unsigned int imagereaderVer2::getT(int x, int y)
{
        if (y >= this->width || x >= this->height || x < 0 || y < 0)
            return 0;
        return this->imgPtrT[y*this->height + x];
}

unsigned int imagereaderVer2::get(int x, int y)
{

        if (x >= this->width || y >= this->height || x < 0 || y < 0)
            return 0;
        return this->imgPtr[y*this->width + x];
}

void imagereaderVer2::show(const int mode)
{
    for (int x = 0; x < this->width; x++)
    {
        for (int y = 0; y < this->height; y++)
        {
            if (mode == 0)
                qInfo() << "x: " << x << "y: " << y << "value" << this->get(x, y);
            else
                qInfo() << "x: " << x << "y: " << y << "value" << this->getT(x, y);
        }
    }
}

void imagereaderVer2::checkTranspose()
{
    QImage b(this->height, this->width, QImage::Format_RGB32);
    QImage a(this->width, this->height, QImage::Format_RGB32);
    for (int y = 0; y < this->height; y++)
    {
        for (int x = 0; x < this->width; x++)
        {
            b.setPixelColor(y , x, QColor(this->getT(y, x), this->getT(y, x), this->getT(y, x)));
            a.setPixelColor(x , y, QColor(this->get(x, y), this->get(x, y), this->get(x, y)));
            if (fabs((this->get(x, y) - this->getT(y, x))) > 0.1)
                qInfo() << "x: " << x << "y: " << y << "value" << this->get(x, y) << "valueT" << this->getT(y, x);
        }
    }
    a.save("/Users/vano144/Downloads/3_1.jpg" );
    b.save("/Users/vano144/Downloads/3_2.jpg" );
}




