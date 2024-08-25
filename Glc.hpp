#ifndef GLC_H
#define GLC_H

#include <unordered_map>
#include <vector>
#include <string>
#include <set>

// remover
#include <iostream>
// remover

using namespace std;

class Glc
{
private:
    // Simbolo inicial da gramática será sempre o primeiro item do vetor;
    int num_t = 0;
    unordered_map<string, vector<string>> regras;
    vector<string> ordemRegras;
    static bool eh_regra_anulavel(string regra, set<string> &anulaveis);
    set<string> encontrar_anulaveis();
    set<string> *produzir_variantes(string regra, vector<int> posicoes, set<string> &anulaveis);
    string capitalizar_regra(string regra, set<string> &capitalizados);
    vector<string> separar_regra(string regra);
    string agrupar_dois(vector<string> &vetor, unordered_map<string, string> &jaAgrupados);

public:
    Glc();
    void nova_variavel(string var);
    void adicionar_regra(string var, string prod);
    void remover_regra(string var, string regra);
    void remover_variavel(string var);
    string stringficar();
    void carregar_arquivo(string caminho);
    void escrever_arquivo(string caminho);
    void remover_recursividade_inicial();
    void eliminar_lambdas();
    void print();
    void regras_cadeia();
    void reach();
    void paraFNC();
};

#endif // GLC_H
