#include "gpssolver.h"
#include <QDebug>

namespace Const {
const double C =    -4.442807633e-10;
const double M =    3.986005e14;
const double W_3 =  7.2921151467e-5;
}

class GPSSolverPrivate {
public:
    GPSEphemeris g;
    double t;
    double delay;
};

GPSSolver::GPSSolver() :
    d_ptr(new GPSSolverPrivate)
{

}

GPSSolver::~GPSSolver()
{
    if (d_ptr) {
        delete d_ptr;
    }

}

void GPSSolver::setTime(double time)
{
    Q_D(GPSSolver);
    d->t = time;
}

void GPSSolver::setDelay(double delay)
{
    Q_D(GPSSolver);
    d->delay = delay;
}

void GPSSolver::setEphemeris(const GPSEphemeris &ephemeris)
{
    Q_D(GPSSolver);
    d->g = ephemeris;
}

int GPSSolver::tPre() const
{
    Q_D(const GPSSolver);
    return d->t / 1000 + 86400 * ((int)((d->g.tow() - d->t / 1000) / 86400));
}

double GPSSolver::tSv() const
{
    Q_D(const GPSSolver);
    return tPre() - d->delay;
}

double GPSSolver::tGPS() const
{
    Q_D(const GPSSolver);
    double deltaTr = Const::C * d->g.ecc() * d->g.rootA() * sin(d->g.m0());
    return tSv() -
            d->g.af0() +
            d->g.af1() * (tSv() - d->g.toc()) +
            d->g.af2() * (tSv() - d->g.toc()) * (tSv() - d->g.toc()) +
            deltaTr;
}

double GPSSolver::tK() const
{
    Q_D(const GPSSolver);
    return tGPS() - d->g.toe();
}

double GPSSolver::n() const
{
    Q_D(const GPSSolver);
    return sqrt(Const::M) / pow(d->g.rootA(), 3) + d->g.deln();
}

double GPSSolver::mK() const
{
    Q_D(const GPSSolver);
    return d->g.m0() + n() * tK();
}

double GPSSolver::eK() const
{
    Q_D(const GPSSolver);
    double k0 = 0.0;
    double mk = mK();
    double k1 = mk;
    int i = 0;
    do {
        i++;
        k0 = k1;
        k1 = k1 - (k1 - d->g.ecc() * sin(k1) - mk) / (1 - d->g.ecc() * cos(k1));
    } while (fabs(k1 - k0) >= 1e-9);
    return k1;
}

double GPSSolver::qK() const
{
    Q_D(const GPSSolver);
    double ek = eK();
    return atan( (sqrt(1 - d->g.ecc() * d->g.ecc()) * sin(ek)) / (cos(ek) - d->g.ecc()) );
}

double GPSSolver::fK() const
{
    Q_D(const GPSSolver);
    return qK() + d->g.argPer();
}

double GPSSolver::uK() const
{
    Q_D(const GPSSolver);
    return fK() + d->g.cuc() * cos(2 * fK()) + d->g.cuc() * sin(2 * fK());
}

double GPSSolver::rK() const
{
    Q_D(const GPSSolver);
    return d->g.crc() * cos(2 * fK()) + d->g.crs() * sin(2 * fK()) +
            d->g.rootA() * d->g.rootA() * (1 - d->g.ecc() * cos(eK()));
}

double GPSSolver::iK() const
{
    Q_D(const GPSSolver);
    return d->g.inc0() +
            d->g.cic() * cos(2 * fK()) + d->g.cis() * sin(2 * fK()) +
            d->g.incDot() * tK();
}

double GPSSolver::vrK() const
{
    Q_D(const GPSSolver);
    return (sqrt(Const::M) / d->g.rootA()) * (d->g.ecc() * sin(qK()) / sqrt(1 - d->g.ecc() * d->g.ecc()));
}

double GPSSolver::vuK() const
{
    Q_D(const GPSSolver);
    return (sqrt(Const::M) / d->g.rootA()) * ((1 + d->g.ecc() * cos(qK())) / sqrt(1 - d->g.ecc() * d->g.ecc()));
}

double GPSSolver::omegaK() const
{
    Q_D(const GPSSolver);
    return d->g.omega0() + (d->g.omegaDot() - Const::W_3) * tK() -
            Const::W_3 * d->g.toe();
}

QVector<double> GPSSolver::coord() const
{
    double x = rK() * (cos(uK()) * cos(omegaK()) - sin(uK()) * sin(omegaK()) * cos(iK()));
    double y = rK() * (cos(uK()) * sin(omegaK()) + sin(uK()) * cos(omegaK()) * cos(iK()));
    double z = rK() * sin(uK()) * sin(iK());
    return (QVector<double>() << x << y << z);
}

QVector<double> GPSSolver::vel() const
{
    double x = vrK() * (cos(uK()) * cos(omegaK()) - sin(uK()) * sin(omegaK()) * cos(iK())) -
               vuK() * (sin(uK()) * cos(omegaK()) + cos(uK()) * sin(omegaK()) * cos(iK())) +
               Const::W_3 * rK() * (cos(uK()) * sin(omegaK()) + sin(uK()) * cos(omegaK()) * cos(iK()));
    double y = vrK() * (cos(uK()) * sin(omegaK()) + sin(uK()) * cos(omegaK()) * cos(iK())) -
               vuK() * (sin(uK()) * sin(omegaK()) - cos(uK()) * cos(omegaK()) * cos(iK())) -
               Const::W_3 * rK() * (cos(uK()) * cos(omegaK()) - sin(uK()) * sin(omegaK()) * cos(iK()));
    double z = vrK() * sin(uK()) * sin(iK()) + vuK() * cos(uK()) * sin(iK());
    return (QVector<double>() << x << y << z);
}

