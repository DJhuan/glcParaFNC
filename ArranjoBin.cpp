#include "ArranjoBin.hpp"

using namespace std;

ArranjoBin::ArranjoBin(int tam)
{
    tamanho = tam;
    arranjo = new bool[tam] {false};
}

void ArranjoBin::somar_um()
{
    bool somando = false;
    int i = tamanho - 1;

    do
    {
        if (arranjo[i])
        {
            arranjo[i] = false;
            somando = true;
        }
        else
        {
            arranjo[i] = true;
            somando = false;
        }
        i--;
    } while (somando and i >= 0);
}

string ArranjoBin::stringficar()
{
    string palavra = "";

    for (int i = 0; i < tamanho; i++)
    {

        if (arranjo[i])
            palavra += "V ";
        else
            palavra += "F ";
    }
    palavra += '\n';

    return palavra;
}
