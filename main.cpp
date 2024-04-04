#include <iostream>
#include <fstream>
#include <sstream>
#include <lexer.h>
#include <sstream>
#include <string>
#include <vector>
#include <map>
using namespace std;

Lexer lexer;
map<int, vector<string>> tokens;


// FUNÇOES ***************************************************************************************************

// MAIN ***************************************************************************************************
int main() {
    ifstream file("C:/Users/pedro.monteiro/Documents/InterpreterBasic/basic.txt");
    if (!file.is_open()) {
        cerr << "Erro ao abrir o arquivo." << endl;
        return 1;
    }

    string linha;
    while (getline(file, linha)) {
        cout << linha << endl; // mostrar linha de entrada
        linha = lexer.standardize(linha);
        cout << linha << endl; // mostrar linha apos a padronização
        tokens = lexer.tokenize(linha,tokens);
    }

    lexer.showMap(tokens);

    //mostrar
    cout<<"\nACESSANDO TOKEN ESPECIFICIO: \n";
    cout<<"tamanho: "<<tokens[14].size();

    std::string token = tokens[14][1];
    cout<<"\nToken: " << token<<endl;


    file.close();
    return 0;
}


