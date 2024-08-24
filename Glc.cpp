#include "Glc.hpp"
#include <algorithm>
#include <fstream>

using namespace std;

Glc::Glc() {}

void Glc::adicionar_regra(string var, string prod)
{
    regras[var].push_back(prod);
}

void Glc::nova_variavel(string var)
{
    regras[var];
    if (find(ordemRegras.begin(), ordemRegras.end(), var) == ordemRegras.end())
    {
        ordemRegras.push_back(var);
    }
}

void Glc::remover_regra(string var, string regra)
{
    vector<string> &vec = regras[var];
    vec.erase(remove(vec.begin(), vec.end(), regra), vec.end());
}

void Glc::remover_variavel(string var)
{
    regras.erase(var);
    ordemRegras.erase(remove(ordemRegras.begin(), ordemRegras.end(), var), ordemRegras.end());
}

void Glc::carregar_arquivo(string caminho)
{
    ifstream arqLeitura(caminho);

    for (string buffer; getline(arqLeitura, buffer);)
    {
        // pos armazena a posição em que começa ->
        string delimitador = " -> ";
        int pos = buffer.find(delimitador);

        // var armazena a variável à esquerda de ->
        string var = buffer.substr(0, pos);
        nova_variavel(var);
        buffer.erase(0, pos + delimitador.length());

        string regra;
        delimitador = " | ";
        while ((pos = buffer.find(delimitador)) != string::npos)
        {
            regra = buffer.substr(0, pos);
            adicionar_regra(var, regra);
            buffer.erase(0, pos + delimitador.length());
        }
        adicionar_regra(var, buffer);
    }
}

void Glc::escrever_arquivo(string caminho)
{
    ofstream arq(caminho);
    arq << stringficar();
}

string Glc::stringficar()
{
    // Transforma a gramática em string;
    string stringGramatica = "";

    for (int i = 0; i < ordemRegras.size(); i++)
    {
        stringGramatica += ordemRegras[i] + " -> ";

        // Cria uma renomeação (referência) temporária para o vetor dos produtos atuais;
        vector<string> &vetVar = regras[ordemRegras[i]];

        for (size_t i = 0; i < vetVar.size(); ++i)
        {
            stringGramatica += vetVar[i] + ' ';
            if (i != (vetVar.size() - 1))
            {
                stringGramatica += "| ";
            }
        }
        stringGramatica += '\n';
    }

    return stringGramatica;
}

void Glc::regras_cadeia()
{
    //Cria o chain
    unordered_map<string, vector<string>> chain;
    for (int i = 0; i < ordemRegras.size(); i++){
        //Cria o prev e o new
        string variavel = ordemRegras[i];
        chain[variavel].push_back(variavel);
        vector<string> Prev;
        vector<string> New;
        do{
            //new = chain[variavel] - prev
            New.clear();
            set_difference(chain[variavel].begin(), chain[variavel].end(), Prev.begin(), Prev.end(), back_inserter(New));
            Prev = chain[variavel];
            for (int j = 0; j < New.size(); j++){
                vector<string> &vetor = regras[New[j]];
                for (int k = 0; k < vetor.size(); k++){
                    if (vetor[k].size() == 1 and isupper(vetor[k][0])){
                        //Se for um caracter maiúsculo e não estiver no chain, adiciona no chain
                        if (find(chain[variavel].begin(), chain[variavel].end(), vetor[k]) == chain[variavel].end()) {
                            chain[variavel].push_back(vetor[k]);
                        }
                    } 
                }
            }
        }while (!equal(chain[variavel].begin(), chain[variavel].end(), Prev.begin()));
    }

    //Tira as regras de cadeia da gramática
    for (int i = 0; i < ordemRegras.size(); i++){
        string variavel = ordemRegras[i];
        for (int j = 0; j < chain[variavel].size(); j++){
            remover_regra(variavel, chain[variavel][j]);
        }
    }

    //Adiciona as regras das variáveis retiradas de cada variável
    for (int i = 0; i < ordemRegras.size(); i++){
        string variavel = ordemRegras[i];
        for (int j = 0; j < chain[variavel].size(); j++){
            string varNoChain = chain[variavel][j];
            for (int k = 0; k < regras[varNoChain].size(); k++){
                if (find(regras[variavel].begin(), regras[variavel].end(), regras[varNoChain][k]) == regras[variavel].end()) {
                    adicionar_regra(variavel, regras[varNoChain][k]);
                }
            }
        }
    }
}