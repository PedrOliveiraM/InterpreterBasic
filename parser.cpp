#include "parser.h"
map<string, int> Parser::getIntVariables() const
{
    return intVariables;
}

map<string, float> Parser::getFloatVariables() const
{
    return floatVariables;
}

map<string, string> Parser::getStringVariables() const
{
    return stringVariables;
}

Parser::Parser() {

}

Parser::~Parser()
{
    // Destrutor vazio
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

std::vector<std::pair<int, std::vector<std::string>>> Parser::verificarComandos(std::vector<std::pair<int, std::vector<std::string>>> &tokens) {

    verificarLabels(tokens);
    for (const auto& tokenPair : tokens) {
        const std::vector<std::string>& tokenList = tokenPair.second;
        std::vector<std::string> comando; // Para armazenar os tokens de cada comando
        for (const auto& token : tokenList) {
            if (token == ":") {
                // Se encontramos um delimitador de comando
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
    return criarComandosExecutaveis(tokens);
    //mostrarVariaveis();
}

void Parser::mostrarComando(const std::vector<std::string>& comando) {
    //std::cout << "Comando:";
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
    } else if (segundoToken == "=" && comando.size() >= 5) {
        if (!structARITMETICA(comando))
            throw "Erro na estrutura do comando OPE ARIT";
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

        if (valor.front() == '"' && valor.back() == '"') {
            stringVariables[variavel] = valor;
        } else {
            // Verifique se tem ponto decimal, se tiver é float
            if (valor.find('.') != std::string::npos) {
                try {
                    size_t pos;
                    float floatValue = std::stof(valor, &pos);
                    if (pos == valor.size()) {
                        floatVariables[variavel] = floatValue;
                    } else {
                        // Caso tenha caracteres extras após o ponto decimal, considere como uma string
                        stringVariables[variavel] = valor;
                    }
                } catch (const std::invalid_argument&) {
                    // Caso não seja um número float válido, considere como uma string
                    stringVariables[variavel] = valor;
                }
            } else {
                // Se não tiver ponto decimal, tente converter para inteiro
                try {
                    size_t pos;
                    int intValue = std::stoi(valor, &pos);
                    if (pos == valor.size()) {
                        intVariables[variavel] = intValue;
                    } else {
                        // Caso tenha caracteres extras após o número, considere como uma string
                        stringVariables[variavel] = valor;
                    }
                } catch (const std::invalid_argument&) {
                    // Caso não seja um número inteiro válido, considere como uma string
                    stringVariables[variavel] = valor;
                }
            }
        }

        return true;
    } else {
        std::cerr << "Erro: Atribuição inválida." << std::endl;
        return false;
    }
}

bool Parser::structARITMETICA(const vector<string> &tokenList)
{
    cout<<"# ARITMETICA #\n";

    // Verifica se há pelo menos três tokens (nome da variável, sinal de atribuição, valor)
    if (tokenList.size() < 3) {
        cerr << "Erro: Expressão incompleta." << endl;
        return false;
    }

    // Extrai os tokens
    const string& nomeVariavel = tokenList[0];
    const string& sinalAtribuicao = tokenList[1];
    const string& valor = tokenList[2];
    int cont = 0;


    for (int pos = 2; pos < tokenList.size(); pos++) {
        cout<<tokenList[pos]<<" ";

        if (pos == tokenList.size()-1){
            if(isOperator(tokenList[pos]))
                throw invalid_argument("Esta faltando argumento");
        }

        if (cont == 0){
            if(isVariable(tokenList[pos]) || isNumber(tokenList[pos]) || isString(tokenList[pos]))
                cout<<"E variavel"<<endl;
            else
                //throw
                throw invalid_argument("Token nao e uma variavel.");

            cont = 1;
        }else{
            if(isOperator(tokenList[pos]))
                cout<<"E operador"<<endl;
            else
                //throw
                throw invalid_argument("Token nao e um operador.");

            cont = 0;
        }
    }
    return true;
}

bool Parser::structPRINT(const vector<string>  &tokenList)
{
    std::cout << "# PRINT # \n";
   // mostrarComando(tokenList);

    if (tokenList.size() >= 2) {
        const std::string& segundoToken = tokenList[1];
        if (segundoToken.size() >= 2 && segundoToken.front() == '"' && segundoToken.back() == '"') {
            // O segundo token é uma string entre aspas
            //std::cout << "STRING: " << segundoToken << std::endl;
        } else {
            // O segundo token pode conter espaços e ainda ser uma string entre aspas
            std::string possivelString;
            for (size_t i = 1; i < tokenList.size(); ++i) {
                possivelString += tokenList[i];
                if (i != tokenList.size() - 1) {
                    possivelString += " ";
                }
            }
            std::stringstream ss(possivelString);
            std::string token;
            if (std::getline(ss, token) && token.front() == '"' && token.back() == '"') {
                //std::cout << "STRING: " << token << std::endl;
                return true;
            } else {
                // Se não é uma string entre aspas, então é uma variável
                //std::cout << "VARIAVEL: " << possivelString << std::endl;
                return true;
            }
        }
    } else {
        // Não há segundo token
        std::cerr << "Erro: esperava um argumento!" << std::endl;
        return false;
    }

    return true;
}

bool Parser::structINPUT(const vector<string>  &tokenList)
{
    std::cout << "# INPUT # \n";

    if (tokenList.size() >= 2) {
        const std::string& segundoToken = tokenList[1];
        if (!segundoToken.empty() && std::isalpha(segundoToken.front()) && segundoToken.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_") == std::string::npos) {
            // O segundo token é um nome de variável válido
            //std::cout << "VALIDO\n";
        } else {
            // Segundo token inválido
            std::cerr << "Erro: Segundo token invalido!" << std::endl;
            return false;
        }
    } else {
        // Não há segundo token
        std::cerr << "Erro: Nao ha segundo token!" << std::endl;
        return false;
    }

    return true;
}

bool Parser::structREM(const std::vector<std::string>& tokenList) {
    std::cout << "# REM # \n";

    if (tokenList.size() >= 2) {
        const std::string& segundoToken = tokenList[1];
        if (segundoToken.front() == '"' && segundoToken.back() == '"') {
            // O segundo token é uma string entre aspas
            //std::cout << "Comentário válido: " << segundoToken << std::endl;
            return true;
        } else {
            // O segundo token não é uma string entre aspas
            for (size_t i = 2; i < tokenList.size(); ++i) {
                if (tokenList[i].front() == '"' && tokenList[i].back() == '"') {
                    // Verifica se a palavra contém uma string entre aspas
                    std::cerr << "Erro: Comentário inválido! Palavra contém uma string entre aspas." << std::endl;
                    return false;
                }
            }
            // Se chegou aqui, indica que nenhuma palavra contém uma string entre aspas
            //std::cout << "Comentário válido: ";
            for (const auto& token : tokenList) {
                //std::cout << token << " ";
            }

            return true;
        }
    } else {
        // Não há segundo token
        std::cerr << "Erro: Não há segundo token!" << std::endl;
        return false;
    }
}


bool Parser::structGOTO(const vector<string>  &tokenList)
{
    std::cout << "# GOTO # \n";

    if (tokenList.size() >= 2) {
        const std::string& segundoToken = tokenList[1];
        std::stringstream ss(segundoToken);
        int numero;
        if (ss >> numero && ss.eof()) {
            // O segundo token é um número inteiro
            return true;
            //std::cout << "O segundo token e um numero inteiro: " << numero << std::endl;
        } else {
            // Segundo token não é um número inteiro
            std::cerr << "Erro: label errada!" << std::endl;
            return false;
        }
    } else {
        // Não há segundo token
        std::cerr << "Erro: Nao ha segundo token!" << std::endl;
        return false;
    }

    return true;
}

bool Parser::structIF(const vector<string>  &tokenList)
{
    std::cout << "# IF #\n";

    if (tokenList.size() >= 6) {
        const std::string& variavelOuNumero1 = tokenList[1];
        const std::string& operador = tokenList[2];
        const std::string& variavelOuNumero2 = tokenList[3];
        const std::string& then = tokenList[4];

        if (then == "THEN" && tokenList.size() >= 7) {
            const std::vector<std::string> comandoDepoisDoThen(tokenList.begin() + 5, tokenList.end());

            if (operador == ">" || operador == "<" || operador == "<>" || operador == "=") {
                if ((isString(variavelOuNumero1) || isVariable(variavelOuNumero1) || isNumber(variavelOuNumero1)) &&
                    (isString(variavelOuNumero2) || isVariable(variavelOuNumero2) || isNumber(variavelOuNumero2))) {
                    //std::cout << "Estrutura IF válida" << std::endl;
                    return true;
                } else {
                    std::cerr << "Erro: Operadores com tipos de operandos inválidos!" << std::endl;
                    return false;
                }
            } else {
                std::cerr << "Erro: Operador inválido!" << std::endl;
                return false;
            }
        } else {
            std::cerr << "Erro: Estrutura IF inválida! THEN não encontrado ou comando após THEN faltando." << std::endl;
            return false;
        }
    } else {
        std::cerr << "Erro: Não há tokens suficientes para formar uma instrução IF válida!" << std::endl;
        return false;
    }
}

void Parser::encontrarTokensAposTHEN(const std::vector<string> &tokenList)
{
    std::vector<std::string> comando;
    bool encontrouTHEN = false;
    for (size_t i = 0; i < tokenList.size(); ++i) {
        if (encontrouTHEN) {
            comando.push_back(tokenList[i]);
        } else if (tokenList[i] == "THEN") {
            encontrouTHEN = true;
        }
    }
    mostrarComando(comando);
    identificarComando(comando);
    //dentificarComando(comando);
}


bool Parser::structHALT(const vector<string> &tokenList)
{
    cout << "# HALT # \n";
    //Parar a Execução
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

bool Parser::isVariable(const string &str)
{
    if (str.empty()) return false;
    if (!std::isalpha(str.front())) return false;
    for (char c : str) {
        if (!std::isalnum(c) && c != '_') return false;
    }
    return true;
}

bool Parser::isOperator(const string &str)
{
    static const std::unordered_set<std::string> operators = {"+", "-", "*", "/"};
    return operators.find(str) != operators.end();
}

bool Parser::isNumber(const string &str)
{
    if (str.empty()) return false;
    bool hasDot = false;
    for (char c : str) {
        if (!std::isdigit(c)) {
            if (c == '.' && !hasDot) {
                hasDot = true;
            } else {
                return false;
            }
        }
    }
    return true;
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


//  TRANSFORMANDO EM EXE
void Parser::imprimirComandosExecutaveis(const std::vector<std::pair<int, std::vector<string> > > &comandosExecutaveis)
{
    for (const auto& comando : comandosExecutaveis) {
        std::cout << "[" << comando.first << "] ";
        for (const auto& token : comando.second) {
            std::cout << token << " ";
        }
        std::cout << std::endl;
    }
}

std::vector<std::pair<int, std::vector<string> > > Parser::criarComandosExecutaveis(const std::vector<std::pair<int, std::vector<string> > > &tokens)
{

    std::vector<std::pair<int, std::vector<std::string>>> comandosExecutaveis;
    for (const auto& tokenPair : tokens) {
        const std::vector<std::string>& tokenList = tokenPair.second;
        std::vector<std::string> comando;
        for (const auto& token : tokenList) {
            if (token == ":") {
                // Se encontramos um delimitador de comando, adicionamos o comando ao vetor de comandos executáveis
                if (!comando.empty()) {
                    comandosExecutaveis.push_back({tokenPair.first, comando});
                    comando.clear();
                }
            } else {
                // Adiciona o token ao comando atual
                comando.push_back(token);
            }
        }
        // Verifica se há algum comando pendente após o término dos tokens
        if (!comando.empty()) {
            comandosExecutaveis.push_back({tokenPair.first, comando});
        }
    }

    cout<<"\n-----* EXECUTAVEL *------\n";

    imprimirComandosExecutaveis(comandosExecutaveis);
    return comandosExecutaveis;

}
