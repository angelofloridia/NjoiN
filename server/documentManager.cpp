#include "documentManager.h"

DocumentManager::DocumentManager(QObject *parent) : QObject(parent)
{

}

bool DocumentManager::saveToServer(QString documentId, QList<Symbol> &symbols)
{
    for(auto it : openDocuments){
        QFile file(documentId + ".bin");
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return false;
        QByteArray bArray;
        for(auto i : symbols){
            bArray.append(i.toJson().toBinaryData());
        }
        auto returnValue = file.write(bArray, bArray.size()); //salva nella cartella di build

        if(returnValue == -1)
            return false;
        else {
            file.close();
            qDebug() <<  file.fileName() << "saved";
            return true;
        }
    }
    return false;
}

bool DocumentManager::checkPermission(QString username, QString documentId)
{
    QList<SharedDocument>::iterator it;
    for(it = this->openDocuments.begin(); it!=this->openDocuments.end(); it++) {
        QString name = it->getName();
        QString creator = it->getCreator();
        if((name+'_'+creator) == documentId) {
            auto userAllowed = it->getUserAllowed();

            if(userAllowed.contains(username)){
                return true;
            }
        }
    }
    return false;
}

void DocumentManager::openDocument(SharedDocument &doc)
{
    this->openDocuments.push_back(doc);
}

void DocumentManager::closeDocument(QString uri)
{
    for (QList<SharedDocument>::iterator doc=openDocuments.begin(); doc != openDocuments.end(); ++doc){
        auto curDoc = doc->getUri();
        if(curDoc == uri){
            openDocuments.erase(doc);
        }
    }
}
