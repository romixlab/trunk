#ifndef YAMI_H
#define YAMI_H

#include <QObject>
#include <yami4-cpp/yami.h>

class Yami : public QObject
{
    Q_OBJECT
public:
    explicit Yami(QObject *parent = 0);

    void operator ()(yami::incoming_message &im);

signals:
    void message(const QString &some);

public slots:

};

#endif // YAMI_H
