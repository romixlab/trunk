#include <QCoreApplication>
#include "gpsephemeris.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    GPSEphemeris eph;
    eph.load("ephemeris.json");
    qDebug() << eph.iodc() << eph.ecc();

    return a.exec();
}
