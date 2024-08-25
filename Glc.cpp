#include "Glc.hpp"
#include "ArranjoBin.hpp"
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
    // Adiciona uma regra ao unordered_map se ela não existir.
    if (find(regras[var].begin(), regras[var].end(), prod) == regras[var].end())
    {
        regras[var].push_back(prod);
    }
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
    auto pos = find(regras[var].begin(), regras[var].end(), regra);

    if (pos != regras[var].end())
    {
        regras[var].erase(pos);
    }
}

void Glc::remover_variavel(string var)
{
    regras.erase(var);
    auto pos = find(ordemRegras.begin(), ordemRegras.end(), var);

    if (pos != ordemRegras.end())
    {
        ordemRegras.erase(pos);
    }
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

void Glc::remover_recursividade_inicial() 
{
    string variavel_inicial = ordemRegras[0]; // Supondo que o símbolo inicial é sempre o primeiro

    // Checa se a variável inicial possui recursividade
    vector<string> producoes_inicial = regras[variavel_inicial];
    bool tem_recursividade = false;

    for (int i = 0; i < producoes_inicial.size(); ++i) {
        if (producoes_inicial[i].find(variavel_inicial) != string::npos) {
            tem_recursividade = true;
        }
    }

    if (tem_recursividade) {
        string nova_var = variavel_inicial + "'";
        
        // Adiciona a nova variável à lista de variáveis e regras
        nova_variavel(nova_var);  
        regras[nova_var].push_back(variavel_inicial);  // Regra S' -> S

        // Atualiza a ordem das variáveis para inserir a nova variável inicial (S') no início
        ordemRegras.insert(ordemRegras.begin(), nova_var); // Insere a nova variável antes do símbolo inicial original

        // Remove a nova variável do final da lista, se estiver presente
        ordemRegras.erase(remove(ordemRegras.begin() + 1, ordemRegras.end(), nova_var), ordemRegras.end());
    }
}

void Glc::eliminar_lambdas()
{
    set<string> anulaveis = encontrar_anulaveis();
    anulaveis.insert(".");

    for (int i = 0; i < ordemRegras.size(); i++)
    {
        vector<string> &vetVar = regras[ordemRegras[i]];

        vector<string> novasRegras;
        for (auto &regra : vetVar)
        {
            int tamRegra = regra.size();
            vector<int> posicoesRemoviveis;
            for (int j = 0; j < tamRegra; j++)
            {
                // Verifica se há caracteres anuláveis e regista a posição deles
                if (anulaveis.find(string(1, regra[j])) != anulaveis.end())
                {
                    posicoesRemoviveis.push_back(j);
                }
            }

            // Caso haja alguma edição que possa ser feita
            if (!posicoesRemoviveis.empty())
            {
                // Produz todas as variantes sem lambda
                set<string> *variantes = produzir_variantes(regra, posicoesRemoviveis, anulaveis);

                for (const string &variante : *variantes)
                {
                    // Verifica todas as regras que são novas
                    if (find(vetVar.begin(), vetVar.end(), variante) == vetVar.end())
                    {
                        novasRegras.push_back(variante);
                    }
                }
                delete variantes;
            }
        }

        // Remove geras anulaveis e lambda exceto no inicial

        if (i == 0)
        {
            // Adiciona todas as regras novas no símbolo inicial
            for (const string &novaRegra : novasRegras)
            {
                vetVar.push_back(novaRegra);
            }
        }
        else
        {
            // Adiciona regras novas nos símbolos não iniciais
            for (auto iterador = vetVar.begin(); iterador != vetVar.end();)
            {
                if (*iterador == ".")
                {
                    iterador = vetVar.erase(iterador);
                }
                else
                    iterador++;
            }
            // Adiciona todas as regras novas
            for (const string &novaRegra : novasRegras)
            {
                if (novaRegra != ".")
                    vetVar.push_back(novaRegra);
            }
        }
    }
}

set<string> *Glc::produzir_variantes(string regra, vector<int> posicoes, set<string> &anulaveis)
{
    set<string> *variacoes = new set<string>;

    ArranjoBin combinacao(posicoes.size());

    string copiaRegra;
    for (int i = 0; i < (1 << posicoes.size()); i++)
    {
        copiaRegra = regra;

        int k = 0; // k ajuda a ajustar o offset das remoções
        for (int j = 0; j < posicoes.size(); j++)
        {
            if (combinacao.arranjo[j])
            {
                copiaRegra.erase(posicoes[j] - k, 1);
                k++;
            }
        }

        if (copiaRegra != "")
        {
            variacoes->insert(copiaRegra);
        }
        else
            variacoes->insert(".");

        combinacao.somar_um();
    }

    return variacoes;
}


void Glc::regras_cadeia()
{
    // Cria o chain
    unordered_map<string, vector<string>> chain;
    for (const auto& variavel: ordemRegras)
    {
        // Cria o prev e o new
        chain[variavel].push_back(variavel);
        vector<string> Prev;
        vector<string> New;
        do
        {
            // new = chain[variavel] - prev
            New.clear();
            set_difference(chain[variavel].begin(), chain[variavel].end(), Prev.begin(), Prev.end(), back_inserter(New));
            Prev = chain[variavel];
            for (const auto& varNoNew: New)
            {
                vector<string> &vetor = regras[varNoNew];
                for (const auto& regra: vetor)
                {
                    if (regra.size() == 1 and isupper(regra[0]))
                    {
                        // Se for um caracter maiúsculo e não estiver no chain, adiciona no chain
                        if (find(chain[variavel].begin(), chain[variavel].end(), regra) == chain[variavel].end())
                        {
                            chain[variavel].push_back(regra);
                        }
                    }
                }
            }
        } while (!equal(chain[variavel].begin(), chain[variavel].end(), Prev.begin()));
    }

    // Tira as regras de cadeia da gramática
    for (const auto& variavel: ordemRegras)
    {
        for (const auto& regra: chain[variavel])
        {
            remover_regra(variavel, regra);
        }
    }

    // Adiciona as regras das variáveis retiradas de cada variável
    for (const auto& variavel: ordemRegras)
    {
        for (const auto& varNoChain: chain[variavel])
        {
            for (const auto& regra: regras[varNoChain])
            {
                if (find(regras[variavel].begin(), regras[variavel].end(), regra) == regras[variavel].end())
                {
                    adicionar_regra(variavel, regra);
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

    do
    {
        New.clear();
        // new = reach-prev
        set_difference(reach.begin(), reach.end(), Prev.begin(), Prev.end(), back_inserter(New));
        Prev = reach;

        // Pega as variáveis que estão no new
        for (const auto& variavel: New)
        {
            // Pega cada regra produzida pelas variáveis
            for (const auto& regra: regras[variavel])
            {
                for (const auto& caracter: regra)
                {
                    if (isupper(caracter))
                    {
                        // Converte caracter em string
                        string charEmString (1, caracter);

                        //Procura a variável no reach
                        if (find(reach.begin(), reach.end(), charEmString) == reach.end())
                        {
                            reach.push_back(charEmString);
                        }
                    }
                }
            }
        }
    } while (!equal(reach.begin(), reach.end(), Prev.begin()));

    // Cria o vetor das variáveis inalcançáveis
    vector<string> inalcancaveis;
    set_difference(ordemRegras.begin(), ordemRegras.end(), reach.begin(), reach.end(), back_inserter(inalcancaveis));

    for (const auto& varInalc: inalcancaveis)
    {
        // Apaga a variável e as regras produzidas por ela
        remover_variavel(varInalc);

        for (const auto& variavel: ordemRegras)
        {
            for (const auto& regra: regras[variavel])
            {
                for (const auto& caracter: regra)
                {
                    string charEmString(1, caracter);

                    // Remove todas as regras que contêm a variável removida
                    if (charEmString == varInalc)
                    {
                        remover_regra(variavel, charEmString);
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