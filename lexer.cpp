#include "lexer.h"

Lexer::Lexer() {

}

string Lexer::standardize(string linha)
{
    size_t pos = 0;
    string simbolos = ":*/-+()=";
    for (char simbolo : simbolos) {
        pos = 0;
        while ((pos = linha.find(simbolo, pos)) != string::npos) {
            if (pos > 0 && linha[pos - 1] != ' ') {
                linha.insert(pos, " ");
                pos++;
            }
            if (pos + 1 < linha.size() && linha[pos + 1] != ' ') {
                linha.insert(pos + 1, " ");
                pos += 2;
            }
            pos++;
        }
    }

    pos = 0;
    while ((pos = linha.find("  ", pos)) != string::npos) {
        linha.erase(pos + 1, 1);
    }

    return linha;
}

map<int, vector<string>> Lexer::tokenize(string line, map<int, vector<string>>& tokenizedLines) {
    std::istringstream iss(line);
    int label;
    if (!(iss >> label) || !iss.eof()) {
        throw std::invalid_argument("Erro na leitura do label");
    }

    std::string token;
    std::vector<std::string> tokens;
    while (iss >> token) {
        tokens.push_back(token);
    }
    tokenizedLines[label] = tokens;

    return tokenizedLines;
}



void Lexer::showMap(const std::map<int, std::vector<string> > &mapa)
{
    for (const auto& par : mapa) {
        std::cout << "[" << par.first<<"] ";
        for (const auto& token : par.second) {
            std::cout << token << " ";
        }
        std::cout << std::endl;
    }
}

bool Lexer::isInteger(const string &s)
{
    if(s.empty()) // Verifica se a string está vazia
        return false;

    // Verifica cada caractere da string para garantir que seja um dígito
    for(char c : s) {
        if(!isdigit(c)) // Se o caractere não for um dígito, retorna falso
            return false;
    }

    return true; // Se todos os caracteres forem dígitos, retorna verdadeiro
}

