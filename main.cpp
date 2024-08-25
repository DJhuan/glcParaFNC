#include "Glc.hpp"
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    Glc gramatica;
    if (argc < 2)
    {
        cerr << "Não foi especificado o caminho para oarquivo de entrada!" << endl;
        return 1;
    } else if (argc > 3)
    {
        cerr << "Não foi especificado o caminho para o arquivo de saída!" << endl;
        return 2;
    }
    gramatica.carregar_arquivo(argv[1]);
    gramatica.paraFNC();
    cout << gramatica.stringficar();

    return 0;
}