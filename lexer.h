#ifndef LEXER_H
#define LEXER_H
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <stdexcept>
#include <unordered_map>
using namespace std;
class Lexer
{
public:
    Lexer();

    // metodo para padronizar as linhas
    string standardize(string linha);

    std::vector<std::pair<int, std::vector<std::string>>> tokenize(string line,  std::vector<std::pair<int, std::vector<std::string>>>& tokenizedLines);

    void showMap(const  std::vector<std::pair<int, std::vector<std::string>>>& mapa);

    bool isInteger(const std::string &s);
};

#endif // LEXER_H
