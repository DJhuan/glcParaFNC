#include "Glc.hpp"
#include "ArranjoBin.hpp"
#include <algorithm>
#include <fstream>
#include <cctype>
#include <numeric>

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
    reach.push_back(ordemRegras[0]); // O símbolo inicial sempre será alcançável
    vector<string> Prev;
    vector<string> New;

    do
    {
        New.clear();
        // new = reach-prev
        set_difference(reach.begin(), reach.end(), Prev.begin(), Prev.end(), back_inserter(New));
        Prev = reach;
        // Pega as variáveis que estão no new
        for (int i = 0; i < New.size(); i++)
        {
            string variavel = New[i];
            // Pega cada w produzido pelas variáveis
            for (int j = 0; j < regras[variavel].size(); j++)
            {
                string w = regras[variavel][j];
                for (int k = 0; k < w.size(); k++)
                {
                    if (isupper(w[k]))
                    {
                        // Converte w(char) em string
                        string wEmString(1, w[k]);
                        // Procura a variável no reach
                        if (find(reach.begin(), reach.end(), wEmString) == reach.end())
                        {
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

    for (int i = 0; i < inalcancaveis.size(); i++)
    {
        // Apaga a variável e as regras produzidas por ela
        remover_variavel(inalcancaveis[i]);
        for (int j = 0; j < ordemRegras.size(); j++)
        {
            string variavel = ordemRegras[j];
            for (int k = 0; k < regras[variavel].size(); k++)
            {
                string w = regras[variavel][k];
                for (int z = 0; z < w.size(); z++)
                {
                    string wEmString(1, w[k]);
                    // Remove todas as regras que contêm a variável removida
                    if (wEmString == inalcancaveis[i])
                    {
                        remover_regra(variavel, w);
                    }
                }
            }
        }
    }
}

string Glc::capitalizar_regra(string regra, set<string> &capitalizados)
{
    /*
        Transforma todos os terminais de uma regra em variáveis,
        elimina a regra antiga, em caso de mudança e adiciona a
        variável nova na gramática.
    */
    bool foiModificada = false;
    string novaRegra = "";

    if (regra.length() == 1 && islower(regra[0]))
        return novaRegra;

    for (char c : regra)
    {
        // Caso seja um terminal, precisamos modificá-lo
        if (islower(c))
        {
            foiModificada = true;
            string cZao = string(1, toupper(c)) + "'";
            // A nova regra é a capitalização de c com adição de "'".
            novaRegra += cZao;

            // Busca se a regra já não existe antes de adicionar na gramática.
            if (capitalizados.find(novaRegra) == capitalizados.end())
            {
                capitalizados.insert(cZao);
                adicionar_regra(novaRegra, string(1, c));
            }
        }
        else
        {
            novaRegra += c;
        }
    }

    return novaRegra;
}

vector<string> Glc::separar_regra(string regra)
{
    /*
        Separa a regra em seus componentes já que mais de um caracter pode
        ser usado para representar uma variável.
        A função entende que um componente é:
            Um caracter minúsculo
            Um caracter maiúsculo seguido por zero ou mais
                - Caracteres numéricos {0..9}
                - Aspas simples {'} (apenas uma é admitida)
    */
    vector<string> compRegra;
    string componente = "";
    int i = 0;
    auto tam = regra.length();

    while (i < tam)
    {
        char c = regra[i];

        if ((c >= 'a' && c <= 'z') || (c == '.'))
        {
            // If it's a terminal, add it directly as a component
            compRegra.push_back(string(1, c));
            i++;
        }
        else if (c >= 'A' && c <= 'Z')
        {
            componente += c;
            i++;

            // Verifica se a variável é seguida por números
            while (i < tam && ((regra[i] >= '0' && regra[i] <= '9') || (regra[i] == '\'')))
            {
                componente += regra[i];
                i++;
            }

            compRegra.push_back(componente);
            componente = "";
        }
        else
        {
            i++;
        }
    }

    return compRegra;
}

string Glc::agrupar_dois(vector<string> &vetor, unordered_map<string, string> &jaAgrupados)
{
    if (vetor.size() <= 2)
    {
        // Se o vetor tem 2 ou menos itens retorna ele como está.
        return vetor[0] + (vetor.size() > 1 ? vetor[1] : "");
    }

    string ultimoT;
    int tamVet = vetor.size();
    while (tamVet > 2)
    {
        string ultimo = vetor.back();
        vetor.pop_back();
        string penultimo = vetor.back();
        vetor.pop_back();

        string novoGrupo = penultimo + ultimo;

        // Verifica se o par já não foi agrupado antes
        if (jaAgrupados.find(novoGrupo) != jaAgrupados.end())
        {
            vetor.push_back(jaAgrupados[novoGrupo]);
        }
        else
        {
            // Nova variável Tnº.
            string variavelT = "T" + to_string(num_t++);
            ultimoT = variavelT;

            // Adiciona o novo par agrupado.
            jaAgrupados[novoGrupo] = variavelT;
            nova_variavel(variavelT); // Verifique se essa função está corretamente implementada
            adicionar_regra(variavelT, novoGrupo); // Verifique se essa função está corretamente implementada

            vetor.push_back(jaAgrupados[novoGrupo]);
        }
        tamVet--;
    }

    return vetor[0] + vetor[1];
}

void Glc::paraFNC()
{
    // Set para guardar as novas variáveis já criadas
    set<string> novasVariaveis;
    // Loop entre as variáveis
    for (auto &variavel : ordemRegras)
    {
        vector<string> &vetVar = regras[variavel];

        // Loop entre as regras
        for (auto iterador = vetVar.begin(); iterador != vetVar.end(); iterador++)
        {
            string regra = *iterador;
            string capit = capitalizar_regra(regra, novasVariaveis);

            // Se a regra possuir um ou mais terminais ela é substituida
            if (capit != "")
            {
                remover_regra(variavel, regra);
                iterador = vetVar.insert(iterador, capit);
            }
        }
    }

    for (auto &novaVar : novasVariaveis)
    {
        nova_variavel(novaVar);
        adicionar_regra(novaVar, string(1, tolower(novaVar[0])));
    }

    unordered_map<string, string> jaAgrupados;
    int i = 0;
    while (i < ordemRegras.size())
    {
        vector<string> &vetVar = regras[ordemRegras[i]];
        vector<string> novasRegras;

        for (auto iterador = vetVar.begin(); iterador != vetVar.end();)
        {
            string regra = *iterador;
            vector<string> regraSeparada = separar_regra(regra);

            if (regraSeparada.size() > 2)
            {
                string regraReduzida = agrupar_dois(regraSeparada, jaAgrupados);
                iterador = vetVar.erase(iterador);
                iterador = vetVar.insert(iterador, regraReduzida);
            }
            else
            {
                ++iterador;
            }
        }
        i++;
    }
}

// Later removal ==================

void Glc::print()
{
}

// Later removal ==================