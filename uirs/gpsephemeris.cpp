#include "gpsephemeris.h"
#include <QAtomicInt>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>

class GPSEphemerisPrivate : public QSharedData {
public:
    GPSEphemerisPrivate()
    {
    }

    GPSEphemerisPrivate(const GPSEphemerisPrivate &other) : QSharedData(other),
        sv(other.sv), tow(other.tow), flags(other.flags), iodc(other.iodc),
        toc(other.toc), ura(other.ura), healthS(other.healthS), wn(other.wn),
        tgd(other.tgd), af2(other.af2), af1(other.af1), af0(other.af0),
        toe(other.toe), iode(other.iode), rootA(other.rootA), ecc(other.ecc),
        m0(other.m0), omega0(other.omega0), inc0(other.inc0), argPer(other.argPer),
        deln(other.deln), omegaDot(other.omegaDot), incDot(other.incDot),
        crc(other.crc), crs(other.crs), cuc(other.cuc), cus(other.cus),
        cic(other.cic), cis(other.cis), cs(other.cs)
    {
    }

    int sv;
    int tow;
    GPSEphemeris::Flags flags;
    int iodc;
    int toc;
    int ura;
    int healthS;
    int wn;
    double tgd;
    double af2;
    double af1;
    double af0;
    int toe;
    int iode;
    double rootA;
    double ecc;
    double m0;
    double omega0;
    double inc0;
    double argPer;
    double deln;
    double omegaDot;
    double incDot;
    double crc;
    double crs;
    double cuc;
    double cus;
    double cic;
    double cis;
    double cs;
};

GPSEphemeris::GPSEphemeris() :
    d(new GPSEphemerisPrivate)
{
}

GPSEphemeris::GPSEphemeris(GPSEphemeris &other) :
    d(other.d)
{
}

GPSEphemeris::~GPSEphemeris()
{
}

GPSEphemeris &GPSEphemeris::operator=(const GPSEphemeris &other)
{
    d = other.d;
    return *this;
}

bool GPSEphemeris::load(const QVariantMap &ephemeris)
{
    QVariantMap g = ephemeris;
    d->sv = g["sv"].toInt();
    d->tow = g["tow"].toInt();
    d->iodc = g["iodc"].toInt();
    d->toc = g["toc"].toInt();
    d->ura = g["ura"].toInt();
    d->healthS = g["healthS"].toInt();
    d->wn = g["wn"].toInt();
    d->tgd = g["tgd"].toDouble();
    d->af2 = g["af2"].toDouble();
    d->af1 = g["af1"].toDouble();
    d->af0 = g["af0"].toDouble();
    d->toe = g["toe"].toInt();
    d->iode = g["iode"].toInt();
    d->rootA = g["rootA"].toDouble();
    d->ecc = g["ecc"].toDouble();
    d->m0 = g["m0"].toDouble();
    d->omega0 = g["omega0"].toDouble();
    d->inc0 = g["inc0"].toDouble();
    d->argPer = g["argPer"].toDouble();
    d->deln = g["deln"].toDouble();
    d->omegaDot = g["omegaDot"].toDouble();
    d->incDot = g["incDot"].toDouble();
    d->crc = g["crc"].toDouble();
    d->crs = g["crs"].toDouble();
    d->cuc = g["cuc"].toDouble();
    d->cus = g["cus"].toDouble();
    d->cic = g["cic"].toDouble();
    d->cis = g["cis"].toDouble();
    d->cs = g["cs"].toDouble();

    return true;
}

int GPSEphemeris::sv() const
{
    return d->sv;
}

int GPSEphemeris::tow() const
{
    return d->tow;
}

GPSEphemeris::Flags GPSEphemeris::flags() const
{
    return d->flags;
}

int GPSEphemeris::iodc() const
{
    return d->iodc;
}

int GPSEphemeris::toc() const
{
    return d->toc;
}

int GPSEphemeris::ura() const
{
    return d->ura;
}

int GPSEphemeris::healthS() const
{
    return d->healthS;
}

int GPSEphemeris::wn() const
{
    return d->wn;
}

double GPSEphemeris::tgd() const
{
    return d->tgd;
}

double GPSEphemeris::af2() const
{
    return d->af2;
}

double GPSEphemeris::af1() const
{
    return d->af1;
}

double GPSEphemeris::af0() const
{
    return d->af0;
}

int GPSEphemeris::toe() const
{
    return d->toe;
}

int GPSEphemeris::iode() const
{
    return d->iode;
}

double GPSEphemeris::rootA() const
{
    return d->rootA;
}

double GPSEphemeris::ecc() const
{
    return d->ecc;
}

double GPSEphemeris::m0() const
{
    return d->m0 * M_PI;
}

double GPSEphemeris::omega0() const
{
    return d->omega0 * M_PI;
}

double GPSEphemeris::inc0() const
{
    return d->inc0 * M_PI;
}

double GPSEphemeris::argPer() const
{
    return d->argPer * M_PI;
}

double GPSEphemeris::deln() const
{
    return d->deln * M_PI;
}

double GPSEphemeris::omegaDot() const
{
    return d->omegaDot * M_PI;
}

double GPSEphemeris::incDot() const
{
    return d->incDot * M_PI;
}

double GPSEphemeris::crc() const
{
    return d->crc;
}

double GPSEphemeris::crs() const
{
    return d->crs;
}

double GPSEphemeris::cuc() const
{
    return d->cuc;
}

double GPSEphemeris::cus() const
{
    return d->cus;
}

double GPSEphemeris::cic() const
{
    return d->cic;
}

double GPSEphemeris::cis() const
{
    return d->cis;
}

double GPSEphemeris::cs() const
{
    return d->cs;
}
