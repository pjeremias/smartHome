#ifndef MQTTMANAGER_H
#define MQTTMANAGER_H

#include <QObject>
#include <QMqttClient>
#include <QMqttTopicName>
#include <QMap>
#include <QList>
#include <QTimer>
#include <QMetaEnum>

class MqttManager : public QObject
{
    Q_OBJECT

public:
    explicit MqttManager(QObject *parent = nullptr);
    ~MqttManager();

    void connectToHost(const QString &host, quint16 port, const QString &username = QString(), const QString &password = QString());
    void disconnectFromHost();

    QMqttSubscription* subscribe(const QString &topic);
    void unsubscribe(const QString &topic, QObject *client);

    void publish(const QString &topic, const QByteArray &payload, int qos = 0, bool retain = false);

signals:
    void connected();
    void disconnected();
    void errorOccurred(const QString &errorMessage);
    void messageReceived(const QString &topic, const QByteArray &payload);  // Emitted globally; clients can connect and filter by topic

private slots:
    void onStateChanged(QMqttClient::ClientState state);
    void onMessageReceived(const QByteArray &payload, const QMqttTopicName &topicName);
    void onErrorChanged(QMqttClient::ClientError error);
    void attemptReconnect();

private:
    QMqttClient *_client;
    QString _host;
    quint16 _port;
    QString _username;
    QString _password;
    QMap<QString, QList<QObject*>> _subscriptions;  // Topic to list of client objects
    QTimer *_reconnectTimer;
    int _reconnectAttempts;
    static const int MAX_RECONNECT_ATTEMPTS = 5;
    static const int RECONNECT_INTERVAL_MS = 5000;  // 5 seconds base, with exponential backoff
};

#endif // MQTTMANAGER_H
