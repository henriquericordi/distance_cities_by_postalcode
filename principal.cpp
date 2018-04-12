#include <iostream>
#include "bib.hpp"

using namespace std;

int main(){
    testa_dados();//verifica o arquivo de pré-processamento e o cria caso necessário
    ler_cidades();//carrega as cidades de dados.csv na memoria
    int op;
    do{ //repetição até informar para sair do programa
        op = menu();
        if (op == 1){//funcionalidade 1
            int cep1, cep2, p_cidade1, p_cidade2;
            ler_ceps(&cep1, &cep2);
            //consultar CEP ------ informar caso não exista o CEP
            if (consulta_cep(cep1, &p_cidade1) && consulta_cep(cep2, &p_cidade2)){
                double distancia = calcular_distancia(&cidades[p_cidade1], &cidades[p_cidade2]);
                cout << "A distancia entre " << cidades[p_cidade1].nome << "-" << cidades[p_cidade1].uf << " e a cidade " << cidades[p_cidade2].nome << "-" << cidades[p_cidade2].uf <<" é de " << distancia << " Km\n";
            } else{
                cout << "ERROR: CEP(s) inválido(s)!\n";
            }
        } else if (op == 2){//funcionalidade 2
            cout << "Informe o nome do arquivo[\"cidades.txt\"]: ";
            string arq;
            fstream arquivo;
            cin.ignore();
            getline(cin,arq);
            if (arq == ""){
                arq = "cidades.txt";
            }
            if (valida_arq(arq, &arquivo)){
                vector<Cidade> arq_cidades; //lista dinâmica das cidades a serem testadas
                vector<int> ceps; //lista dinâmica apenas dos CEPs
                while(arquivo.good()){
                    string linha;
                    int i;
                    getline(arquivo, linha);
                    bool consulta = consulta_cep(stoi(linha), &i);
                    if (consulta){
                        arq_cidades.push_back(cidades[i]);
                        ceps.push_back(stoi(linha));
                    } else{
                        cout << "ERROR: CEP(s) inválido(s)!\n";
                    }
                }
                arquivo.close();
                fstream distancias;
                distancias.open("distancias.txt", ios::out | ios::trunc);
                for (int c = 0; c < arq_cidades.size(); c++){ //escreve o arquivo final e retorna ao menu
                    for (int i = c + 1; i < arq_cidades.size(); i++){
                        double distancia = calcular_distancia(&arq_cidades[c], &arq_cidades[i]);
                        distancias << arq_cidades[c].nome << '-' << arq_cidades[c].uf << ';' << ceps[c] << ';' << arq_cidades[i].nome << '-' << arq_cidades[i].uf << ';' << ceps[i] << ';' << distancia << "Km\n";
                    }
                }
                distancias.close();
                cout << "Arquivo criado com sucesso![\"distancias.txt\"]\n";
            } else{
                cout << "ERROR: Arquivo inválido!\n";
            }
        }
    } while (op != 0);
    return 0;
}