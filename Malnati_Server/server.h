#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <map>
#include <queue>
#include "symbol.h"
#include "sharedDocument.h"
#include "socketManager.h"
#include "databaseManager.h"
#include "accountManager.h"
#include "filemanager.h"
#include "account.h"
#include "message.h"


class SharedDocument;
//class Account;
class Message;

class Server: public QObject
{
Q_OBJECT

private:
    //std::map<std::string, std::vector<Symbol>> documents;     in questo caso il file andrebbe aggiornato anche lato server (ottimizzazione futura = OF)
    //std::map<int, Account> onlineAccounts;
    //std::queue<Message> codaMessaggi;             Questo sarebbe il modo classico di gestirlo in c++, provare a usare invece signal e slots per gestire gli eventi
    std::unique_ptr<SocketManager> socketMan;
//    SocketManager *socketMan;
    std::unique_ptr<DatabaseManager> dbMan;
//    DatabaseManager *dbMan;
    std::unique_ptr<FileManager> fileMan;

    AccountManager *acMan;

    int remoteInsert(Symbol symbol);
    int remoteDelete(Symbol symbol);

    std::vector<Symbol> symbols;
    void dispatchMessage(Message &mes);          //capisci a quali client inviare il messaggi


signals:

    void openFileFromDB (std::string nome);                      // chiedi al DB manager di recuperare il file dal DB
    void closed();  //TODO: decidere quando emettere questo segnale

public slots:
    void processMessage(Message mes);
    //void tryInLocal (std::string);              //controlla se il file è tra quelli nella memoria locale del server (in documents) OF

public:
    explicit Server(QObject *parent = nullptr);

};

#endif // SERVER_H
