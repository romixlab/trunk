#ifndef GPSEPHEMERIS_H
#define GPSEPHEMERIS_H

#include <QtGlobal>
#include <QJsonObject>
#include <QSharedDataPointer>

class GPSEphemerisPrivate;

class GPSEphemeris
{
public:
    GPSEphemeris();
    GPSEphemeris(GPSEphemeris &other);
    ~GPSEphemeris();

    GPSEphemeris &operator=(const GPSEphemeris &other);

    bool load(const QVariantMap &ephemeris);

    enum class Flag {
            F_0 = 1,
            F_1 = 2,
            F_2 = 4,
            F_3 = 8,
            F_4 = 16,
            F_5 = 32,
            F_6 = 64,
            F_7 = 128
    };
    Q_DECLARE_FLAGS(Flags, Flag)

    int     sv() const;
    int     tow() const;
    Flags   flags() const;
    int     iodc() const;
    int     toc() const;
    int     ura() const;
    int     healthS() const;
    int     wn() const;
    double  tgd() const;
    double  af2() const;
    double  af1() const;
    double  af0() const;
    int     toe() const;
    int     iode() const;
    double  rootA() const;
    double  ecc() const;
    double  m0() const;
    double  omega0() const;
    double  inc0() const;
    double  argPer() const;
    double  deln() const;
    double  omegaDot() const;
    double  incDot() const;
    double  crc() const;
    double  crs() const;
    double  cuc() const;
    double  cus() const;
    double  cic() const;
    double  cis() const;
    double  cs() const;

private:
    QSharedDataPointer<GPSEphemerisPrivate> d;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(GPSEphemeris::Flags)

#endif // GPSEPHEMERIS_H
