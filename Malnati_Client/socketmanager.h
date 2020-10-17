#ifndef SOCKETMANAGER_H
#define SOCKETMANAGER_H

#include <QObject>
#include "QtWebSockets/qwebsocket.h"
#include "message.h"
#include <QDebug>
#include <string>
#include "string.h"
#include <QDataStream>
#include <QMessageBox>


class socketManager : public QObject
{
    Q_OBJECT    

public:
    explicit socketManager(const QUrl &url, QObject *parent = nullptr);
     ~socketManager() override;

signals:
    void newMessage(Message *m);
    void receivedLogin(bool resp);
    void receivedInfoAccount(Message&);
    void setSiteId(int);
    void receivedFile(QList<Symbol>);
    void receivedURIerror();
    void loggedin(bool);
    void receiveRegistration(Message);
    void showUsers(Message);

public slots:
    void receiveImage(QByteArray im);
    void onConnected();
    void onTextMessageReceived(QString message);
    void onBinaryMessageReceived(QByteArray bytemex); //Received binary message from server and emit a signal
    void binaryMessageToServer (Message* m);
    void messageToServer (Message* m);
    //


//
public:



private:
    QWebSocket webSocket;
    QUrl url;
    int i=100;
    int siteId;
    bool server_on=0;
};

#endif // SOCKETMANAGER_H
