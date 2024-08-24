#include "Glc.hpp"
#include <algorithm>
#include <fstream>

using namespace std;

Glc::Glc() {}

bool Glc::eh_regra_anulavel(string regra, set<string> &anulaveis)
{
    if (regra == ".")
        return true;

    for (int i = 0; i < regra.length(); i++)
    {
        // A variável ponteiro recebe a posição do item buscado
        auto ponteiro = anulaveis.find(string(1, regra[i]));
        /*
            Se o ponteiro estiver apontando para o final do set, significa
            que ele não foi encontrado, logo, a regra não é completamente
            anulável.
        */
        if (ponteiro == anulaveis.end())
        {
            return false;
        }
    }

    return true;
}

set<string> Glc::encontrar_anulaveis()
{
    set<string> anulaveis;

    int tamAnulaveis = anulaveis.size();
    int tamAnuAntigo = -1;
    while (tamAnulaveis != tamAnuAntigo)
    {
        // Enquanto forem descobertos novos atuláveis, busca-se por novos
        // Para cada variável que existe
        for (int i = 0; i < ordemRegras.size(); i++)
        {
            vector<string> &vetVar = regras[ordemRegras[i]];
            bool encontrado = false;
            /*
                Buscamos dentro das suas regras se:
                    É possível encontrar lambda ('.')
                    É possível que a regra se anule
                Caso uma anulavel seja encontrada, paramos o loop.
            */
            for (int j = 0; j < (vetVar.size()) && !encontrado; ++j)
            {
                if (eh_regra_anulavel(vetVar[j], anulaveis))
                {
                    anulaveis.insert(ordemRegras[i]);
                    encontrado = true;
                }
            }
        }
        tamAnuAntigo = tamAnulaveis;
        tamAnulaveis = anulaveis.size();
    }

    return anulaveis;
}

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

        // regra armazena uma das regas da V atual
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

void Glc::eliminar_lambdas()
{
}

void Glc::regras_cadeia()
{
    // Cria o chain
    unordered_map<string, vector<string>> chain;
    for (int i = 0; i < ordemRegras.size(); i++)
    {
        // Cria o prev e o new
        string variavel = ordemRegras[i];
        chain[variavel].push_back(variavel);
        vector<string> Prev;
        vector<string> New;
        do
        {
            // new = chain[variavel] - prev
            New.clear();
            set_difference(chain[variavel].begin(), chain[variavel].end(), Prev.begin(), Prev.end(), back_inserter(New));
            Prev = chain[variavel];
            for (int j = 0; j < New.size(); j++)
            {
                vector<string> &vetor = regras[New[j]];
                for (int k = 0; k < vetor.size(); k++)
                {
                    if (vetor[k].size() == 1 and isupper(vetor[k][0]))
                    {
                        // Se for um caracter maiúsculo e não estiver no chain, adiciona no chain
                        if (find(chain[variavel].begin(), chain[variavel].end(), vetor[k]) == chain[variavel].end())
                        {
                            chain[variavel].push_back(vetor[k]);
                        }
                    }
                }
            }
        } while (!equal(chain[variavel].begin(), chain[variavel].end(), Prev.begin()));
    }

    // Tira as regras de cadeia da gramática
    for (int i = 0; i < ordemRegras.size(); i++)
    {
        string variavel = ordemRegras[i];
        for (int j = 0; j < chain[variavel].size(); j++)
        {
            remover_regra(variavel, chain[variavel][j]);
        }
    }

    // Adiciona as regras das variáveis retiradas de cada variável
    for (int i = 0; i < ordemRegras.size(); i++)
    {
        string variavel = ordemRegras[i];
        for (int j = 0; j < chain[variavel].size(); j++)
        {
            string varNoChain = chain[variavel][j];
            for (int k = 0; k < regras[varNoChain].size(); k++)
            {
                if (find(regras[variavel].begin(), regras[variavel].end(), regras[varNoChain][k]) == regras[variavel].end())
                {
                    adicionar_regra(variavel, regras[varNoChain][k]);
                }
            }
        }
    }
    
}

void Glc::reach()
{
    // Cria o reach, o prev e o new
    vector<string> reach;
    reach.push_back(ordemRegras[0]); //O símbolo inicial sempre será alcançável
    vector<string> Prev;
    vector<string> New;

    do{
        New.clear();
        // new = reach-prev
        set_difference(reach.begin(), reach.end(), Prev.begin(), Prev.end(), back_inserter(New));
        Prev = reach;
        //Pega as variáveis que estão no new
        for (int i = 0; i < New.size(); i++){
            string variavel = New[i];
            //Pega cada w produzido pelas variáveis
            for (int j = 0; j < regras[variavel].size(); j++){
                string w = regras[variavel][j];
                for (int k = 0; k < w.size(); k++){
                    if (isupper(w[k])){
                        // Converte w(char) em string
                        string wEmString (1, w[k]);
                        //Procura a variável no reach
                        if (find(reach.begin(), reach.end(), wEmString) == reach.end()){
                            reach.push_back(wEmString);
                        }
                    }
                }
            }
        }
    } while (!equal(reach.begin(), reach.end(), Prev.begin()));

    // Cria o vetor das variáveis inalcançáveis
    vector<string> inalcancaveis;
    set_difference(ordemRegras.begin(), ordemRegras.end(), reach.begin(), reach.end(), back_inserter(inalcancaveis));

    for (int i = 0; i < inalcancaveis.size(); i++){
        // Apaga a variável e as regras produzidas por ela
        remover_variavel(inalcancaveis[i]);
        for (int j = 0; j < ordemRegras.size(); j++){
            string variavel = ordemRegras[j];
            for (int k = 0; k < regras[variavel].size(); k++){
                string w = regras[variavel][k];
                for (int z = 0; z < w.size(); z++){
                    string wEmString(1, w[k]);
                    // Remove todas as regras que contêm a variável removida
                    if (wEmString == inalcancaveis[i]){
                        remover_regra(variavel, w);
                    }
                }
            }
        }
    }
}

// Later removal ==================

void Glc::print()
{
    set<string> a;
    a.insert("A");
    a.insert("B");
    if (eh_regra_anulavel("AB", a))
        cout << "1SIM\n";
    if (eh_regra_anulavel("A", a))
        cout << "2SIM\n";
    if (eh_regra_anulavel("CA", a))
        cout << "3SIM\n";
}

// Later removal ==================