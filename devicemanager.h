#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include <QObject>
#include <QList>
#include "device.h"

class DeviceManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject*> devices READ devices NOTIFY devicesChanged)

public:
    explicit DeviceManager(QObject *parent = nullptr);
    ~DeviceManager();

    QList<QObject*> devices() const;
    void addDevice(Device *device);
    void removeDevice(Device *device);

signals:
    void devicesChanged();

private:
    QList<QObject*> _devices;
};

#endif // DEVICEMANAGER_H
