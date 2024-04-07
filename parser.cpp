#include "parser.h"
Parser::Parser() {

}

bool Parser::verificarLabels( std::vector<std::pair<int, std::vector<std::string>>>& tokens)
{
    try {
        for (auto it = tokens.begin(); it != std::prev(tokens.end()); ++it) {
            auto it2 = it; // Inicialize it2 para começar da posição posterior a it
            ++it2; // Avança it2 para a próxima posição em relação a it

            int labelFixada = it->first;

            for (; it2 != tokens.end(); ++it2) {
                int proximaLabel = it2->first;

                if (labelFixada >= proximaLabel)
                    throw std::runtime_error("labelFixada é menor que proximaLabel");
            }
        }

        return false;
    } catch (const std::invalid_argument& e) {
        std::cerr << "Erro: " << e.what() << std::endl;
    }
    return false;
}

bool Parser::verificarComandos(std::vector<std::pair<int, std::vector<std::string>>> &tokens) {
    for (const auto& tokenPair : tokens) {
        const std::vector<std::string>& tokenList = tokenPair.second;
        std::vector<std::string> comando; // Para armazenar os tokens de cada comando
        for (const auto& token : tokenList) {
            if (token == ":") {
                // Se encontramos um delimitador de comando, mostramos o comando
                if (!comando.empty()) {
                    //mostrarComando(comando);
                    identificarComando(comando);
                    // Limpa o vetor de comando para o próximo comando
                    comando.clear();
                }
            } else {
                // Adiciona o token ao comando atual
                comando.push_back(token);
            }
        }
        // Verifica se há algum comando pendente após o término dos tokens
        if (!comando.empty()) {
            //mostrarComando(comando);
            identificarComando(comando);
        }
    }
    return true;
}

void Parser::mostrarComando(const std::vector<std::string>& comando) {
    std::cout << "Comando:";
    for (const auto& token : comando) {
        std::cout << " " << token;
    }
    std::cout << std::endl;
}

bool Parser::identificarComando(const std::vector<std::string>& comando) {
    if (comando.empty()) {
        std::cerr << "Erro: Comando vazio!" << std::endl;
        return false;
    }

    const std::string& primeiroToken = comando[0];
    const std::string& segundoToken = comando[1];
    if (primeiroToken == "PRINT") {
        if (!structPRINT(comando))
            throw "Erro na estrutura do comando PRINT";
    } else if (primeiroToken == "INPUT") {
        if (!structINPUT(comando))
            throw "Erro na estrutura do comando INPUT";
    } else if (primeiroToken == "HALT") {
        if (!structHALT(comando))
            throw "Erro na estrutura do comando HALT";
    } else if (primeiroToken == "IF") {
        if (!structIF(comando))
            throw "Erro na estrutura do comando IF";
    } else if (primeiroToken == "REM") {
        if (!structREM(comando))
            throw "Erro na estrutura do comando REM";
    } else if (primeiroToken == "GOTO") {
        if (!structGOTO(comando))
            throw "Erro na estrutura do comando GOTO";
    } else if (segundoToken == "=") {
        if (!structATRIBUICAO(comando))
            throw "Erro na estrutura do comando ATRIBUIR";
    }else {
        std::cerr << "Erro: Comando desconhecido!" << std::endl;
    }
    return false;
}

bool Parser::structATRIBUICAO(const std::vector<std::string>& tokenList) {
    std::cout << "# ATRIBUICAO # \n";
    //mostrarComando(tokenList);

    if (tokenList.size() >= 3) {
        const std::string& operador = tokenList[1];
        if (operador != "=") {
            std::cerr << "Erro: Operador de atribuição inválido." << std::endl;
            return false;
        }

        const std::string& variavel = tokenList[0];

        std::string valor;
        for (size_t i = 2; i < tokenList.size(); ++i) {
            if (tokenList[i] == ":") {
                std::cerr << "Erro: Atribuição inválida - dois pontos não são permitidos em uma atribuição." << std::endl;
                return false;
            }
            valor += tokenList[i];
            if (i != tokenList.size() - 1) {
                valor += " ";
            }
        }

        return true;
    } else {
        std::cerr << "Erro: Atribuição inválida." << std::endl;
        return false;
    }
}

bool Parser::structPRINT(const vector<string>  &tokenList)
{
    std::cout << "# PRINT # \n";

    return true;
}

bool Parser::structINPUT(const vector<string>  &tokenList)
{
    cout << "# INPUT # \n";
    //mostrarComando(tokenList);
    return true;
}

bool Parser::structREM(const vector<string>  &tokenList)
{
    cout << "# REM # \n";
    //mostrarComando(tokenList);
    return true;
}

bool Parser::structGOTO(const vector<string>  &tokenList)
{
    cout << "# GOTO # \n";
    //mostrarComando(tokenList);
    return true;
}

bool Parser::structIF(const vector<string>  &tokenList)
{
    cout << "# IF # \n";
    //mostrarComando(tokenList);
    return true;
}

bool Parser::structHALT(const vector<string> &tokenList)
{
    cout << "# HALT # \n";
    //mostrarComando(tokenList);
    return true;
}

void Parser::armazenarValor(const std::string& variavel, const std::string& valor) {
    // Verifica o tipo de valor e armazena na estrutura apropriada
    if (isString(valor)) {
        stringVariables[variavel] = trimQuotes(valor);
    } else if (isInt(valor)) {
        intVariables[variavel] = std::stoi(valor);
    } else if (isFloat(valor)) {
        floatVariables[variavel] = std::stof(valor);
    } else {
        // Se não for nem string, nem int, nem float, lança uma exceção
        throw std::invalid_argument("Erro: Tipo de valor inválido.");
    }
}

bool Parser::isString(const std::string& valor) {
    return (!valor.empty() && valor.front() == '"' && valor.back() == '"');
}

bool Parser::isInt(const std::string& valor) {
    for (char c : valor) {
        if (!std::isdigit(c)) {
            return false;
        }
    }
    return !valor.empty();
}

bool Parser::isFloat(const std::string& valor) {
    std::istringstream iss(valor);
    float f;
    iss >> std::noskipws >> f;
    return iss.eof() && !iss.fail();
}

std::string Parser::trimQuotes(const std::string& s) {
    if (s.size() >= 2 && s.front() == '"' && s.back() == '"') {
        return s.substr(1, s.size() - 2);
    }
    return s;
}

string Parser::buscarVariavel(const string &variavel)
{

    if (intVariables.count(variavel) > 0) {
        return std::to_string(intVariables[variavel]);
    } else if (floatVariables.count(variavel) > 0) {
        return std::to_string(floatVariables[variavel]);
    } else if (stringVariables.count(variavel) > 0) {
        return stringVariables[variavel];
    } else {
        return "Variável não encontrada";
    }
}

void Parser::mostrarVariaveis()
{
    std::cout << "Variaveis inteiras:\n";
    for (const auto& pair : intVariables) {
        std::cout << pair.first << " = " << pair.second << std::endl;
    }

    std::cout << "\nVariaveis de ponto flutuante:\n";
    for (const auto& pair : floatVariables) {
        std::cout << pair.first << " = " << pair.second << std::endl;
    }

    std::cout << "\nVariaveis de string:\n";
    for (const auto& pair : stringVariables) {
        std::cout << pair.first << " = " << pair.second << std::endl;
    }
}
