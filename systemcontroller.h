// // systemcontroller.h
// #ifndef SYSTEMCONTROLLER_H
// #define SYSTEMCONTROLLER_H

// #include <QObject>
// #include <QList>
// #include <QPointer>
// #include <QAbstractListModel>
// #include <QMap>

// class MqttManager;
// class Device;  // Forward declaration

// class SystemController : public QAbstractListModel
// {
//     Q_OBJECT

//     // Expose properties for QML binding
//     Q_PROPERTY(int deviceCount READ deviceCount NOTIFY deviceCountChanged)
//     Q_PROPERTY(QString hubId READ hubId CONSTANT)  // For app-wide hub prefix, e.g., "esp32hub"

// public:
//     explicit SystemController(MqttManager *mqttManager, QObject *parent = nullptr);
//     ~SystemController();

//     // Dependency injection for MQTT
//     MqttManager *mqttManager() const;

//     QString hubId() const { return _hubId; }

//     // Device management with uniqueness enforcement
//     bool addDevice(Device *device);  // Returns true on success, false if name conflict
//     void removeDevice(Device *device);
//     Device *deviceAt(int index) const;
//     Device *deviceByName(const QString &name) const;
//     int deviceCount() const;

//     // QAbstractListModel overrides for exposing devices to QML (e.g., ListView)
//     enum DeviceRoles {
//         NameRole = Qt::UserRole + 1,
//         TypeRole,
//         StatusRole  // Add more as needed, e.g., DataRole for specific values
//     };
//     int rowCount(const QModelIndex &parent = QModelIndex()) const override;
//     QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
//     QHash<int, QByteArray> roleNames() const override;

// signals:
//     void deviceAdded(Device *device);
//     void deviceRemoved(Device *device);
//     void deviceCountChanged();
//     void errorOccurred(const QString &error);  // Bubble up errors from devices/MQTT

// private slots:
//     void onMqttError(const QString &error);

// private:
//     MqttManager *_mqttManager;
//     QList<QPointer<Device>> _devices;  // Ordered list for model
//     QMap<QString, QPointer<Device>> _devicesByName;  // For fast uniqueness lookup
//     const QString _hubId = "esp32hub";  // Hardcoded; could be configurable via constructor
// };

// #endif // SYSTEMCONTROLLER_H
