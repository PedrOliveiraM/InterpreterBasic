#include "lexer.h"
#include <unordered_set>
Lexer::Lexer() {

}
string Lexer::standardize(std::string linha) {
    size_t pos = 0;
    std::string simbolos = ":*/-+()=";
    std::unordered_set<std::string> palavras_chave = {"INPUT", "PRINT", "REM", "IF", "THEN", "HALT", "GOTO"};

    // Verifica se o número da linha é um inteiro e coloca espaço após ele
    pos = linha.find_first_not_of("0123456789");
    if (pos != std::string::npos && pos > 0 && isspace(linha[pos])) {
        linha.insert(pos," ");
    }

    for (char simbolo : simbolos) {
        pos = 0;
        while ((pos = linha.find(simbolo, pos)) != std::string::npos) {
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
            size_t next_pos = linha.find_first_not_of("0123456789", pos + 1);
            if (next_pos != std::string::npos && next_pos > pos + 1 && isalpha(linha[next_pos])) {
                linha.insert(next_pos, " ");
            }
            pos = next_pos;
        } else {
            pos = linha.find_first_of("0123456789", pos + 1);
        }
    }

    // Adiciona espaço antes e depois de palavras-chave
    for (const std::string& palavra : palavras_chave) {
        pos = 0;
        while ((pos = linha.find(palavra, pos)) != std::string::npos) {
            if (pos > 0 && !isspace(linha[pos - 1])) {
                linha.insert(pos, " ");
                pos++;
            }
            size_t next_pos = pos + palavra.size();
            if (next_pos < linha.size() && !isspace(linha[next_pos])) {
                linha.insert(next_pos, " ");
            }
            pos = next_pos + 1;
        }
    }

    // Adiciona espaço após os números seguidos por uma letra
    pos = 0;
    while (pos < linha.size() - 1) {
        if (isdigit(linha[pos]) && isalpha(linha[pos + 1])) {
            linha.insert(pos + 1, " ");
        }
        pos++;
    }

    // Remove espaços extras
    pos = 0;
    while ((pos = linha.find("  ", pos)) != std::string::npos) {
        linha.erase(pos + 1, 1);
    }

    return linha;
}


std::vector<std::pair<int, std::vector<std::string>>> Lexer::tokenize(std::string line, std::vector<std::pair<int, std::vector<std::string>>>& tokenizedLines) {

    cout <<"entrou tokenize\n";
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

    tokenizedLines.push_back(std::make_pair(label, tokens));

    cout <<"SAIU\n";
    return tokenizedLines;
}


void Lexer::showMap(const std::vector<std::pair<int, std::vector<std::string>>> &mapa)
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

