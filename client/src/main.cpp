#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "include/client.h"
#include <QIcon>
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    Client client;
    app.setWindowIcon(QIcon(":/icon.jpg"));
    engine.rootContext()->setContextProperty("client", &client);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
