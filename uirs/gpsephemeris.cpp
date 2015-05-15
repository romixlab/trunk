#include "gpsephemeris.h"
#include <QAtomicInt>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>

class GPSEphemerisPrivate {
public:
    QAtomicInt ref;

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
    Q_D(GPSEphemeris);
    d->ref.ref();
}

GPSEphemeris::GPSEphemeris(GPSEphemeris &other)
{
    d_ptr = other.d_ptr;
    d_ptr->ref.ref();
}

GPSEphemeris::~GPSEphemeris()
{
    if (d_ptr->ref == 1) {
        delete d_ptr;
    } else {
        d_ptr->ref.deref();
    }
}

GPSEphemeris &GPSEphemeris::operator=(const GPSEphemeris &other)
{
    d_ptr->ref.deref();
    if (d_ptr->ref == 0)
        delete d_ptr;
    d_ptr = other.d_ptr;
    return *this;
}

void GPSEphemeris::detach()
{
    if (d_ptr->ref != 1) {
        d_ptr->ref.deref();
        GPSEphemerisPrivate *new_dptr = new GPSEphemerisPrivate;

        new_dptr->sv = d_ptr->sv;
        new_dptr->tow = d_ptr->tow;
        new_dptr->flags = d_ptr->flags;
        new_dptr->iodc = d_ptr->iodc;
        new_dptr->toc = d_ptr->toc;
        new_dptr->ura = d_ptr->ura;
        new_dptr->healthS = d_ptr->healthS;
        new_dptr->wn = d_ptr->wn;
        new_dptr->tgd = d_ptr->tgd;
        new_dptr->af2 = d_ptr->af2;
        new_dptr->af1 = d_ptr->af1;
        new_dptr->af0 = d_ptr->af0;
        new_dptr->toe = d_ptr->toe;
        new_dptr->iode = d_ptr->iode;
        new_dptr->rootA = d_ptr->rootA;
        new_dptr->ecc = d_ptr->ecc;
        new_dptr->m0 = d_ptr->m0;
        new_dptr->omega0 = d_ptr->omega0;
        new_dptr->inc0 = d_ptr->inc0;
        new_dptr->argPer = d_ptr->argPer;
        new_dptr->deln = d_ptr->deln;
        new_dptr->omegaDot = d_ptr->omegaDot;
        new_dptr->incDot = d_ptr->incDot;
        new_dptr->crc = d_ptr->crc;
        new_dptr->crs = d_ptr->crs;
        new_dptr->cuc = d_ptr->cuc;
        new_dptr->cus = d_ptr->cus;
        new_dptr->cic = d_ptr->cic;
        new_dptr->cis = d_ptr->cis;
        new_dptr->cs = d_ptr->cs;

        d_ptr = new_dptr;
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

int GPSEphemeris::sv() const
{
    Q_D(const GPSEphemeris);
    return d->sv;
}

int GPSEphemeris::tow() const
{
    Q_D(const GPSEphemeris);
    return d->tow;
}

GPSEphemeris::Flags GPSEphemeris::flags() const
{
    Q_D(const GPSEphemeris);
    return d->flags;
}

int GPSEphemeris::iodc() const
{
    Q_D(const GPSEphemeris);
    return d->iodc;
}

int GPSEphemeris::toc() const
{
    Q_D(const GPSEphemeris);
    return d->toc;
}

int GPSEphemeris::ura() const
{
    Q_D(const GPSEphemeris);
    return d->ura;
}

int GPSEphemeris::healthS() const
{
    Q_D(const GPSEphemeris);
    return d->healthS;
}

int GPSEphemeris::wn() const
{
    Q_D(const GPSEphemeris);
    return d->wn;
}

double GPSEphemeris::tgd() const
{
    Q_D(const GPSEphemeris);
    return d->tgd;
}

double GPSEphemeris::af2() const
{
    Q_D(const GPSEphemeris);
    return d->af2;
}

double GPSEphemeris::af1() const
{
    Q_D(const GPSEphemeris);
    return d->af1;
}

double GPSEphemeris::af0() const
{
    Q_D(const GPSEphemeris);
    return d->af0;
}

int GPSEphemeris::toe() const
{
    Q_D(const GPSEphemeris);
    return d->toe;
}

int GPSEphemeris::iode() const
{
    Q_D(const GPSEphemeris);
    return d->iode;
}

double GPSEphemeris::rootA() const
{
    Q_D(const GPSEphemeris);
    return d->rootA;
}

double GPSEphemeris::ecc() const
{
    Q_D(const GPSEphemeris);
    return d->ecc;
}

double GPSEphemeris::m0() const
{
    Q_D(const GPSEphemeris);
    return d->m0;
}

double GPSEphemeris::omega0() const
{
    Q_D(const GPSEphemeris);
    return d->omega0;
}

double GPSEphemeris::inc0() const
{
    Q_D(const GPSEphemeris);
    return d->inc0;
}

double GPSEphemeris::argPer() const
{
    Q_D(const GPSEphemeris);
    return d->argPer;
}

double GPSEphemeris::deln() const
{
    Q_D(const GPSEphemeris);
    return d->deln;
}

double GPSEphemeris::omegaDot() const
{
    Q_D(const GPSEphemeris);
    return d->omegaDot;
}

double GPSEphemeris::incDot() const
{
    Q_D(const GPSEphemeris);
    return d->incDot;
}

double GPSEphemeris::crc() const
{
    Q_D(const GPSEphemeris);
    return d->crc;
}

double GPSEphemeris::crs() const
{
    Q_D(const GPSEphemeris);
    return d->crs;
}

double GPSEphemeris::cuc() const
{
    Q_D(const GPSEphemeris);
    return d->cuc;
}

double GPSEphemeris::cus() const
{
    Q_D(const GPSEphemeris);
    return d->cus;
}

double GPSEphemeris::cic() const
{
    Q_D(const GPSEphemeris);
    return d->cic;
}

double GPSEphemeris::cis() const
{
    Q_D(const GPSEphemeris);
    return d->cis;
}

double GPSEphemeris::cs() const
{
    Q_D(const GPSEphemeris);
    return d->cs;
}
