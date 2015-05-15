#include <QCoreApplication>
#include "gpsephemeris.h"
#include "gpssolver.h"
#include <iostream>
#include <ios>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    cout << "-=- GPS Solver v0.0 -=-" << endl;

    GPSEphemeris ephemeris;
    cout << "Loading ephemeris.json ...";
    bool ok = ephemeris.load("ephemeris.json");
    cout << (ok == true ? "ok" : "error") << endl;
    if (!ok)
        return -1;

    GPSSolver solver;
    solver.setEphemeris(ephemeris);

    cout << "Loading data.json ...";
    QFile file("data.json");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        cout << file.errorString().toLocal8Bit().constBegin();
        return -1;
    } else {
        cout << "ok" << endl;
    }
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    QJsonObject g = doc.object();
    solver.setTime(g["time"].toDouble());
    solver.setDelay(g["delay"].toDouble());

    cout.precision(14);
    cout.setf(ios::fixed);

    cout << "tPre:" << solver.tPre() << endl;
    cout << "tSv:" << solver.tSv() << endl;
    cout << "tGPS:" << solver.tGPS() << endl;
    cout << "tK:" << solver.tK() << endl;
    cout << "n:" << solver.n() << endl;
    cout << "mK:" << solver.mK() << endl;
    cout << "eK:" << solver.eK() << endl;
    cout << "qK:" << solver.qK() << endl;
    cout << "fK:" << solver.fK() << endl;
    cout << "uK:" << solver.uK() << endl;
    cout << "rK:" << solver.rK() << endl;
    cout << "iK:" << solver.iK() << endl;
    cout << "omegaK:" << solver.omegaK() << endl;
    QVector<double> coord = solver.coord();
    cout << "x y z:" << endl << coord[0] << endl << coord[1] << endl << coord[2];

    cout.flush();

    return 0;
    //return a.exec();
}
