#include "Semantic.h"
Semantic::Semantic() {

}

void Semantic::interpretador(vector<pair<int, vector<string> > > executaveis)
{
    std::cout << "\n-----* RESULTADO *------\n";

    // 1) Mapeia a ordem de execução com as labels
    std::unordered_map<int, size_t> labelMap;
    for (size_t i = 0; i < executaveis.size(); ++i) {
        labelMap[executaveis[i].first] = i;
    }

    size_t proximaLinha = 0;

    while (proximaLinha < executaveis.size()) {
        const auto& comando = executaveis[proximaLinha];

        if (!identificarComando(comando.second, proximaLinha)) {
            if (comando.second.front() == "HALT") {
                std::cerr << "FIM" << std::endl;
                break;
            }
            //std::cerr << "ERRO NA LABEL " << comando.first << std::endl;
            //break;
        }

        if (comando.second.front() == "GOTO") {
            // 3) Verifica se o comando é um GOTO
            int label = std::stoi(comando.second[1]);
            auto it = labelMap.find(label);
            if (it != labelMap.end()) {
                proximaLinha = it->second;
            } else {
                std::cerr << "ERRO: LABEL " << label << " NÃO ENCONTRADA" << std::endl;
                break;
            }
        } else if (comando.second.front() == "IF") {
            // 4) Se for um IF, manda para execIF
            int label = execIF(comando.second, proximaLinha);
            if (label != 0) {
                // Se o IF retornar um número diferente de zero, executa um execGOTO
                auto it = labelMap.find(label);
                if (it != labelMap.end()) {
                    proximaLinha = it->second;
                } else {
                    std::cerr << "ERRO: LABEL " << label << " NÃO ENCONTRADA" << std::endl;
                    break;
                }
            } else {
                proximaLinha++;
            }
        } else {
            proximaLinha++;
        }
    }
}

bool Semantic::identificarComando(const std::vector<string> &comando, int proximaLinha)
{
    if (comando.empty()) {
        std::cerr << "Erro: Comando vazio" << std::endl;
        return false;
    }

    // cout<<" #COMANDO"<<endl;
    // for (const string &tokens : comando) {
    //     cout<<tokens<<" ";
    // }
    // cout<<endl;

    const std::string& primeiroToken = comando[0];
    const std::string& segundoToken = comando[1];

    if (primeiroToken == "PRINT") {
        if (!execPRINT(comando))
            return false;
    } else if (primeiroToken == "INPUT") {
        if (!execINPUT(comando))
            return false;
    } else if (primeiroToken == "HALT") {
        if (!execHALT(comando))
            return false;
    } else if (primeiroToken == "IF") {
        if (!execIF(comando,proximaLinha))
            return false;
    } else if (primeiroToken == "REM") {
        if (!execREM(comando))
            return false;
    } else if (primeiroToken == "GOTO") {
        execGOTO(comando, proximaLinha);
    } else if (segundoToken == "=" && comando.size() >= 5) {
        if (!execARITMETIC(comando))
            throw "Erro na estrutura do comando OPE ARIT";
    }else if (segundoToken == "="){
        if (!execATB(comando))
            return false;
    } else {
        std::cerr << "Erro: Comando desconhecido!" << std::endl;
        return false;
    }
    return true;
}


bool Semantic::verificarGOTO(std::vector<std::string>& comando, int& proximaLinha, const std::vector<int>& ordemDeExecucao) {
    auto itGOTO = std::find(comando.begin(), comando.end(), "GOTO");
    if (itGOTO != comando.end()) {
        auto posGOTO = std::distance(comando.begin(), itGOTO);
        if (posGOTO + 1 < comando.size()) {
            try {
                int linhaDestino = std::stoi(comando[posGOTO + 1]);
                auto itLinha = std::find(ordemDeExecucao.begin(), ordemDeExecucao.end(), linhaDestino);
                if (itLinha != ordemDeExecucao.end()) {
                    proximaLinha = std::distance(ordemDeExecucao.begin(), itLinha);
                    return true;
                } else {
                    std::cerr << "Erro: Linha de destino para GOTO não encontrada" << std::endl;
                }
            } catch (const std::invalid_argument &) {
                std::cerr << "Erro: Argumento inválido para GOTO" << std::endl;
            }
        }
    }
    return false;
}
bool Semantic::verificarIF(std::vector<std::string>& comando, int& proximaLinha, const std::vector<int>& ordemDeExecucao) {
    auto itIF = std::find(comando.begin(), comando.end(), "IF");
    if (itIF != comando.end()) {
        auto posIF = std::distance(comando.begin(), itIF);
        if (posIF + 3 < comando.size()) {
            // Implemente aqui a lógica para verificar e executar o comando IF
            // Você pode chamar sua função execIF aqui
            return true; // Suponha que a verificação do IF tenha sido bem-sucedida
        }
    }
    return false;
}

bool Semantic::execPRINT(const std::vector<string> &comando)
{
    if (comando.size() < 2) {
        std::cerr << "Erro: Comando PRINT incompleto" << std::endl;
        return false;
    }

    std::string conteudo; // Variável para armazenar o conteúdo a ser impresso

    // Percorre os tokens do comando
    for (size_t i = 1; i < comando.size(); ++i) {
        const std::string& token = comando[i];

        // Verifica se o token começa e termina com aspas
        if (token.front() == '"' && token.back() == '"') {
            // Se sim, extrai o conteúdo entre as aspas e imprime
            std::string texto = token.substr(1, token.size() - 2);
            std::cout << texto << " ";
        } else if (token.front() == '"') {
            // Se o token começa com aspas, mas não termina, armazena para processamento posterior
            conteudo = token.substr(1); // Remove a aspas inicial
        } else if (token.back() == '"') {
            // Se o token termina com aspas, mas não começa, completa o conteúdo e imprime
            conteudo += " " + token.substr(0, token.size() - 1); // Remove a aspas final
            std::cout << conteudo << " ";
            conteudo.clear(); // Limpa o conteúdo para o próximo token
        } else if (!conteudo.empty()) {
            // Se há um conteúdo pendente, continua a adicionar ao conteúdo
            conteudo += " " + token;
        } else {
            //cout<<"e UMA VAR: ";
            // Se não estiver delimitado por aspas, imprime o próprio token
            if (verificaToken(token))
                if (existeVariavel(token)){
                    std::string textoSemAspas;
                    std::string texto = buscarVariavel(token).second;
                    for (char c : texto) {
                        if (c != '\"') { // Verifica se o caractere não é uma aspas
                            textoSemAspas += c;
                        }
                    }
                    cout<<textoSemAspas;
                }else
                    cout<<"0";
            else
                return true;
        }
    }

    std::cout << std::endl;
    return true;
}

bool Semantic::execINPUT(const std::vector<string> &comando)
{
    // Verifica se o comando tem pelo menos dois tokens
    if (comando.size() < 2) {
        std::cerr << "Erro: Comando INPUT inválido." << std::endl;
        return false;
    }

    // Obtém o nome da variável a partir do comando
    const std::string& nomeDaVariavel = comando[1];

    // Verifica se a variável já existe
    if (existeVariavel(nomeDaVariavel)) {
        // Se a variável já existe, solicita ao usuário que insira o novo valor
        //std::cout << "Insira Novo: ";
        std::string novoValor;
        std::cin >> novoValor;
        // Modifica o valor da variável
        modificarVariavel(nomeDaVariavel, novoValor);
    } else {
        // Se a variável não existe, solicita ao usuário que insira o valor inicial
        //std::cout << "INFORME ";
        std::string valorInicial;
        std::cin >> valorInicial;
        // Cria a nova variável com o valor inicial
        criarVariavel(nomeDaVariavel, valorInicial);
    }

    return true;
}

bool Semantic::execREM(const std::vector<string> &comando)
{
    return true; //feito
}

int Semantic::execIF(const std::vector<string> &comando,int proximaLinha)
{

    // Verifica se o comando está completo
    if (comando.size() < 6) {
        std::cerr << "Erro: Comando IF incompleto" << std::endl;
        return false;
    }

    // Extrai os componentes do comando
    std::string variavel1 = comando[1];
    const std::string& operador = comando[2];
    std::string valor = comando[3];
    const std::string& thenToken = comando[4];


    //*********************************************** THEN
    /// Procura a posição do comando THEN
    size_t thenPos = 0;
    for (size_t i = 0; i < comando.size(); ++i) {
        if (comando[i] == "THEN") {
            thenPos = i;
            break;
        }
    }

    // Obtém o comando THEN
    std::vector<std::string> thenCommand(comando.begin() + thenPos + 1, comando.end());
    //*********************************************** THEN

    // Verifica se a variável 1 existe
    if (!existeVariavel(variavel1)) {
        std::cerr << "Erro: Variável " << variavel1 << " não existe" << std::endl;
        return false;
    }
    if (!existeVariavel(valor)) {
        valor = "0";
    }

    // Obtém o valor da variável 1
    string valorVar1 = buscarVariavel(variavel1).second;
    valor = buscarVariavel(valor).second;
    // Converte o valorVar1 para o tipo apropriado (int, float ou mantém como string)
    valorVar1 = converterParaTipo(valorVar1);
    valor = converterParaTipo(valor);

    // Realiza a comparação
    bool condicao = verificarComparacao(valorVar1,valor,operador);

    if (condicao) {
        if (thenCommand.front() == "GOTO") {
            try {
                int label = std::stoi(thenCommand[1]);
                return label;
            } catch (const std::invalid_argument&) {
                return 0;
            }
        }else{
            //identificarComando(thenCommand,1);
            return 0;
        }
    }
    return 0;
}

int Semantic::execGOTO(const std::vector<string> &comando, int label)
{
    if (comando.size() < 2) {
        std::cerr << "Erro: Comando GOTO incompleto" << std::endl;
        return false;
    }

    const std::string& tokenLinha = comando[1];

    try {
        int linhaDestino = std::stoi(tokenLinha);
        label = linhaDestino; // Ajusta para o índice correto do vetor de comandos
    } catch (const std::invalid_argument&) {
        std::cerr << "Erro: Argumento inválido para GOTO" << std::endl;
    }
    return label;
}

bool Semantic::execHALT(const std::vector<string> &comando)
{
    return false;
}

bool Semantic::execATB(const vector<string> &tokenList)
{

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

bool Semantic::execARITMETIC(const vector<string> &tokenList)
{

    // Verifica se há pelo menos três tokens
    if (tokenList.size() < 3) {
        std::cerr << "Erro: Expressão incompleta." << std::endl;
        return false;
    }

    // Extrai os tokens
    const std::string& nomeVariavel = tokenList[0];
    const std::string& sinalAtribuicao = tokenList[1];
    std::string concatString = "";
    float resultFloat = 0.0;


    if (verificarTiposString(tokenList)){
        //sefor verdadeiro é pq todos sãos string e é para concatenar
        // Realiza a concatenação das strings a partir do terceiro token
        for (size_t i = 2; i < tokenList.size(); i++) {
            if (isString(tokenList[i])){
                concatString = operacaoString(concatString,tokenList[i],"+");
            }
        }
        // Atualiza ou cria a variável com a string concatenada
        if (existeVariavel(nomeVariavel)) {
            modificarVariavel(nomeVariavel, concatString);
        } else {
            criarVariavel(nomeVariavel, concatString);
        }
    }else if (verificarTiposNumber(tokenList)){
        cout <<"sao todos numero"<<endl;
        //sefor verdadeiro é pq todos sãos numeros e é um operação valida
        for (size_t i = 2; i < tokenList.size(); i++) {
            cout <<"token"<<tokenList[i]<<endl;
            if (isNumber(tokenList[i])){
                cout <<"resultFloat "<<to_string(resultFloat)<<" | "<<tokenList[i] <<" | "<<tokenList[i+1]<<endl;
                resultFloat = operacaoFloat(to_string(resultFloat),tokenList[i],tokenList[i+1]);
            }
        }
        // Atualiza ou cria a variável com a string concatenada
        if (existeVariavel(nomeVariavel)) {
            modificarVariavel(nomeVariavel, to_string(resultFloat));
        } else {
            criarVariavel(nomeVariavel, to_string(resultFloat));
        }
    }else{
        // os valores tem variaveis
        // verificar tipo e variavel
        //
    }

    return true;
}

bool Semantic::verificarTiposNumber(const vector<string> &tokenList)
{
    bool status;
    for (size_t i = 2; i < tokenList.size(); i++) {
        if (i % 2 == 0){ // VALOR
            if (isNumber(tokenList[i])){
                status = true;
            }else{
                status = false;
            }
        }else{
            if(tokenList[i] == "+"){
                status = true;
            }else{
                status = false;
            }
        }
    }
    return status;
}

bool Semantic::verificarTiposString(const vector<string> &tokenList)
{
    bool status;
    for (size_t i = 2; i < tokenList.size(); i++) {
        if (i % 2 == 0){ // VALOR
            if (isString(tokenList[i])){
                status = true;
            }else{
                status = false;
            }
        }else{
            if(tokenList[i] == "+"){
                status = true;
            }else{
                status = false;
            }
        }
    }
    return status;
}




bool Semantic::isString(const string &valor)
{
    return (!valor.empty() && valor.front() == '"' && valor.back() == '"');
}

bool Semantic::isVariable(const string &str)
{
    if (str.empty()) return false;
    if (!std::isalpha(str.front())) return false;
    for (char c : str) {
        if (!std::isalnum(c) && c != '_') return false;
    }
    return true;
}

bool Semantic::isOperator(const string &str)
{
    static const std::unordered_set<std::string> operators = {"+", "-", "*", "/"};
    return operators.find(str) != operators.end();
}

bool Semantic::isNumber(const string &str)
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

bool Semantic::isFloat(const string &valor)
{
    std::istringstream iss(valor);
    float f;
    iss >> std::noskipws >> f;
    return iss.eof() && !iss.fail();
}

float Semantic::operacaoFloat(const string &var1, const string &var2, const string &op)
{
    float variavel1 = stof(var1);
    float variavel2 = stof(var2);

    float result = 0.0;

    if (op == "+"){
        result = variavel1 + variavel2;
    }else if (op == "-"){
        result = variavel1 - variavel2;
    }else if (op == "/"){
        if (variavel1 == 0 || variavel2 == 0 )
            result = 0;
        else
            result = variavel1 / variavel2;
    }else if (op == "*"){
        result = variavel1 * variavel2;
    }else{
        result = 0.0;
    }

    return result;
}

int Semantic::operacaoInt(const string &var1, const string &var2, const string &op)
{
    int variavel1 = stof(var1);
    int variavel2 = stof(var2);

    int result = 0;

    if (op == "+"){
        result = variavel1 + variavel2;
    }else if (op == "-"){
        result = variavel1 - variavel2;
    }else if (op == "/"){
        if (variavel1 == 0 || variavel2 == 0 )
            result = 0;
        else
            result = variavel1 / variavel2;
    }else if (op == "*"){
        result = variavel1 * variavel2;
    }else{
        result = 0;
    }

    return result;
}

string Semantic::operacaoString(const string &var1, const string &var2, const string &op)
{
    string variavel1 = var1;
    string variavel2 = var2;

    //cout<<"@variavel1"<< variavel1 <<endl;
    //cout<<"@variavel2"<< variavel2 <<endl;

    string result = "";
    variavel1 += " ";

    if (op != "+")
        return "ERRO AO CONCATENAR";
    else
        result += variavel1 + variavel2;

    //cout<<"@result: " << result <<endl;
    return result;
}

void Semantic::armazenarVars(map<string, int> intVariablesExt, map<string, float> floatVariablesExt, map<string, string> stringVariablesExt)
{
    intVariables = intVariablesExt;
    floatVariables = floatVariablesExt;
    stringVariables = stringVariablesExt;
}


std::pair<std::string, std::string> Semantic::buscarVariavel(const std::string &variavel)
{
    if (intVariables.count(variavel) > 0) {
        return std::make_pair("int", std::to_string(intVariables[variavel]));
    } else if (floatVariables.count(variavel) > 0) {
        return std::make_pair("float", std::to_string(floatVariables[variavel]));
    } else if (stringVariables.count(variavel) > 0) {
        return std::make_pair("string", stringVariables[variavel]);
    } else {
        return std::make_pair("0", ""); // Retornar um par de strings vazias se não for encontrada a variável
    }
}

bool Semantic::existeVariavel(const string &variavel)
{
    return (intVariables.count(variavel) > 0 ||
            floatVariables.count(variavel) > 0 ||
            stringVariables.count(variavel) > 0);
}

bool Semantic::verificaToken(const string &token)
{
    // Verifica se a string não está vazia
    if (token.empty()) {
        return false;
    }

    // Verifica se o primeiro caractere é uma letra
    if (!std::isalpha(token.front())) {
        return false;
    }

    // Verifica se cada caractere, exceto o primeiro, é alfanumérico
    for (size_t i = 1; i < token.size(); ++i) {
        if (!std::isalnum(token[i])) {
            return false;
        }
    }

    // Se passou em todas as verificações, retorna verdadeiro
    return true;
}

string Semantic::converterParaTipo(const string &valor)
{
    // Verifica se a string é um número inteiro
    bool isInt = true;
    for (char c : valor) {
        if (!std::isdigit(c)) {
            isInt = false;
            break;
        }
    }

    // Se for um número inteiro, retorna como inteiro
    if (isInt) {
        return valor;
    }

    // Verifica se a string é um número de ponto flutuante
    std::istringstream iss(valor);
    float floatValue;
    if (iss >> std::noskipws >> floatValue >> std::ws && iss.eof()) {
        // Se for um número de ponto flutuante válido, retorna como float
        std::ostringstream oss;
        oss << std::setprecision(6) << std::fixed << floatValue;
        return oss.str();
    }

    // Se não for um número, retorna como string
    return valor;
}

bool Semantic::criarVariavel(const string &variavel, const string &valor)
{
    // Verificar se a variável já existe em alguma coleção
    if (intVariables.count(variavel) > 0 || floatVariables.count(variavel) > 0 || stringVariables.count(variavel) > 0) {
        std::cout << "A variável '" << variavel << "' já existe." << std::endl;
        return false;
    } else {
        // Determinar o tipo da variável com base no valor fornecido
        // e adicionar à coleção apropriada
        try {
            // Tenta converter o valor em int
            int intValor = std::stoi(valor);
            intVariables[variavel] = intValor;
        } catch (const std::invalid_argument&) {
            try {
                // Se a conversão para int falhar, tenta converter para float
                float floatValor = std::stof(valor);
                floatVariables[variavel] = floatValor;
            } catch (const std::invalid_argument&) {
                // Se a conversão para float também falhar, assume-se que é uma string
                stringVariables[variavel] = valor;
            }
        }
        return true;
    }
}

bool Semantic::modificarVariavel(const string &variavel, const string &novoValor)
{
    // Verificar se a variável existe
    if (intVariables.count(variavel) > 0) {
        // Se a variável é do tipo int
        try {
            // Tenta converter o novo valor em int
            int novoIntValor = std::stoi(novoValor);
            intVariables[variavel] = novoIntValor;
            return true;
        } catch (const std::invalid_argument&) {
            std::cout << "O novo valor fornecido para '" << variavel << "' não é do tipo int." << std::endl;
            return false;
        }
    } else if (floatVariables.count(variavel) > 0) {
        // Se a variável é do tipo float
        try {
            // Tenta converter o novo valor em float
            float novoFloatValor = std::stof(novoValor);
            floatVariables[variavel] = novoFloatValor;
            return true;
        } catch (const std::invalid_argument&) {
            std::cout << "O novo valor fornecido para '" << variavel << "' não é do tipo float." << std::endl;
            return false;
        }
    } else if (stringVariables.count(variavel) > 0) {
        // Se a variável é do tipo string
        stringVariables[variavel] = novoValor;
        return true;
    } else {
        std::cout << "A variável '" << variavel << "' não existe." << std::endl;
        return false;
    }
}
void Semantic::mostrarVariaveis()
{
    std::cout << "Variaveis SEMATIC :\n";
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

template<typename T, typename T2>
bool Semantic::verificarComparacao(const T &var, const T2 &var2, string op)
{
    if (op == "=")
        return var == var2;
    else if (op == ">")
        return var > var2;
    else if (op == "<")
        return var < var2;
    else if (op == "<>")
        return var != var2;
    else
        std::cerr<<"Erro na expressao boleana\n";

    return false;
}


