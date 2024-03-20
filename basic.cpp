#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <algorithm>

void print(const std::string &msg)
{
    std::cout << msg << std::endl;
}

bool isNumber(const std::string &s)
{
    return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}

bool isString(const std::string &s)
{
    return !s.empty() && std::all_of(s.begin(), s.end(), ::isalpha);
}

void sumInt(std::map<std::string, std::string> &variaveis)
{
    if (variaveis.find("Procurar A") != variaveis.end() && variaveis.find("Procurar B") != variaveis.end() && isNumber(variaveis["VER SE É INT A"]) && isNumber(variaveis["VER SE É INT C"]))
    {
        // Converte os valores das variáveis A e C para inteiros e soma-os
        int soma = std::stoi(variaveis["A"]) + std::stoi(variaveis["C"]);

        // Imprime o resultado
        print("A + C = " + std::to_string(soma));
    }
    else
    {
        print("Erro: Variáveis A e/ou C não foram definidas ou não são numéricas!");
    }
}

void functionATB(std::map<std::string, std::string> &variaveis, const std::string &linha)
{
    std::istringstream iss(linha);

    std::string numLinha;
    std::string nomeVariavel;
    std::string sinalAtribuicao;
    std::string valorVariavel;

    while (iss >> numLinha >> nomeVariavel >> sinalAtribuicao >> valorVariavel)
    {
        if (nomeVariavel == ":")
        {
            // Ignora o ":" como nome de variável
            continue;
        }

        if (isString(nomeVariavel))
        {
            print("LABEL: " + numLinha);
            print("VARIAVEL: " + nomeVariavel);
            print("SINAL: " + sinalAtribuicao);

            if (sinalAtribuicao == "=")
            {
                if (isNumber(valorVariavel))
                {
                    print("VALOR: " + valorVariavel);
                    variaveis[nomeVariavel] = valorVariavel;
                }
                else
                {
                    print("Erro: Valor inválido!");
                    return;
                }
            }
            else
            {
                print("Erro: Sinal de atribuição inválido!");
                return;
            }
        }
        else
        {
            print("Erro: Nome de variável inválido!");
            return;
        }
    }
}

int main()
{
    std::ifstream file("/home/pedro/Documents/interpreterBasic/basic.txt");
    if (file.is_open())
    {
        print("Arquivo aberto com sucesso!");
        std::string linha;

        std::map<std::string, std::string> variaveis;

        // Lê uma linha do arquivo
        std::getline(file, linha);

        // Chama a função para processar a linha
        functionATB(variaveis, linha);

        // Saída para debug
        print("// Saida do programa: \n");
        for (const auto &par : variaveis)
        {
            std::cout << par.first << " = " << par.second << std::endl;
        }

        file.close(); // Fecha o arquivo
    }
    else
    {
        print("Erro ao abrir o arquivo!");
    }

    return 0;
}
