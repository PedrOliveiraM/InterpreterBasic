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
using namespace std;
class Parser
{
public:
    Parser();
    bool verificarOrdem(map<int, vector<string>>& tokens);
    bool verStruct(map<int, vector<string>> tokens);
};

#endif // PARSER_H
