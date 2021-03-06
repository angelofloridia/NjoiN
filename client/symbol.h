#ifndef SYMBOL_H
#define SYMBOL_H
#include <vector>
#include <QChar>
#include <QString>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

class Symbol
{
private:
    QChar value;
    std::vector<int> position;
    int siteId;
    int counter;

    /**informazioni del testo**/
    QString family;
    bool bold;
    bool italic;
    bool underln;
    qreal size;
    QChar align;
    /***********************/

public:
    QString getFamily();
    bool getBold();
    bool getItalic();
    bool getUnderln();
    qreal getSize();

    void setFamily(QString f);
    void setBold(bool b);
    void setItalic(bool i);
    void setUnderln(bool u);
    void setSize(qreal s);
    Symbol(QChar value, std::vector<int> position, int siteId, int counter);
    Symbol();
    QChar getValue() const;
    void setValue(QChar value);
    std::vector<int> getPosizione() const;
    void setPosizione(const std::vector<int> &value);
    int getSiteId() const;
    void setSiteId(int value);
    QChar getAlign();
    void setAlign(QChar);
    int getCounter() const;
    void setCounter(int value);
    static Symbol fromJson(const QJsonDocument &charJD);
    QJsonDocument toJson();
    bool operator < (const Symbol &other) const;
    bool operator!=(const Symbol &symbol){
        if(this->siteId!=symbol.siteId || this->value!=symbol.value || this->counter!=symbol.counter)
            return true;
        else return false;
    }
    bool operator==(const Symbol &symbol){
        if(this->siteId==symbol.siteId && this->value==symbol.value && this->counter==symbol.counter)
            return true;
        else return false;
    }
};

#endif // SYMBOL_H
