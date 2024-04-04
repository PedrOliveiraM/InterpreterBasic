#ifndef LEXER_H
#define LEXER_H
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <iostream>
using namespace std;
class Lexer
{
public:
    Lexer();

    // metodo para padronizar as linhas
    string standardize(string linha);
    map<int, vector<string>> tokenize(string line, map<int, vector<string>>& tokenizedLines);
    void showMap(const std::map<int, std::vector<std::string>>& mapa);
    bool isInteger(const std::string &s);
};

#endif // LEXER_H
