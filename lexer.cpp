#include "lexer.h"
#include <unordered_set>
Lexer::Lexer() {

}
string Lexer::standardize(string linha) {
    size_t pos = 0;
    string simbolos = ":*/-+()=";
    unordered_set<string> palavras_chave = {"INPUT", "PRINT", "REM", "IF", "THEN", "HALT", "GOTO"};

    // Verifica se o número da linha é um inteiro e coloca espaço após ele
    pos = linha.find_first_not_of("0123456789"); // Encontra a primeira posição que não é um dígito
    if (pos != string::npos && pos > 0 && isspace(linha[pos])) {
        linha.insert(pos, " ");
    }

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

    // Verifica se há um número de linha seguido de uma letra e coloca espaço entre eles, se necessário
    pos = 0;
    while (pos < linha.size()) {
        if (isdigit(linha[pos])) {
            size_t next_pos = linha.find_first_not_of("0123456789", pos + 1); // Encontra a primeira posição que não é um dígito após o número
            if (next_pos != string::npos && next_pos > pos + 1 && isalpha(linha[next_pos])) {
                linha.insert(next_pos, " ");
            }
            pos = next_pos;
        } else {
            pos = linha.find_first_of("0123456789", pos + 1); // Encontra a próxima posição que é um dígito
        }
    }

    // Adiciona espaço após palavras-chave
    for (const string& palavra : palavras_chave) {
        pos = 0;
        while ((pos = linha.find(palavra, pos)) != string::npos) {
            size_t next_pos = pos + palavra.size();
            if (next_pos < linha.size() && !isspace(linha[next_pos])) {
                linha.insert(next_pos, " ");
            }
            pos = next_pos + 1;
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

    std::string label_str;
    if (!(iss >> label_str)) {
        throw std::invalid_argument("Erro na leitura do label");
    }

    if (!isInteger(label_str))
        throw std::invalid_argument("Erro na leitura do label " + label_str);

    int label;
    try {
        label = stoi(label_str);
    } catch (const std::invalid_argument& e) {
        throw std::invalid_argument("Erro na conversão do label para inteiro");
    } catch (const std::out_of_range& e) {
        throw std::out_of_range("O label está fora do intervalo válido para int");
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

