#ifndef TYPEDFILE_H
#define TYPEDFILE_H

#include <fstream>
#include <type_traits>
using namespace std;

const ios::openmode mode = ios::in | ios::out | ios::binary;

template <class T>
class typedFile : private fstream
{
    static_assert(is_base_of<serializable, T>::value, "T must be serializable");
public:
    typedFile();
    typedFile(const string name, const string type, const unsigned int version, ios::openmode openmode = mode);
    virtual ~typedFile();
    bool open(const string name, const string type, const unsigned int version, ios::openmode openmode = mode);
    bool isOpen();
    bool close();
    void clear();
    bool readRecord(record<T> &r, unsigned long long int i);
    bool writeRecord(record<T> &r, unsigned long long int i);
    bool insertRecord(record<T> &r);
    bool deleteRecord(unsigned long long int i);
    unsigned long long int getFirstValid();
    unsigned long long int getFirstDeleted();
    unsigned long long int search(T data);
protected:
    header head;
    bool readHeader(header &h);
    bool writeHeader(header &h);
    unsigned long long int index2pos(unsigned long long int i);
    unsigned long long int pos2index(unsigned long long int p);
};

template <class T>
typedFile<T>::typedFile() : fstream()  //construtor
{
    head = ""; //atributo
}

template <class T>
typedFile<T>::typedFile(const string name, const string type, const unsigned int version, ios::openmode openmode) : fstream(name.c_str(), mode)
{
    if(isOpen()){
        readHeader(head);
    }
    else{
        open(name,type,version,openmode); //se quiser pode passar o mode
        head.setType(type);
        head.setVersion(version);
        writeHeader(head);
    }
}

template <class T>
typedFile<T>::~typedFile() // Fecha o arquivo se estiver aberto
{
    if(isOpen())
    {
        close();
    }
}

template <class T>
bool typedFile<T>::open(const string name, const string type, const unsigned int version, ios::openmode openmode)
{
    if(isOpen())
    {
        return true;

    }
    else
    {
        fstream::open(name,ios::out); //abre o arquivo em modo de escrita
        fstream::close(); // fecha o arquivo
        fstream::open(name,openmode); // ja esta dentro do escopo
    }
}

template <class T>
bool typedFile<T>::isOpen()
{
    return fstream::is_open(); //encapsulamento
}

template <class T>
bool typedFile<T>::close()
{
    fstream::close();
    return true; //encapsulamento
}

template <class T>
void typedFile<T>::clear()
{
    fstream::clear(); //encapsulamento
}

template <class T>
bool typedFile<T>::readRecord(record<T> &r, unsigned long long int i)
{
    if(isOpen())
    {
        char *vetChar = new char [head.size()]; //23 bytes
        seekp(index2pos(i));
        read(vetChar,r.size()); //topico 3
        r.fromString(string(vetChar,r.size()));
        return true; // conversao do vetChar pra String e essa String desserializa e guarda num registro r
    }
    else{
        return false;
    }
}

template <class T>
bool typedFile<T>::writeRecord(record<T> &r, unsigned long long int i)
{
    if (isOpen())
    {
        seekp(index2pos(i));
        write(r.toString().c_str(), r.size());
        return fstream::good();
    }
    else
    {
        return false;
    }
}

template <class T>
bool typedFile<T>::insertRecord(record<T> &r)
{
    bool success = false;


// Encontrar a posição para inserir o novo registro
unsigned long long int positionToInsert;
if (this->head.getFirstDeleted() == 0) {
    fstream::seekg(0, ios::end);
    positionToInsert = fstream::tellg();
} else {
    positionToInsert = this->head.getFirstDeleted();
    this->head.setFirstDeleted(r.getNext());
}

// Definir o próximo ponteiro do novo registro para o primeiro válido
r.setNext(this->getFirstValid());

cout << "Valor no Registro : " << r.getData().getValue() << endl;

// Escrever o novo registro na posição positionToInsert
if (writeRecord(r, pos2index(positionToInsert))) {
    // Atualizar o ponteiro do primeiro válido no cabeçalho
    this->head.setFirstValid(pos2index(positionToInsert));

    // Escrever o cabeçalho atualizado no arquivo
    if (writeHeader(this->head)) {
        success = true;
    }
}

return success;
}
//topico 3 indice 3




template <class T>
bool typedFile<T>::deleteRecord(unsigned long long int i)
{
 record<T> r;

 // Verifique se o registro atual existe
 if (!readRecord(r, i)) {
     cout << "O registro " << i << " não existe." << endl;
     return false;
 }

 // Caso o registro seja o primeiro válido
 if (i == this->head.getFirstValid()) {
     this->head.setFirstValid(r.getNext());
 } else {
     // Procure o registro anterior ao registro a ser excluído
     unsigned long long int anteriorI = 0;
     unsigned long long int mexendoI = this->head.getFirstValid();

     while (mexendoI != 0) {
         readRecord(r, mexendoI);

         if (r.getNext() == i) {
             // Marque o registro anterior como excluído
             r.setNext(0);

             if (writeRecord(r,mexendoI)) {
                 break; // Excluímos o registro, saia do loop
             } else {
                 cout << "Erro ao escrever o registro " << mexendoI << endl;
                 return false;
             }
         }

         anteriorI = mexendoI;
         mexendoI = r.getNext();
     }
 }

 // Agora o registro foi excluído logicamente, você pode marcar espaço como "vazio" se desejar
 // Se você deseja marcar o registro como excluído fisicamente, pode adicionar a lógica aqui

 return true;
}

    //topico 3 indice 3


template <class T>
unsigned long long int typedFile<T>::getFirstValid()
{
    return head.getFirstValid(); //retorna o index do primeiro valido
}

template <class T>
unsigned long long int typedFile<T>::getFirstDeleted()
{
    return head.getFirstDeleted(); //retorna o index do primeiro deletado
}

template <class T>
unsigned long long int typedFile<T>::search(T data)
{
    unsigned long long int searchValue = this->head.getFirstValid();
unsigned long long int foundIndex = 0;
record<T> currentRecord;

while (searchValue != 0) {
    readRecord(currentRecord, searchValue);

    if (T(currentRecord.getData().getValue()) == data) {
        foundIndex = searchValue;
        break; // Encontrou o valor, podemos parar a busca
    }

    searchValue = currentRecord.getNext();
}

return foundIndex;
    //carregar um registro e verificar se e igual ao data, voce retorna o index que ta, se nao voce pega o  proximo e repete(laco de repeticao, se nao voce retorna -1
    //voce nunca faz busca nos deletados, comeca no index do primeiro valido
}

template <class T>
bool typedFile<T>::readHeader(header &h)
{
      if(isOpen())
    {
        char *vetChar = new char [h.size()]; //23 bytes
        fstream::seekg(0, ios::beg);
        fstream::read(vetChar,h.size()); //topico 3
        h.fromString(string(vetChar,h.size()));
        return true; // conversao do vetChar pra String e essa String desserializa e guarda num registro r
    }
    else{
        return false;
    }
}

template <class T>
bool typedFile<T>::writeHeader(header &h)
{
    if (isOpen())
    {
        fstream::seekp(0, ios::beg);
        fstream::write(h.toString().c_str(), h.size());
        return fstream::good();
    }
    else
    {
        return false;
    }
}

template <class T>
unsigned long long int typedFile<T>::index2pos(unsigned long long int i)
{
     record<T> rec;
     return (head.size() + (i-1) * rec.size());
}

template <class T>
unsigned long long int typedFile<T>::pos2index(unsigned long long int p)
{
    record<T> rec;
    return (p - head.size()) / rec.size() + 1;
}
#endif // TYPEDFILE_H
