#include "config.h"
#include "imagereader.h"
#include "imagereaderver2.h"
#include "avximgver2.h"
#include "imgapi.h"
#include "avximgapi.h"
#include <QDebug>
#include <chrono>
#include <avximgver2.h>

void withoutAvx()
{
    typedef std::chrono::high_resolution_clock Time;
    typedef std::chrono::milliseconds ms;
    typedef std::chrono::duration<float> fsec;
    auto t0 = Time::now();
    ImageReader * img = new ImageReader();
    img->checkTranspose();
    imgapi * imgApi =  new imgapi(img);
    imgApi->countBvAndBh();
    auto result = imgApi->countQs();
    auto t1 = Time::now();
    fsec fs = t1 - t0;
    ms d = std::chrono::duration_cast<ms>(fs);
    qInfo() << "result: " << result;
    qInfo() << fs.count() << "s\n";
    qInfo() << d.count() << "ms\n";
}

void withAvx()
{
    typedef std::chrono::high_resolution_clock Time;
    typedef std::chrono::milliseconds ms;
    typedef std::chrono::duration<float> fsec;

    qInfo()  << "AVX";
    ImageReader * img = new ImageReader();
    auto t0 = Time::now();
    avximgapi * imgApi =  new avximgapi(img);
    imgApi->countBvAndBh();
    imgApi->countDandBAVx();
    auto result = imgApi->countQs();
    auto t1 = Time::now();
    qInfo() << "result: " << result;
    fsec fs = t1 - t0;
    ms d = std::chrono::duration_cast<ms>(fs);
    qInfo() << "result: " << result;
    qInfo() << fs.count() << "s\n";
    qInfo() << d.count() << "ms\n";
}

void withoutQVectorAvx()
{
    typedef std::chrono::high_resolution_clock Time;
    typedef std::chrono::milliseconds ms;
    typedef std::chrono::duration<float> fsec;

    qInfo()  << "AVX - QVector";
    ImageReader * img = new ImageReader();
    auto t0 = Time::now();
    avximgapi * imgApi =  new avximgapi(img);
    imgApi->countBvAndBhCalloc();
    imgApi->countDandBAVxCalloc();
    auto result = imgApi->countQs();
    auto t1 = Time::now();
    qInfo() << "result: " << result;
    fsec fs = t1 - t0;
    ms d = std::chrono::duration_cast<ms>(fs);
    qInfo() << "result: " << result;
    qInfo() << fs.count() << "s\n";
    qInfo() << d.count() << "ms\n";
}

int main(int argc, char *argv[])
{
    withoutAvx();
    withAvx();
    withoutQVectorAvx();
    return 0;
}
