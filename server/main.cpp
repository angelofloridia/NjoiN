#include <QCoreApplication>
#include <QtCore/QCommandLineParser>
#include <QDebug>
#include<QDataStream>

#include "server.h"
#include "iostream"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QCommandLineParser parser;
    parser.setApplicationDescription("QtServer textEditor");
    parser.addHelpOption();

    QCommandLineOption dbgOption(QStringList() << "d" << "debug",
                                 QCoreApplication::translate("main", "Debug output [default: off]."));
    parser.addOption(dbgOption);
    QCommandLineOption portOption(QStringList() << "p" << "port",
                                  QCoreApplication::translate("main", "Port for echoserver [default: 1234]."),
                                  QCoreApplication::translate("main", "port"), QLatin1String("1234"));
    parser.addOption(portOption);
    parser.process(a);
    int port = parser.value(portOption).toInt();

    qDebug()<<"porta:"<<port;

    Server server;
    QObject::connect(&server, &Server::closed, &a, &QCoreApplication::quit);

    return a.exec();
}
