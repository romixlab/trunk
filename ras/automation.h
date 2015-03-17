#ifndef AUTOMATION_H
#define AUTOMATION_H

#include <QObject>

class Automation : public QObject
{
    Q_OBJECT
public:
    explicit Automation(QObject *parent = 0);
    bool connect(const QString &source, const QString &signal, QObject *receiver, const char *slot);

signals:

public slots:

};

#endif // AUTOMATION_H
