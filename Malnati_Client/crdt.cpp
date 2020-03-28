#include "crdt.h"
#include "symbol.h"
#include <iostream>
#include <QtDebug>

#define MAXNUM 100

bool exists(int index, std::vector<int> vector);
void resetVal();
int i=0, flag=0;
int level=1;
std::vector<int> tmp;

void print(std::vector<int> const &input)
{
    for (size_t i = 0; i < input.size(); i++) {
        //std::cout << input.at(i) << ' ';
        qDebug() << "pos: " << input.at(i);
    }
}


std::vector<int> createFractional(std::vector<int> preceding, std::vector<int> following){
    int prec, foll;

    /*try {
       prec=preceding.at(i);

    } catch (const std::out_of_range) {
        prec=0;
        flag=1;
    }

    try {
        foll = following.at(i);

    } catch (const std::out_of_range) {
        foll = 0;
    }*/

    prec=preceding.at(0);
    foll=following.at(0);

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

    /*if(diff==0){ // si ricorre
        i++;
        //createFractional(preceding, following);

    }

    if(diff==1){                       //primo caso di terminazione
        if ( i>0 ){
            tmp.push_back(prec);                //
            //tmp.push_back((MAXNUM-preceding.back())/2+preceding.back()); //bisogna verificare che non esista già
            tmp.push_back((MAXNUM-preceding.back())/2+preceding.back()); //bisogna verificare che non esista già
            level*=2;
            return tmp;
        }
        else if(i==0){
            tmp=preceding;
            tmp.push_back(MAXNUM/2);
            return tmp;
        }
    }
    else if(diff>1){
        tmp.push_back(prec+(foll-prec)/2); //media tra i 2, anche questo bisogna verificare che non esista already
        return tmp;
    }*/

    if(diff==1 || diff==0){
        if(preceding.size()==1){
            preceding.at(0)=0;
        }
        else{
            preceding.erase(preceding.begin());
        }
        if(following.size()==1){
            following.at(0)=MAXNUM;
        }
        else{
            following.erase(following.begin());
        }

        createFractional(preceding, following);
    }
    else if(diff > 1){
        tmp.push_back(prec+(foll-prec)/2);
        return tmp;
    }

    tmp.insert(tmp.begin(), prec);          //inserimento a ritroso
    i--; //maybe can be avoided
    return tmp;
}

Crdt::Crdt()
{

}

Symbol Crdt::localInsert(char value, int precedingC, int followingC){
    //mi da la dimensione del mio vettore di simboli
    //int symbolsSize = this.symbols.size();
    size_t symbolsSize = this->symbols.size();

    std::vector<int> following;
    std::vector<int> preceding;
    std::vector<int> fractionalPos={0};

    //prendo il simbolo nuovo
    Symbol symbolToInsert(value, std::vector<int>{0}, this->getSiteId(), this->getCounter());

    if(precedingC==-1){
        qDebug() << "sono all'inserimento in testa";

        if(symbolsSize==0){ //primissimo inserimento
            fractionalPos = std::vector<int>{MAXNUM/2};
        }
        else{
            following = this->symbols[followingC].getPosizione();
            fractionalPos = createFractional({0}, following);
        }

        symbolToInsert.setPosizione(fractionalPos);
    }
    else{
        if(followingC == (int)symbolsSize){         // inserimento in coda
                preceding = this->symbols[precedingC].getPosizione();
                following=std::vector<int>{MAXNUM};
                /*****************Da TESTARE***************************/
                fractionalPos = createFractional(preceding, following);
                /*****************************************************/
                qDebug() << "fractionalpos: " << fractionalPos;
                symbolToInsert.setPosizione(fractionalPos);
                resetVal();
        }
        else{ // cioè sono in mezzo
            //mi salvo le posizioni frazionarie di quello prima e di quello dopo
            preceding = this->symbols[precedingC].getPosizione();
            following = this->symbols[followingC].getPosizione();
            fractionalPos = createFractional(preceding, following);
//            fractionalPos = createFractional({3,99}, {4,5});
//            resetVal();
//            fractionalPos = createFractional({0,1}, {0,1,1});
//            resetVal();
//            fractionalPos = createFractional({0,2}, {0,2,1});
//            resetVal();
//            fractionalPos = createFractional({0,0}, {0,0,1});
//            resetVal();
//            fractionalPos = createFractional({0,0}, {0,0,50});
//            resetVal();
//            fractionalPos = createFractional({0,3,4,5}, {0,5,3,27});
//            resetVal();
//            fractionalPos = createFractional({0,3,1}, {0,3,2});
//            resetVal();
//            fractionalPos = createFractional({0,0}, {1,0});
//            resetVal();
//            fractionalPos = createFractional({0}, {0,2});
//            resetVal();
//            fractionalPos = createFractional({0,1}, {1,2});
//            resetVal();
//            fractionalPos = createFractional({0,0}, {0,0,3});
//            resetVal();
//            fractionalPos = createFractional({0,5}, {2,5});
//            resetVal();
//            fractionalPos = createFractional({0,0}, {0,0,0,3});
//            resetVal();
//            fractionalPos = createFractional({2, 3, 4}, {3}); //{2,50}
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


int Crdt::remoteinsert(Symbol s){
    int min=0,max=symbols.size()-1,middle=(max+min)/2,pos;
    std::vector<int> index=s.getPosizione();
    std::vector<int> tmp;
    std::vector<Symbol>::iterator it;
    //controllo se è ultimo
    if(symbols[max].getPosizione().back()<index.front()){
        symbols.push_back(s);
        return max;
    }
    //controllo se è primo
    if(symbols[0].getPosizione().front()>index.back()){
        it=symbols.begin();
        symbols.insert(it,s);
        return min;
    }
    //è in mezzo
    while(max-min>1){
       pos=this->compare(s,symbols[middle]);
       if(pos>0){
           min=middle;
       }
       else if(pos<0){
           max=middle;
       }
       middle=(max+min)/2;
    }
    it=symbols.begin();
    pos=this->compare(s,symbols[min]);
    if(pos>0){
        //inserisco dopo il min
        symbols.insert(it+min+1,s);
        return min+1;
    }
    if(pos<0){
        //inserisco prima del min
        symbols.insert(it+min-1,s);
        return min-1;
    }

}

int Crdt::remotedelete(Symbol s){
    int min=0,max=symbols.size()-1,middle=(max+min)/2,pos;
    std::vector<int> index=s.getPosizione();
    std::vector<int> tmp;
    std::vector<Symbol>::iterator it;
    it=symbols.begin();
    //controllo se è ultimo
    if(this->compare(s,symbols[max])==0){
            symbols.erase(it+max);
            return max;
}
    //controllo se è primo
    if(this->compare(s,symbols[min])==0){
            symbols.erase(it+min);
            return min;
        }
    while(max-min>1){
       pos=this->compare(s,symbols[middle]);
       if(pos>0){
           min=middle;
       }
       else if(pos<0){
           max=middle;
       }
       if(pos==0){
           symbols.erase(it+middle);
           break;
       }
       middle=(max+min)/2;
    }
    return middle;

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

//if(diff > 1 ){
//    tmp=preceding;
//    //tmp.push_back(diff/2+prec);
//    tmp.at(tmp.size()-1)=(MAXNUM-preceding.back())/2+preceding.back();
//    return tmp;
//}
//else if(diff == 1){
//        /*//e si ricorre
//        i++;
//        createFractional(preceding, following);*/
//    if(i==0){
//        //tmp.insert(tmp.begin(), prec);
//        //tmp.push_back((MAXNUM-preceding.back())/2+preceding.back());
//        tmp=preceding;
//        tmp.at(tmp.size()-1)=(MAXNUM-preceding.back())/2+preceding.back();
//        return tmp;
//    }
//    else{
//        //tmp.push_back(MAXNUM/2);
//    }
//    if(flag){
//        tmp.push_back(rand() % foll + prec);
//        i--;
//        return tmp;
//    }

//    tmp.insert(tmp.begin(), prec);
//    tmp.push_back((MAXNUM-preceding.back())/2+preceding.back());
//    //tmp.push_back(rand() % foll + prec);
//    i--;
//    return tmp;
//    }
//    else if(diff==0){
//            i++;
//            createFractional(preceding, following);
//        }


//tmp.insert(tmp.begin(), prec);
//i--;
//return tmp;
//}
