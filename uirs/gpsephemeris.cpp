#include "gpsephemeris.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>

class GPSEphemerisPrivate {
public:
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
    d_ptr(new GPSEphemerisPrivate)
{
}

GPSEphemeris::~GPSEphemeris()
{
    if (d_ptr) {
        delete d_ptr;
        d_ptr = 0;
    }
}

bool GPSEphemeris::load(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << file.errorString();
        return false;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    QJsonObject g = doc.object()["ephemeris"].toObject();
    Q_D(GPSEphemeris);
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

int GPSEphemeris::sv()
{
    Q_D(GPSEphemeris);
    return d->sv;
}

int GPSEphemeris::tow()
{
    Q_D(GPSEphemeris);
    return d->tow;
}

GPSEphemeris::Flags GPSEphemeris::flags()
{
    Q_D(GPSEphemeris);
    return d->flags;
}

int GPSEphemeris::iodc()
{
    Q_D(GPSEphemeris);
    return d->iodc;
}

int GPSEphemeris::toc()
{
    Q_D(GPSEphemeris);
    return d->toc;
}

int GPSEphemeris::ura()
{
    Q_D(GPSEphemeris);
    return d->ura;
}

int GPSEphemeris::healthS()
{
    Q_D(GPSEphemeris);
    return d->healthS;
}

int GPSEphemeris::wn()
{
    Q_D(GPSEphemeris);
    return d->wn;
}

double GPSEphemeris::tgd()
{
    Q_D(GPSEphemeris);
    return d->tgd;
}

double GPSEphemeris::af2()
{
    Q_D(GPSEphemeris);
    return d->af2;
}

double GPSEphemeris::af1()
{
    Q_D(GPSEphemeris);
    return d->af1;
}

double GPSEphemeris::af0()
{
    Q_D(GPSEphemeris);
    return d->af0;
}

int GPSEphemeris::toe()
{
    Q_D(GPSEphemeris);
    return d->toe;
}

int GPSEphemeris::iode()
{
    Q_D(GPSEphemeris);
    return d->iode;
}

double GPSEphemeris::rootA()
{
    Q_D(GPSEphemeris);
    return d->rootA;
}

double GPSEphemeris::ecc()
{
    Q_D(GPSEphemeris);
    return d->ecc;
}

double GPSEphemeris::m0()
{
    Q_D(GPSEphemeris);
    return d->m0;
}

double GPSEphemeris::omega0()
{
    Q_D(GPSEphemeris);
    return d->omega0;
}

double GPSEphemeris::inc0()
{
    Q_D(GPSEphemeris);
    return d->inc0;
}

double GPSEphemeris::argPer()
{
    Q_D(GPSEphemeris);
    return d->argPer;
}

double GPSEphemeris::deln()
{
    Q_D(GPSEphemeris);
    return d->deln;
}

double GPSEphemeris::omegaDot()
{
    Q_D(GPSEphemeris);
    return d->omegaDot;
}

double GPSEphemeris::incDot()
{
    Q_D(GPSEphemeris);
    return d->incDot;
}

double GPSEphemeris::crc()
{
    Q_D(GPSEphemeris);
    return d->crc;
}

double GPSEphemeris::crs()
{
    Q_D(GPSEphemeris);
    return d->crs;
}

double GPSEphemeris::cuc()
{
    Q_D(GPSEphemeris);
    return d->cuc;
}

double GPSEphemeris::cus()
{
    Q_D(GPSEphemeris);
    return d->cus;
}

double GPSEphemeris::cic()
{
    Q_D(GPSEphemeris);
    return d->cic;
}

double GPSEphemeris::cis()
{
    Q_D(GPSEphemeris);
    return d->cis;
}

double GPSEphemeris::cs()
{
    Q_D(GPSEphemeris);
    return d->cs;
}
