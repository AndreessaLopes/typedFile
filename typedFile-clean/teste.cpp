#include <iostream>
#include <clocale>
#include "intserial.h"

using namespace std;

int main(){
    intSerial teste = intSerial(100);
    intSerial teste2 = intSerial(100);
    intSerial somaAB= teste+teste2;
    intSerial teste3 = intSerial(12);

    cout<<"SOma de AB:"<<somaAB.getValue()<<endl;
    ++teste;
    cout<<"Incrementar"<<teste.getValue()  <<endl;
    --teste;
    cout<<"Decrementar"<<teste.getValue()  <<endl;

    cout<<"Teste:"<<teste.getValue()<<endl;
    cout<<"Teste2:"<<teste2.getValue()<<endl;
    teste3.fromString(teste.toString());
    cout<<teste3.getValue()<<endl;
    cout<<"tostring: "<< teste.toString();


    //intSerial *teste3= new intSerial();


    //cout<<"Incrementador:"<< teste->getValue()<<endl;

return 0;
}
