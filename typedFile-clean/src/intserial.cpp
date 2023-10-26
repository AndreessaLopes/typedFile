#include "intserial.h"

intSerial::intSerial() : serializable()   //construtor
{
    value = 0;
}

intSerial::intSerial(int i) : serializable()   //construtor parametrizado
{
    value = i;
}

intSerial::intSerial(const intSerial &other)   //construtor de c�pia
{
    value = other.getValue();
}

intSerial::~intSerial()   //destrutor
{
    //nao precisa implementar
}

intSerial intSerial::operator =(const intSerial &other)   //operador de = (recebe)
{
    intSerial aux(other); // copy p to aux
    if (this == &other)
        return *this;
    value = other.getValue();
    return aux;
}

bool intSerial::operator==(const intSerial &other) const   //operador de == (igual)
{
    return value == other.getValue();
}

bool intSerial::operator<(const intSerial &other) const   //operador de < (menor)
{
    return value < other.getValue();
}

bool intSerial::operator<=(const intSerial &other) const   //operador de <= (menor igual)
{
    return value <= other.getValue();
}

bool intSerial::operator>(const intSerial &other) const   //operador de > (maior)
{
    return value > other.getValue();
}

bool intSerial::operator>=(const intSerial &other) const   // operador de >= (maior igual)
{
    return value >= other.getValue();
}

bool intSerial::operator!=(const intSerial &other) const   //operador de != (diferente)
{
    return value != other.getValue();
}

intSerial intSerial::operator+(const intSerial& other) const   //operador de + (soma)
{
    return value + other.getValue();
}

intSerial intSerial::operator-(const intSerial& other) const   //operador de - (subtra��o)
{
    return value - other.getValue();
}

intSerial intSerial::operator*(const intSerial& other) const   //operador de * (multiplica��o)
{
    return value * other.getValue();
}

intSerial intSerial::operator/(const intSerial& other) const   //operador de / (divis�o)
{
    return value / other.getValue();
}

intSerial intSerial::operator++()   //operador de ++ (incrementador)
{
    intSerial aux(value);
    value ++;
    return aux;
}

intSerial intSerial::operator--()   //operador de -- (subtrator)
{
    intSerial aux(value);
    value --;
    return aux;
}

void intSerial::setValue(int v)   // seta o valor
{
    value = v;
}

int intSerial::getValue() const   //pega o valor
{
    return value;
}

string intSerial::toString()
{
    string aux = "";
    aux += string(reinterpret_cast<char*>(&value), sizeof(value));
    return aux;
}

void intSerial::fromString(string repr)
{
    int pos = 0;
    repr.copy(reinterpret_cast<char*>(&value),sizeof(int),pos);
}

string intSerial::toXML()
{

}

void intSerial::fromXML(string repr)
{

}

string intSerial::toCSV()
{

}

void intSerial::fromCSV(string repr)
{

}

string intSerial::toJSON()
{

}

void intSerial::fromJSON(string repr)
{

}

unsigned long long int intSerial::size() const
{
    return sizeof(value);
}
