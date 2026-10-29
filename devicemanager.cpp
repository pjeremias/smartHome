#include "devicemanager.h"

DeviceManager::DeviceManager(QObject *parent)
    : QObject(parent)
{
}

DeviceManager::~DeviceManager()
{
}

QList<QObject*> DeviceManager::devices() const
{
    return _devices;
}

void DeviceManager::addDevice(Device *device)
{
    if (!_devices.contains(device)) {
        _devices.append(device);
        emit devicesChanged();
    }
}

void DeviceManager::removeDevice(Device *device)
{
    if (_devices.removeOne(device)) {
        emit devicesChanged();
    }
}
