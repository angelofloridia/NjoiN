#include "symbol.h"



Symbol::Symbol(std::vector<int> posizione, int siteId, int counter)
{
    this->position = std::move(posizione);
    this->siteId = siteId;
    this->counter = counter;

}

Symbol::Symbol(QChar value, std::vector<int> posizione, int siteId, int counter)
{
    this->position = std::move(posizione);
    this->siteId = siteId;
    this->counter = counter;
    this->value = value;

}

Symbol::Symbol()
{

}
QChar Symbol::getValue() const
{
    return value;
}

void Symbol::setValue(QChar v)
{
    value = v;
}

int Symbol::getSiteId() const
{
    return siteId;
}

void Symbol::setSiteId(int value)
{
    siteId = value;
}

int Symbol::getCounter() const
{
    return counter;
}

void Symbol::setCounter(int value)
{
    counter = value;
}

std::vector<int> Symbol::getPosition() const
{
    return position;
}

void Symbol::setPosition(const std::vector<int> &value)
{
    position = value;
}

QString Symbol::getFamily()
{
    return family;
}

bool Symbol::getBold()
{
    return bold;
}

bool Symbol::getItalic()
{
    return italic;
}

bool Symbol::getUnderln()
{
    return underln;
}

qreal Symbol::getSize()
{
    return size;
}

void Symbol::setFamily(QString f)
{
    family=f;
}

void Symbol::setBold(bool b)
{
    bold=b;
}

void Symbol::setItalic(bool i)
{
    italic=i;
}

void Symbol::setUnderln(bool u)
{
    underln=u;
}

void Symbol::setSize(qreal s)
{
    size=s;
}

Symbol Symbol::fromJson(const QJsonObject &charJ){
//    ushort value = charJ["value"].toString().at(0).unicode();
    int value = charJ["value"].toInt();
    int siteId = charJ["siteId"].toInt();
    QJsonArray fractionalPosJ = charJ["position"].toArray();
    int counter = charJ["counter"].toInt();

    std::vector<int> fractionalPos;
    for(auto i: fractionalPosJ){
        fractionalPos.push_back(i.toInt());
    }
    /** TODO: non ancora implementato lo stile **/
//    QJsonObject styleJ = charJ["style"].toObject();
//    tStyle l_style = { styleJSON["fontFamily"].toString(), styleJSON["fontSize"].toInt(),
//                           styleJSON["bold"].toBool(), styleJSON["italic"].toBool(),
//                           styleJSON["underline"].toBool(), styleJSON["alignment"].toInt() };
    Symbol result(value, fractionalPos, siteId, counter);
    return result;
}
