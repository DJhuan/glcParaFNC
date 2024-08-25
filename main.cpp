#include "Glc.hpp"
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    Glc gramatica;

    gramatica.carregar_arquivo(argv[1]);
    gramatica.remover_recursividade_inicial();
    gramatica.eliminar_lambdas();
    gramatica.regras_cadeia();
    gramatica.term();
    gramatica.reach();
    gramatica.paraFNC();
    gramatica.escrever_arquivo(argv[2]);

    return 0;
}