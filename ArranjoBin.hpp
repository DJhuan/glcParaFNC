#ifndef ARRANJO_BIN
#define ARRANJO_BIN
#include <string>

using namespace std;

class ArranjoBin
{
    /* 
        Essa classe representa um número binário na forma de arranjo
        e é usada na remoção de variáveis anuláveis como uma combinação;
    */
private:
    int tamanho;
    bool *arranjo;

public:
    ArranjoBin(int tamanho);
    void somar_um();
    string stringficar();
};

#endif