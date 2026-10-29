// // systemcontroller.cpp
// #include "systemcontroller.h"
// #include "mqttmanager.h"
// #include "device.h"
// #include <QDebug>

// SystemController::SystemController(MqttManager *mqttManager, QObject *parent)
//     : QAbstractListModel(parent),
//     _mqttManager(mqttManager)
// {
//     if (!_mqttManager) {
//         qWarning() << "SystemController requires a valid MqttManager";
//     } else {
//         connect(_mqttManager, &MqttManager::errorOccurred, this, &SystemController::onMqttError);
//     }
//     // Removed central messageReceived connection—devices handle their own via wildcards
// }

// SystemController::~SystemController()
// {
//     // If owning devices, qDeleteAll(_devices); but assuming QML/external ownership
// }

// MqttManager *SystemController::mqttManager() const
// {
//     return _mqttManager;
// }

// bool SystemController::addDevice(Device *device)
// {
//     if (!device || _devicesByName.contains(device->name())) {
//         emit errorOccurred("Device name '" + device->name() + "' already exists");
//         return false;  // Uniqueness enforcement
//     }
//     if (_devices.contains(device)) {
//         return false;  // Avoid duplicates
//     }

//     beginInsertRows(QModelIndex(), rowCount(), rowCount());
//     _devices.append(device);
//     _devicesByName.insert(device->name(), device);
//     endInsertRows();

//     // Optional: Take ownership if desired (better for memory safety if not QML-created)
//     // device->setParent(this);

//     device->configure(_mqttManager);

//     // Bubble up changes/errors
//     connect(device, &Device::dataChanged, [this, device]() {
//         int index = _devices.indexOf(device);
//         if (index != -1) {
//             emit dataChanged(createIndex(index, 0), createIndex(index, 0));
//         }
//     });
//     connect(device, &Device::errorOccurred, this, &SystemController::errorOccurred);

//     emit deviceAdded(device);
//     emit deviceCountChanged();
//     return true;
// }

// void SystemController::removeDevice(Device *device)
// {
//     int index = _devices.indexOf(device);
//     if (index == -1) {
//         return;
//     }

//     beginRemoveRows(QModelIndex(), index, index);
//     _devices.removeAt(index);
//     _devicesByName.remove(device->name());
//     endRemoveRows();

//     device->deconfigure(_mqttManager);
//     device->disconnect(this);  // Disconnect our signals

//     emit deviceRemoved(device);
//     emit deviceCountChanged();
// }

// Device *SystemController::deviceAt(int index) const
// {
//     if (index < 0 || index >= _devices.size()) {
//         return nullptr;
//     }
//     return _devices.at(index);
// }

// Device *SystemController::deviceByName(const QString &name) const
// {
//     return _devicesByName.value(name);
// }

// int SystemController::deviceCount() const
// {
//     return _devices.size();
// }

// int SystemController::rowCount(const QModelIndex &parent) const
// {
//     Q_UNUSED(parent);
//     return _devices.size();
// }

// QVariant SystemController::data(const QModelIndex &index, int role) const
// {
//     if (!index.isValid() || index.row() >= _devices.size()) {
//         return QVariant();
//     }

//     Device *device = _devices.at(index.row());
//     switch (role) {
//     case NameRole:
//         return device->name();
//     case TypeRole:
//         return device->type();
//     case StatusRole:
//         return device->status();
//     default:
//         return QVariant();
//     }
// }

// QHash<int, QByteArray> SystemController::roleNames() const
// {
//     QHash<int, QByteArray> roles;
//     roles[NameRole] = "name";
//     roles[TypeRole] = "type";
//     roles[StatusRole] = "status";
//     return roles;
// }

// void SystemController::onMqttError(const QString &error)
// {
//     emit errorOccurred(error);
// }
