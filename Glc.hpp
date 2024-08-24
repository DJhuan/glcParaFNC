#ifndef GLC_H
#define GLC_H

#include <unordered_map>
#include <vector>
#include <string>

using namespace std;

class Glc
{
private:
    // Simbolo inicial da gramática será sempre o primeiro item do vetor;
    unordered_map<string, vector<string>> regras;
    vector<string> ordemRegras;

public:
    Glc();
    void nova_variavel(string var);
    void adicionar_regra(string var, string prod);
    void remover_regra(string var, string regra);
    void remover_variavel(string var);
    string stringficar();
    void carregar_arquivo(string caminho);
    void escrever_arquivo(string caminho);
};

#endif // GLC_H
