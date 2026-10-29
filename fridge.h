#ifndef FRIDGE_H
#define FRIDGE_H

#include <QObject>
#include "device.h"

class MqttManager;
class QMqttMessage;
class Sensor;
class Output;

class Fridge : public Device
{
    Q_OBJECT
    Q_PROPERTY(QString state READ state NOTIFY stateChanged)
    Q_PROPERTY(float setPoint READ setPoint WRITE setSetPoint NOTIFY setPointChanged)
    Q_PROPERTY(float hysteresis READ hysteresis WRITE setHysteresis NOTIFY hysteresisChanged)
    Q_PROPERTY(float temperature READ temperature NOTIFY temperatureChanged)

public:
    Fridge(MqttManager* manager, const QString &name);

    void setConfig(QVariantHash& config) override;
    // void setSetPoint(float sp);
    void setHysteresis(float hyst);

    QString type() const override;
    
    QString state() const { return _state; }
    float setPoint() const { return _setPoint; }
    float hysteresis() const { return _hysteresis; }
    float temperature() const { return _temperature; }

public slots:
    void setSetPoint(float sp) override;
    // void setConfig(QVariantHash& config) override;

signals:
    void stateChanged();
    void setPointChanged();
    void hysteresisChanged();
    void temperatureChanged();

private:
    void suscribeToTopics() override;
    void tempReceived(const QMqttMessage &message);
    void outStateReceived(const QMqttMessage &message);
    void outConfigReceived(const QMqttMessage &message);

private:
    // Sensor* _tempSensor;
    // Output* _coolingOutput;
    MqttManager* _mqtt;
    QString _state;
    float _setPoint = 20.0f;
    float _hysteresis = 2.0f;
    float _temperature = 0.0f;
};

#endif // FRIDGE_H
