#include "crdt.h"
#include "symbol.h"
#include <iostream>
#include <QtDebug>

#define MAXNUM 100

bool exists(int index, std::vector<int> vector);
void resetVal();
int i=0, flag=0;
std::vector<int> tmp;

void print(std::vector<int> const &input)
{
    for (int i = 0; i < input.size(); i++) {
        //std::cout << input.at(i) << ' ';
        qDebug() << "pos: " << input.at(i);
    }
}


std::vector<int> createFractional(std::vector<int> preceding, std::vector<int> following){
    int prec, foll;
    //se esistono
    try {
       prec=preceding.at(i);

    } catch (const std::out_of_range) {
        prec=0;
        flag=1;
    }

    try {
        foll = following.at(i);

    } catch (const std::out_of_range) {
        foll = 0;
    }

    /*if(foll > prec){
        if(i==0){
            tmp.insert(tmp.begin(), prec);
            tmp.push_back((MAXNUM-preceding.back())/2+preceding.back());
            return tmp;
        }
        if(flag){
            tmp.push_back(rand() % foll + prec);
            i--;
            return tmp;
        }

        tmp.insert(tmp.begin(), prec);
        tmp.push_back((MAXNUM-preceding.back())/2+preceding.back());
        //tmp.push_back(rand() % foll + prec);
        i--;
        return tmp;
    }*/


    int diff=std::abs(foll-prec);

    if(diff > 1 ){
        tmp.push_back(diff/2+prec);
        return tmp;
    }
    else if(diff == 1){
            /*//e si ricorre
            i++;
            createFractional(preceding, following);*/
        if(i==0){
            tmp.insert(tmp.begin(), prec);
            tmp.push_back((MAXNUM-preceding.back())/2+preceding.back());
            return tmp;
        }
        if(flag){
            tmp.push_back(rand() % foll + prec);
            i--;
            return tmp;
        }

        tmp.insert(tmp.begin(), prec);
        tmp.push_back((MAXNUM-preceding.back())/2+preceding.back());
        //tmp.push_back(rand() % foll + prec);
        i--;
        return tmp;
        }
        else if(diff==0){
                i++;
                createFractional(preceding, following);
            }


    tmp.insert(tmp.begin(), prec);
    i--;
    return tmp;
}

Crdt::Crdt()
{

}

Symbol Crdt::localInsert(char value, int precedingC, int followingC){
    //mi da la dimensione del mio vettore di simboli
    //int symbolsSize = this.symbols.size();
    size_t symbolsSize = this->symbols.size();

    //prendo il simbolo nuovo
    //Symbol symbolToInsert(value, this->getSiteId(), this->getCounterAndIncrement());
    Symbol symbolToInsert(value, std::vector<int>{0}, this->getSiteId(), this->getCounter());

    std::vector<int> fractionalPos={0};

    if(precedingC==-1){
        qDebug() << "sono all'inserimento in testa";
        fractionalPos = std::vector<int>{MAXNUM/2};
        symbolToInsert.setPosizione(fractionalPos);
    }
    else{ //caso generale?
        if(followingC == (int)symbolsSize){ //è il secondo. no caso no elemtni a destra!
                std::vector<int> preceding = this->symbols[precedingC].getPosizione();
                std::vector<int> following{MAXNUM};
                //fractionalPos = createFractional(preceding, following);
                fractionalPos = {this->symbols.back().getPosizione()[0]+1};
                qDebug() << "fractionalpos: " << fractionalPos;
                symbolToInsert.setPosizione(fractionalPos);
                resetVal();
        }
        else{
            //mi salvo le posizioni frazionarie di quello prima e di quello dopo
            std::vector<int> preceding = this->symbols[precedingC].getPosizione();
            std::vector<int> following = this->symbols[followingC].getPosizione();
            fractionalPos = createFractional(preceding, following);
//            fractionalPos = createFractional({0,0}, {1,0});
//            resetVal();
//            fractionalPos = createFractional({0,0}, {0,1});
//            resetVal();
//            fractionalPos = createFractional({0,1}, {1,2});
//            resetVal();
//            fractionalPos = createFractional({0,0}, {0,0,3});
//            resetVal();
//            fractionalPos = createFractional({0,5}, {2,5});
//            resetVal();
//            fractionalPos = createFractional({0,0}, {0,0,0,3});
//            resetVal();

            qDebug() << "fractionalpos: " << fractionalPos;
            symbolToInsert.setPosizione(fractionalPos);
            resetVal();
        }
    }

    this->symbols.insert(this->symbols.begin()+followingC, symbolToInsert);

    print(fractionalPos);
    resetVal();
    return symbolToInsert;
}

//MAI USATA
bool exists(int index, std::vector<int> vector){
    if( index < static_cast<int>(vector.size()) )
        return true;
    else return false;
}

void resetVal(){
    i=0;
    tmp.clear();
    flag=0;
}


void Crdt::localErase(Symbol symbolToErase, int position){
    /*for( std::vector<Symbol>::iterator i = this->symbols.begin(); i!=this->symbols.end(); ++i){
        if(*i==symbolToErase)
            this->symbols.erase(i);
    }
    return;*/

    std::vector<Symbol>::iterator i = this->symbols.begin()+position;
    this->symbols.erase(i);
}

int Crdt::getSiteId(){
    return this->siteId;
}

int Crdt::getCounter(){
    return this->counter;
}

std::vector<Symbol> Crdt::getSymbols(){
    return this->symbols;
}

int Crdt::getCounterAndIncrement(){
    return ++this->counter;
}


//INIZIO PARTE LORENZO******************************************************//


std::vector<Symbol> Crdt::remoteinsert(std::vector<Symbol> vect, Symbol s){
    int min=0,max=vect.size()-1,middle=(max+min)/2,pos;
    std::vector<int> index=s.getPosizione();
    std::vector<int> tmp;
    std::vector<Symbol>::iterator it;
    //controllo se è ultimo
    if(vect[max].getPosizione().back()<index.front()){
        vect.push_back(s);
        return vect;
    }
    //controllo se è primo
    if(vect[0].getPosizione().front()>index.back()){
        it=vect.begin();
        vect.insert(it,s);
        return vect;
    }
    //è in mezzo
    while(max-min>1){
       pos=this->compare(s,vect[middle]);
       if(pos>0){
           min=middle;
       }
       else if(pos<0){
           max=middle;
       }
       middle=(max+min)/2;
    }
    it=vect.begin();
    pos=pos=this->compare(s,vect[min]);
    if(pos>0){
        //inserisco dopo il min
        vect.insert(it+min+1,s);
    }
    if(pos<0){
        //inserisco prima del min
        vect.insert(it+min-1,s);
    }
return vect;
}

std::vector<Symbol> Crdt::remotedelete(std::vector<Symbol> vect, Symbol s){
    int min=0,max=vect.size()-1,middle=(max+min)/2,pos;
    std::vector<int> index=s.getPosizione();
    std::vector<int> tmp;
    std::vector<Symbol>::iterator it;
    it=vect.begin();
    //controllo se è ultimo
    if(this->compare(s,vect[max])==0){
            vect.erase(it+max);
            return vect;
}
    //controllo se è primo
    if(this->compare(s,vect[min])==0){
            vect.erase(it+min);
            return vect;
        }
    while(max-min>1){
       pos=this->compare(s,vect[middle]);
       if(pos>0){
           min=middle;
       }
       else if(pos<0){
           max=middle;
       }
       if(pos==0){
           vect.erase(it+middle);
           break;
       }
       middle=(max+min)/2;
    }
    return vect;

}

int Crdt::compare(Symbol s1, Symbol s2){
    int len1=s1.getPosizione().size();
    int len2=s2.getPosizione().size();
    int res=0;
    for(int i=0;i<std::min(len1,len2);i++){

        if(s1.getPosizione()[i]>s2.getPosizione()[i]){
            res=1;
            break;
        }
        if(s1.getPosizione()[i]<s2.getPosizione()[i]){
            res=-1;
            break;
        }
    }
    if(res==0){
        if(len1>len2){
            res=1;
        }else
        if(len1<len2){
            res=-1;
        }

    }
    return res;




}
