#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDebug>
#include <QObject>
#include "mqttmanager.h"
#include "fridge.h"
#include "devicemanager.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    MqttManager mqttManager;
    DeviceManager deviceManager;

    mqttManager.connectToHost("test.mosquitto.org", 1883);
    Fridge *fridge = new Fridge(&mqttManager, "fridge");
    
    deviceManager.addDevice(fridge);

    QObject::connect(&mqttManager, &MqttManager::connected,
                     [fridge]() {
                         qDebug() << "MqttManager successfully connected to the broker.";
                         QVariantHash config;
                         config["hysteresis"] = 2;
                         config["setPoint"] = 20;
                         fridge->setConfig(config);
                     });

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("deviceManager", &deviceManager);
    
    const QUrl url(QStringLiteral("qrc:/qt/qml/smartHome/Main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}

// #include <QCoreApplication>
// #include <QMqttClient>
// #include <QMqttSubscription>
// #include <QTimer>
// #include <QDebug>

//     // What this test checks:
//     //   1. Connect to broker
//     //   2. Subscribe to a topic and receive at least one message
//     //   3. Force a disconnect
//     //   4. Reconnect WITHOUT calling subscribe() again
//     //   5. Check if the QMqttSubscription* still delivers messages
//     //
//     // Run a publisher in a separate terminal:
//     //   mosquitto_pub -h localhost -t "test/reconnect" -m "hello" -r -l
//     // The -r flag retains the message so it's delivered immediately on (re)connect.
//     //
//     // Expected outcomes:
//     //   A) Messages arrive after reconnect  -> Qt preserves the subscription object,
//     //                                          no resubscribe logic needed in MqttManager
//     //   B) No messages after reconnect      -> subscription silently dies on disconnect,
//     //                                          resubscribeAll() is required
//     //   C) Crash / Qt object-deleted warning -> subscription object is destroyed,
//     //                                           resubscribeAll() is definitely required

//     int main(int argc, char *argv[])
// {
//     QCoreApplication app(argc, argv);

//     QMqttClient client;
//     client.setHostname("localhost"); // change to your broker
//     client.setPort(1883);

//     QMqttSubscription *sub = nullptr;
//     int messageCount = 0;

//     auto onMessage = [&](const QMqttMessage &msg) {
//         ++messageCount;
//         qDebug() << "[sub signal] message #" << messageCount
//                  << "| topic:" << msg.topic().name()
//                  << "| payload:" << msg.payload()
//                  << "| sub state:" << (sub ? sub->state() : -1);
//     };

//     // Declared before the connected lambda so the [&] capture can see it
//     QTimer disconnectTimer;
//     disconnectTimer.setSingleShot(true);
//     QObject::connect(&disconnectTimer, &QTimer::timeout, [&]() {
//         qDebug() << "\n--- forcing disconnect to test subscription survival ---";
//         client.disconnectFromHost();

//         QTimer::singleShot(2000, &app, [&]() {
//             qDebug() << "--- reconnecting ---";
//             client.connectToHost();
//         });
//     });

//     QObject::connect(&client, &QMqttClient::connected, [&]() {
//         qDebug() << "[client] connected";

//         if (!sub) {
//             // First connection: subscribe and start the disconnect timer
//             sub = client.subscribe(QMqttTopicFilter("test/reconnect"), 0);
//             if (sub) {
//                 qDebug() << "[client] subscribed | sub ptr:" << sub
//                          << "| state:" << sub->state();
//                 QObject::connect(sub, &QMqttSubscription::messageReceived,
//                                  &app, onMessage);
//             } else {
//                 qDebug() << "[client] subscribe() returned nullptr!";
//             }
//             disconnectTimer.start(3000);
//         } else {
//             // Reconnect: intentionally NOT calling subscribe() again
//             qDebug() << "[client] reconnected - NOT resubscribing"
//                      << "| sub ptr:" << sub
//                      << "| sub state:" << sub->state();
//         }
//     });

//     QObject::connect(&client, &QMqttClient::disconnected, [&]() {
//         qDebug() << "[client] disconnected"
//                  << "| sub ptr:" << sub
//                  << "| sub state:" << (sub ? sub->state() : -1);
//     });

//     QObject::connect(&client, &QMqttClient::stateChanged,
//                      [](QMqttClient::ClientState s) {
//                          qDebug() << "[client] state ->" << s;
//                      });

//     // 12s total: ~1s connect, 3s observe, 2s reconnect, ~6s observe after reconnect
//     QTimer::singleShot(12000, &app, [&]() {
//         qDebug() << "\n=== RESULT ===";
//         qDebug() << "Total messages received:" << messageCount;
//         if (sub) qDebug() << "Sub state at exit:" << sub->state();
//         if (messageCount == 0) {
//             qDebug() << "No messages at all - check broker and publisher";
//         } else {
//             qDebug() << "Did messages arrive AFTER the 'reconnecting' line?";
//             qDebug() << "  YES -> subscription survives reconnect, no resubscribe needed";
//             qDebug() << "  NO  -> subscription dies on disconnect, resubscribeAll() required";
//         }
//         app.quit();
//     });

//     client.connectToHost();

//     return app.exec();
// }
