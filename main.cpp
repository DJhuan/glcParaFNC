#include "Glc.hpp"
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    Glc gramatica;
    gramatica.carregar_arquivo(argv[1]);
    gramatica.regras_cadeia();
    cout << gramatica.stringficar();

    return 0;
}