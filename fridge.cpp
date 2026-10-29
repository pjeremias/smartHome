#include "fridge.h"
#include "mqttmanager.h"
#include <QJsonObject>

Fridge::Fridge(MqttManager* manager, const QString &name) :
    Device(name),
    _mqtt(manager)
{
    // HORRIBLE SE CREAN NUEVOS SUSCRIPTION CADA VEZ QUE SE RECONECTA
    // ESTA LOGICA DEBERIA ESTAR EN ALGUN MANAGER
    connect(_mqtt, &MqttManager::connected,
            this, &Fridge::suscribeToTopics);
}

void Fridge::setSetPoint(float sp)
{
    qDebug() << "Setting setpoint (float):" << sp;
    QJsonObject configJson;
    // Maybe check here the sp value before sending
    configJson["setPoint"] = sp;

    // publishConfig(QJsonObject config) {
    QJsonObject commandJson;
    commandJson["command"] = "set_output_config";
    commandJson["id"] = name();
    commandJson["config"] = configJson;

    QJsonDocument doc(commandJson);
    QByteArray payload = doc.toJson(QJsonDocument::Compact);

    _mqtt->publish("esp32hub/commands", payload);
    // }
}

void Fridge::setHysteresis(float hyst)
{
    QJsonObject configJson;
    // Maybe check here the sp value before sending
    configJson["hysteresis"] = hyst;


    // publishConfig() {
    QJsonObject commandJson;
    commandJson["command"] = "set_output_config";
    commandJson["id"] = name();
    commandJson["config"] = configJson;

    QJsonDocument doc(commandJson);
    QByteArray payload = doc.toJson(QJsonDocument::Compact);

    _mqtt->publish("esp32hub/commands", payload);
    // }
}

void Fridge::setConfig(QVariantHash& config)
{
    // Validate and update local if needed, but primarily send to hub
    QJsonObject configJson;
    for (auto it = config.constBegin(); it != config.constEnd(); ++it) {
        configJson[it.key()] = QJsonValue::fromVariant(it.value());
    }

    QJsonObject commandJson;
    commandJson["command"] = "set_output_config";
    commandJson["id"] = name();
    commandJson["config"] = configJson;

    QJsonDocument doc(commandJson);
    QByteArray payload = doc.toJson(QJsonDocument::Compact);

    _mqtt->publish("esp32hub/commands", payload);
}

QString Fridge::type() const
{
    return "fridge";
}

void Fridge::suscribeToTopics()
{
    auto subTemp = _mqtt->subscribe("esp32hub/sensors/ds18b20/temp");
    if (subTemp) {
        // Connect the SPECIFIC signal from the subscription object
        // to a dedicated slot for handling living room messages.
        connect(subTemp, &QMqttSubscription::messageReceived,
                this, &Fridge::tempReceived);
    }
    auto subOutState = _mqtt->subscribe("esp32hub/outputs/fridge/state");
    if (subOutState) {
        // Connect the SPECIFIC signal from the subscription object
        // to a dedicated slot for handling living room messages.
        connect(subOutState, &QMqttSubscription::messageReceived,
                this, &Fridge::outStateReceived);
    }
    auto subOutconfig = _mqtt->subscribe("esp32hub/outputs/fridge/config");
    if (subOutconfig) {
        // Connect the SPECIFIC signal from the subscription object
        // to a dedicated slot for handling living room messages.
        connect(subOutconfig, &QMqttSubscription::messageReceived,
                this, &Fridge::outConfigReceived);
    }
}

void Fridge::tempReceived(const QMqttMessage &message)
{
    bool ok;
    const QByteArray payloadData = message.payload();
    float temperature = payloadData.toFloat(&ok);

    if (ok) {
        if (temperature != _temperature) {
            _temperature = temperature;
            emit temperatureChanged();
            emit dataChanged();
        }
        qDebug() << "Parsed temperature (float):" << temperature;
    } else {
        qDebug() << "Failed to parse float from message:" << payloadData;
    }
}

void Fridge::outStateReceived(const QMqttMessage &message)
{
    const QString newstate =
        QString::fromUtf8(message.payload()).trimmed().toLower();

    if (_state == newstate)
        return;

    _state = newstate;
    emit stateChanged();
    emit dataChanged();

    qDebug() << "Parsed state:" << _state;
}


void Fridge::outConfigReceived(const QMqttMessage &message)
{
    const QByteArray payloadData = message.payload();
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(payloadData, &error);
    if (error.error != QJsonParseError::NoError) {
        qDebug() << "Failed to parse config JSON:" << error.errorString();
        return;
    }
    if (!doc.isObject()) {
        qDebug() << "Config payload is not a JSON object";
        return;
    }

    QJsonObject configObj = doc.object();
    QVariantHash config;
    for (auto it = configObj.constBegin(); it != configObj.constEnd(); ++it) {
        config[it.key()] = it.value().toVariant();
    }

    // Update local state from received config
    if (config.contains("setPoint")) {
        bool ok;
        float tempSP = config["setPoint"].toFloat(&ok);
        if (ok && tempSP != _setPoint) {
            _setPoint = tempSP;
            emit setPointChanged();
            emit dataChanged();
        }
    }
    if (config.contains("hysteresis")) {
        bool ok;
        float tempHyst = config["hysteresis"].toFloat(&ok);
        if (ok && tempHyst != _hysteresis) {
            _hysteresis = tempHyst;
            emit hysteresisChanged();
            emit dataChanged();
        }
    }

    qDebug() << "Parsed config:" << config;
}
