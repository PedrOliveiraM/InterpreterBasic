#include <iostream>
#include <fstream>
#include <string>
#include <vector>
// minhas libs
#include "lexer.h"
#include "parser.h"
using namespace std;

Lexer lexer;
Parser parser;
std::vector<std::pair<int, std::vector<std::string>>> tokens;

// FUNÇOES ***************************************************************************************************

// MAIN ***************************************************************************************************
int main() {

    string saida = "";
    string linha;

    ifstream file("C:/compilador/interpreterBasic/basic.txt");

    if (!file.is_open()) {
        cerr << "Erro ao abrir o arquivo." << endl;
        return 1;
    }

    cout<<"\n-----* ENTRADA *------\n";

    while (getline(file, linha)) {
        cout << linha << endl; // mostrar linha de entrada
        linha = lexer.standardize(linha);

        saida += linha + "\n";
        tokens = lexer.tokenize(linha,tokens);
    }

    cout<<"\n-----* SAIDA *------\n";
    cout<<saida<<endl;

    cout<<"\n-----* VER TOKENS *------\n";
    lexer.showMap(tokens);

    cout<<"\n-----* DIVIDIR COMANDOS *------\n";
    parser.verificarComandos(tokens);

    file.close();
    return 0;
}


