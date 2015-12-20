#ifndef GPSSOLVER_H
#define GPSSOLVER_H
#include <QtGlobal>
#include "gpsephemeris.h"
#include <QVector>


class GPSSolverPrivate;

class GPSSolver
{
public:
    GPSSolver();
    ~GPSSolver();

    void setEphemeris(const GPSEphemeris &ephemeris);
    void setTime(double time);
    void setDelay(double delay);

    int tPre() const;
    double tSv() const;
    double tGPS() const;
    double tK() const;
    double n() const;
    double mK() const;
    double eK() const;
    double qK() const;
    double fK() const;
    double uK() const;
    double rK() const;
    double iK() const;
    double vrK() const;
    double vuK() const;
    double omegaK() const;
    QVector<double> coord() const;
    QVector<double> vel() const;

private:
    GPSSolverPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(GPSSolver)
};

#endif // GPSSOLVER_H
