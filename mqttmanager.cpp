// mqttmanager.cpp
#include "mqttmanager.h"
#include <QDebug>

MqttManager::MqttManager(QObject *parent)
    : QObject(parent),
    _client(new QMqttClient(this)),
    _reconnectTimer(new QTimer(this)),
    _reconnectAttempts(0)
{
    connect(_client, &QMqttClient::stateChanged, this, &MqttManager::onStateChanged);
    connect(_client, &QMqttClient::messageReceived, this, &MqttManager::onMessageReceived);
    connect(_client, &QMqttClient::errorChanged, this, &MqttManager::onErrorChanged);

    _reconnectTimer->setSingleShot(true);
    connect(_reconnectTimer, &QTimer::timeout, this, &MqttManager::attemptReconnect);
}

MqttManager::~MqttManager()
{
    disconnectFromHost();
}

void MqttManager::connectToHost(const QString &host, quint16 port, const QString &username, const QString &password)
{
    _host = host;
    _port = port;
    _username = username;
    _password = password;

    _client->setHostname(_host);
    _client->setPort(_port);
    if (!_username.isEmpty()) {
        _client->setUsername(_username);
        _client->setPassword(_password);
    }
    _client->connectToHost();
}

void MqttManager::disconnectFromHost()
{
    _client->disconnectFromHost();
    _reconnectTimer->stop();
    _reconnectAttempts = 0;
}

QMqttSubscription* MqttManager::subscribe(const QString &topic)
{
    QMqttSubscription* subscription = _client->subscribe(QMqttTopicFilter(topic));
    return subscription;
}

void MqttManager::unsubscribe(const QString &topic, QObject *client)
{
    if (_subscriptions.contains(topic)) {
        _subscriptions[topic].removeAll(client);
        if (_subscriptions[topic].isEmpty()) {
            _client->unsubscribe(topic);
            _subscriptions.remove(topic);
        }
    }
}

void MqttManager::publish(const QString &topic, const QByteArray &payload, int qos, bool retain)
{
    _client->publish(topic, payload, qos, retain);
}

void MqttManager::onStateChanged(QMqttClient::ClientState state)
{
    if (state == QMqttClient::Connected) {
        emit connected();
        _reconnectAttempts = 0;
        _reconnectTimer->stop();
        // Re-subscribe to all topics (QtMqtt may persist if cleanSession false, but explicit for safety)
        for (const auto &topic : _subscriptions.keys()) {
            _client->subscribe(topic);
        }
    } else if (state == QMqttClient::Disconnected) {
        emit disconnected();
        if (_reconnectAttempts < MAX_RECONNECT_ATTEMPTS) {
            attemptReconnect();
        }
    }
}

void MqttManager::onMessageReceived(const QByteArray &payload, const QMqttTopicName &topicName)
{
    const QString topic = topicName.name();

    if (_subscriptions.contains(topic)) {
        // Option 1 (simple, recommended): Emit globally, let clients filter in their connected slots
        emit messageReceived(topic, payload);
    }
}

void MqttManager::onErrorChanged(QMqttClient::ClientError error)
{
    if (error != QMqttClient::NoError) {
        const QMetaEnum metaEnum = QMetaEnum::fromType<QMqttClient::ClientError>();
        const QString errorStr = QString::fromLatin1(metaEnum.valueToKey(static_cast<int>(error)));
        emit errorOccurred(errorStr);
    }
}

void MqttManager::attemptReconnect()
{
    ++_reconnectAttempts;
    int delay = RECONNECT_INTERVAL_MS * (1 << (_reconnectAttempts - 1));  // Exponential backoff: 5s, 10s, 20s, etc.
    _reconnectTimer->start(delay);
    _client->connectToHost();
}
