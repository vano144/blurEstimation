#ifndef CONFIG_H
#define CONFIG_H
#include <QString>
#include <QDebug>

namespace Config
{
    static const QString imgPath = "/Users/vano144/Downloads/img.png";
    static const int kSize = 5;
    static const int mode = 1; // 0 for simple

    void static info();
}

void static Config::info()
{
    qInfo() << "ImgPath: " << Config::imgPath << "\nKSize: " << Config::kSize << "\nmode: " << Config:: mode;
}

#endif // CONFIG_H
