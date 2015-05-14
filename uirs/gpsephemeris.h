#ifndef GPSEPHEMERIS_H
#define GPSEPHEMERIS_H
#include <QtGlobal>

class GPSEphemerisPrivate;

class GPSEphemeris
{
public:
    GPSEphemeris();
    ~GPSEphemeris();
    bool load(const QString &fileName);

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

    int     sv();
    int     tow();
    Flags   flags();
    int     iodc();
    int     toc();
    int     ura();
    int     healthS();
    int     wn();
    double  tgd();
    double  af2();
    double  af1();
    double  af0();
    int     toe();
    int     iode();
    double  rootA();
    double  ecc();
    double  m0();
    double  omega0();
    double  inc0();
    double  argPer();
    double  deln();
    double  omegaDot();
    double  incDot();
    double  crc();
    double  crs();
    double  cuc();
    double  cus();
    double  cic();
    double  cis();
    double  cs();

private:
    GPSEphemerisPrivate *d_ptr;
    Q_DECLARE_PRIVATE(GPSEphemeris)
};

Q_DECLARE_OPERATORS_FOR_FLAGS(GPSEphemeris::Flags)

#endif // GPSEPHEMERIS_H
