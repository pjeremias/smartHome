// device.h
#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>

class Device : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(QString type READ type CONSTANT)
    // Q_PROPERTY(QList<QObject*> sensors READ sensors CONSTANT)
    // Q_PROPERTY(QList<QObject*> outputs READ outputs CONSTANT)

public:
    explicit Device(const QString &name, QObject *parent = nullptr);
    virtual ~Device();

    // virtual QList<QObject*> sensors() const = 0;
    // virtual QList<QObject*> outputs() const = 0;

    virtual void setConfig(QVariantHash& config) = 0;

    QString name() const;

    // To be overridden by subclasses
    virtual QString type() const = 0;  // e.g., "Fridge"

public slots:
    virtual void setSetPoint(float sp) = 0;

signals:
    void dataChanged();  // Emitted when internal state (e.g., sensor values) changes
    void errorOccurred(const QString &error);

protected:
    virtual void suscribeToTopics() = 0;

private:
    QString _name;
};

#endif // DEVICE_H
