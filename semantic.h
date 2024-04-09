#ifndef SEMANTIC_H
#define SEMANTIC_H
#include <iostream>
#include <map>
#include <vector>
#include <utility>
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <unordered_map>
using namespace std;
class Semantic
{
public:
    map<string, int> intVariables;
    map<string, float> floatVariables;
    map<string, string> stringVariables;

    Semantic();


    void interpretador(vector<pair<int, vector<string>>> executaveis);
    bool identificarComando(const std::vector<std::string>& comando,int proximaLinha);


    // EXECUTADORES

    bool execPRINT(const std::vector<string>& comando);
    bool execINPUT(const std::vector<string>& comando);
    bool execREM(const std::vector<string>& comando);
    int execIF(const std::vector<string>& comando,int label);
    int execGOTO(const std::vector<string>& comando, int label);
    bool execHALT(const std::vector<string>& comando);
    bool execATB(const vector<string> & tokenList);

    bool verificarGOTO(std::vector<std::string>& comando, int& proximaLinha, const std::vector<int>& ordemDeExecucao);
    bool verificarIF(std::vector<std::string>& comando, int& proximaLinha, const std::vector<int>& ordemDeExecucao);

    template<typename T ,typename T2>
    bool verificarComparacao(const T &var,const T2 &var2,string op);


    // PARA VARIAVEIS
    void armazenarVars(map<string, int> intVariablesExt,map<string, float> floatVariablesExt,map<string, string> stringVariablesExt);
    void mostrarVariaveis();
    string buscarVariavel(const std::string& variavel);

    bool verificaToken(const std::string& token);

    std::string converterParaTipo(const std::string& valor);
    bool existeVariavel(const std::string& variavel);
    bool criarVariavel(const std::string& variavel, const std::string& valor);
    bool modificarVariavel(const std::string& variavel, const std::string& novoValor);
};

#endif // SEMANTIC_H
