#include "parser.h"

Parser::Parser() {

}

bool Parser::verificarOrdem(map<int, vector<string>>& tokens)
{
    int expectedId = 1; // Começa com o valor 1

    for (const auto& pair : tokens) {
        int currentId = pair.first;

        // Verifica se o ID atual é igual ao esperado
        if (currentId != expectedId) {
            // Se não for, significa que a ordem está quebrada ou há uma duplicata
            return false;
        }

        // Atualiza o valor esperado para o próximo valor
        ++expectedId;
    }

    // Se todos os IDs estiverem em ordem crescente e sem duplicatas, retorna verdadeiro
    return true;
}


bool Parser::verStruct(map<int, vector<string> > tokens)
{
    try {
        if (!verificarOrdem(tokens)) {
            throw std::invalid_argument("Erro nas Labels: repeticao ou fora de ordem");
        }
    } catch (const std::invalid_argument& e) {
        throw e; // Lança a exceção capturada novamente
    }

    return true; // Retorna true se todas as verificações passarem
}
