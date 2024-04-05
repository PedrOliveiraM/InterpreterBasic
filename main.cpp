#include <iostream>
#include <fstream>
#include <sstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
// minhas libs
#include <lexer.h>
#include <parser.h>
using namespace std;

Lexer lexer;
Parser parser;
map<int, vector<string>> tokens;


// FUNÇOES ***************************************************************************************************

// MAIN ***************************************************************************************************
int main() {

    string saida = "";
    ifstream file("C:/Users/pedro.monteiro/Documents/InterpreterBasic/basic.txt");
    if (!file.is_open()) {
        cerr << "Erro ao abrir o arquivo." << endl;
        return 1;
    }

    string linha;
    while (getline(file, linha)) {
        cout << linha << endl; // mostrar linha de entrada
        linha = lexer.standardize(linha);

        saida += linha + "\n";
        tokens = lexer.tokenize(linha,tokens);
    }

    parser.verStruct(tokens);


    cout<<"\n-----* SAIDA *------\n";
    cout<<saida<<endl;;

    file.close();
    return 0;
}


