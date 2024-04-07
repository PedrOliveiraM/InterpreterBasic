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
    std::map<std::string, int> intVariables;
    std::map<std::string, float> floatVariables;
    std::map<std::string, std::string> stringVariables;

    Parser();
    bool verificarLabels( vector<pair<int, vector<string>>>& tokens);
    bool verificarComandos(std::vector<std::pair<int, std::vector<std::string>>> &tokens);
    bool identificarComando(const std::vector<std::string>& comando);
    void mostrarComando(const std::vector<std::string>& comando);



    // VERIFICAR STRUTURAS:
    bool structPRINT(const vector<string> & tokenList);
    bool structINPUT(const vector<string> & tokenList);
    bool structREM(const vector<string> & tokenList);
    bool structGOTO(const vector<string> & tokenList);
    bool structIF(const vector<string> & tokenList);
    bool structHALT(const vector<string> & tokenList);
    bool structATRIBUICAO(const vector<string> & tokenList);


    // PARA VARIAVEIS
    void armazenarValor(const std::string& variavel, const std::string& valor);
    bool isString(const std::string& valor);
    bool isInt(const std::string& valor);
    bool isFloat(const std::string& valor);
    string trimQuotes(const std::string& valor);

    // metodos para variaveis
    string buscarVariavel(const std::string& variavel);
    void mostrarVariaveis();


};

#endif // PARSER_H
