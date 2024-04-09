#ifndef PARSER_H
#define PARSER_H
#include <limits> // Adicione esta linha
#include <unordered_set> // Adicione esta linha
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include <map>
#include <algorithm>
#include <stdexcept>
#include <cctype> // para isdigit
#include <sstream> // para stringstream
using namespace std;
class Parser
{

public:
    map<string, int> intVariables;
    map<string, float> floatVariables;
    map<string, string> stringVariables;



    Parser();
    ~Parser();
    bool verificarLabels( vector<pair<int, vector<string>>>& tokens);
    vector<pair<int, vector<string>>> verificarComandos(vector<pair<int, vector<string>>> &tokens);
    bool identificarComando(const vector<string>& comando);
    void mostrarComando(const vector<string>& comando);

    // VERIFICAR STRUTURAS:
    bool structPRINT(const vector<string> & tokenList);
    bool structINPUT(const vector<string> & tokenList);
    bool structREM(const vector<string> & tokenList);
    bool structGOTO(const vector<string> & tokenList);
    bool structIF(const vector<string> & tokenList);
    void encontrarTokensAposTHEN(const vector<string>& tokenList);
    bool structHALT(const vector<string> & tokenList);
    bool structATRIBUICAO(const vector<string> & tokenList);
    void extracted(const vector<string> &tokenList);
    bool structARITMETICA(const vector<string> &tokenList);

    // PARA VARIAVEIS
    void armazenarValor(const string& variavel, const string& valor);
    bool isString(const string& valor);
    bool isInt(const string& valor);
    bool isFloat(const string& valor);
    string trimQuotes(const string& valor);
    bool isVariable(const string& str);
    bool isNumber(const string& str);

    // metodos para variaveis
    string buscarVariavel(const string& variavel);
    void mostrarVariaveis();

    // EXECUTAVEIS
    void imprimirComandosExecutaveis(const vector<pair<int, vector<string>>>& comandosExecutaveis);
    vector<pair<int, vector<string>>> criarComandosExecutaveis(const vector<pair<int, vector<string>>>& tokens);

    map<string, int> getIntVariables() const;
    map<string, float> getFloatVariables() const;
    map<string, string> getStringVariables() const;
};

#endif // PARSER_H
