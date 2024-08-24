#include "Glc.hpp"
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    Glc gramatica;
    gramatica.carregar_arquivo(argv[1]);
    gramatica.reach();
    //cout << gramatica.remover_regra("B", "bB");
    //cout << gramatica.remover_regra("B", "b");
    cout << gramatica.stringficar();
    

    return 0;
}