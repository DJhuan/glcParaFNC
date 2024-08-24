#include "Glc.hpp"
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{

    Glc gramatica;
    gramatica.carregar_arquivo(argv[1]);
    cout << gramatica.stringficar() << endl;

    return 0;
}