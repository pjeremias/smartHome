// device.cpp
#include "device.h"
#include <QDebug>

Device::Device(const QString &name, QObject *parent)
    : QObject(parent),
    _name(name)
{
}

Device::~Device()
{
}

QString Device::name() const
{
    return _name;
}
