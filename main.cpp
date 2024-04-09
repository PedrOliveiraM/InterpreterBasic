#include <iostream>
#include <fstream>
#include <string>
#include <vector>
// minhas libs
#include "lexer.h"
#include "parser.h"
#include "Semantic.h"

using namespace std;

Lexer lexer;
Parser *parser = new Parser;
Semantic *semantic = new Semantic;
vector<pair<int, vector<string>>> tokens;
vector<pair<int, vector<string>>> executaveis;

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
    cout <<"SAIU D0 WHILE";
    cout<<"\n-----* SAIDA *------\n";
    cout<<saida<<endl;

    cout<<"\n-----* VER TOKENS *------\n";
    lexer.showMap(tokens);

    cout<<"\n-----* DIVIDIR COMANDOS *------\n";
    // Adicione esta linha para criar um objeto Parser
    executaveis = parser->verificarComandos(tokens);

    cout<<"\n-----* VENDO AS VARIAVEIS *------\n";
    parser->mostrarVariaveis();

    cout<<"\n-----* EXECUTANDO *------\n";
    //semantic->armazenarVars(parser->getIntVariables(),parser->getFloatVariables(),parser->getStringVariables());
    semantic->interpretador(executaveis);
    //semantic->mostrarVariaveis();

    file.close();
    return 0;
}
