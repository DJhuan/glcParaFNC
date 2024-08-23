#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;



class glc {
private:
    // Simbolo inicial da gramática será sempre o primeiro item do vetor;
    unordered_map<string, vector<string>> regras;
    vector<string> ordemRegras;

public:
    glc();
    void nova_variavel(string var);
    void adicionar_regra(string var, string prod);
    void remover_regra(string var, string regra);
    void remover_variavel(string var);
    string stringficar();
};

glc::glc(){

}

void glc::adicionar_regra(string var, string prod){
    regras[var].push_back(prod);
}

void glc::nova_variavel(string var){
    regras[var];
    ordemRegras.push_back(var);
}

void glc::remover_regra(string var, string regra){
    vector<string>& vec = regras[var];
    vec.erase(remove(vec.begin(), vec.end(), regra), vec.end()); 
}

void glc::remover_variavel(string var){
    regras.erase(var);
    ordemRegras.erase(remove(ordemRegras.begin(), ordemRegras.end(), var), ordemRegras.end());
}

string glc::stringficar(){
    // Transforma a gramática em string;
    string stringGramatica = "";

    for (int i=0; i < ordemRegras.size(); i++){
        stringGramatica += ordemRegras[i] + " -> ";

        // Cria uma renomeação (referência) temporária para o vetor dos produtos atuais;
        vector<string>& vetVar = regras[ordemRegras[i]];

        for (size_t i = 0; i < vetVar.size(); ++i) {
            stringGramatica += vetVar[i] + ' ';
            if (i != (vetVar.size()-1)){
                stringGramatica += "| ";
            }
        }
        stringGramatica += '\n';
    }

    return stringGramatica;
}

int main(int argc, char *argv[]){

    glc gramatica;
    gramatica.nova_variavel("S");
    gramatica.adicionar_regra("S", "aS");
    gramatica.adicionar_regra("S", "bA");
    gramatica.nova_variavel("A");
    gramatica.adicionar_regra("A", "potato");
    gramatica.remover_variavel("S");
    cout << gramatica.stringficar() << endl;

    return 0;
}