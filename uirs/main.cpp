#include <QCoreApplication>
#include "gpsephemeris.h"
#include "gpssolver.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qDebug() << "-=- GPS Solver v0.1 -=-";

    qDebug() << "Loading data.json... ";
    QFile file("data.json");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << file.errorString();
        return -1;
    } else {
        qDebug() << "ok";
    }
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    QJsonObject g = doc.object();
    QVariantMap satellites = g.toVariantMap();

    GPSEphemeris ephemeris;
    GPSSolver solver;

    qDebug() << "time:" << qSetRealNumberPrecision(20) << satellites["time"].toDouble();
    solver.setTime(satellites["time"].toDouble());

    QMapIterator<QString, QVariant> it(satellites);
    while (it.hasNext()) {
        it.next();
        if (it.key() == "time")
            continue;
        qDebug() << endl << it.key();
        ephemeris.load(it.value().toMap()["ephemeris"].toMap());
        solver.setEphemeris(ephemeris);
        solver.setDelay(it.value().toMap()["delay"].toDouble());



        if (a.arguments().contains("d")) {
            qDebug() << qSetRealNumberPrecision(20) << "tPre:" << solver.tPre();
            qDebug() << qSetRealNumberPrecision(20) << "tSv:" << solver.tSv();
            qDebug() << qSetRealNumberPrecision(20) << "tGPS:" << solver.tGPS();
            qDebug() << qSetRealNumberPrecision(20) << "tK:" << solver.tK();
            qDebug() << qSetRealNumberPrecision(20) << "n:" << solver.n();
            qDebug() << qSetRealNumberPrecision(20) << "mK:" << solver.mK();
            qDebug() << qSetRealNumberPrecision(20) << "eK:" << solver.eK();
            qDebug() << qSetRealNumberPrecision(20) << "qK:" << solver.qK();
            qDebug() << qSetRealNumberPrecision(20) << "fK:" << solver.fK();
            qDebug() << qSetRealNumberPrecision(20) << "uK:" << solver.uK();
            qDebug() << qSetRealNumberPrecision(20) << "rK:" << solver.rK();
            qDebug() << qSetRealNumberPrecision(20) << "iK:" << solver.iK();
            qDebug() << qSetRealNumberPrecision(20) << "omegaK:" << solver.omegaK();
        }
        QVector<double> coord = solver.coord();
        qDebug() << qSetRealNumberPrecision(20) << "x y z:" << coord[0] << coord[1] << coord[2];
        coord = solver.vel();
        qDebug() << qSetRealNumberPrecision(20) << "vx vy vz:" << coord[0] << coord[1] << coord[2];
    }

    return 0;
    //return a.exec();
}
