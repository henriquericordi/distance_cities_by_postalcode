#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>

using namespace std;

struct Cidade { //struct que armazena as informações das cidades em memória
    string nome;
    string uf;
    int cep_inicial = 0;
    int cep_final = 0;
    double latitude;
    double longitude;
};

void ler_ceps(int*, int*);
vector<string> separador(string, char);
void testa_dados();
void ler_cidades();
bool consulta_cep(int, int*);
int menu();
void limpa_tela();
double calcular_distancia(Cidade*, Cidade*);
bool valida_arq(string arq, fstream*);

vector<Cidade> cidades; //lista dinâmica das cidades

void ler_ceps(int *cep1, int *cep2){ //lê os ceps das duas cidades em variaveis para consulta
    limpa_tela();
    cout << "Informe o CEP da 1ª Cidade: ";
    cin >> *cep1;
    cout << "Informe o CEP da 2ª Cidade: ";
    cin >> *cep2;
}

vector<string> separador(string linha, char delimitador){ //separa os campos da linha lida do arquivo
   vector<string> elementos;
   string item;
   stringstream convertido;
   convertido.str(linha);
   while (getline(convertido, item, delimitador)) {
       elementos.push_back(item);
   }
   return elementos;
}

void testa_dados(){ //verifica o arquivo de pré-processamento e o cria caso necessário
    fstream dados;
    vector<Cidade> cidades_temp;
    dados.open("dados.csv", ios::in);
    if (dados.fail()){
        //CEP1;CEP2;NOMEDACIDADE;UF;LAT;LONG  - padrão de escrita do pré-processamento
        fstream municipios;
        municipios.open("municipios_br.csv", ios::in);
        while(municipios.good()){ // faz a leitura dos campos dos municípios
            string linha;
            getline(municipios, linha);
            if (linha != ""){
                vector<string> campos_municipios = separador(linha, ';');
                Cidade aux;
                aux.nome = campos_municipios[3];
                aux.uf = campos_municipios[4];
                campos_municipios[1][campos_municipios[1].find(',')] = '.';
                aux.latitude = stod(campos_municipios[1]);
                campos_municipios[2][campos_municipios[2].find(',')] = '.';
                aux.longitude = stod(campos_municipios[2]);
                cidades_temp.push_back(aux);
            }
        }
        municipios.close();
        vector<Cidade> cidades_cep;
        fstream cep;
        cep.open("cep.csv", ios::in);
        while(cep.good()){ // faz a leitura dos campos do CEP
            string linha;
            getline(cep, linha);
            if (linha != ""){
                vector<string> campos_cep = separador(linha, ';');
                Cidade aux;
                aux.nome = campos_cep[0];
                aux.uf = campos_cep[3];
                aux.cep_inicial = stoi(campos_cep[1]);
                aux.cep_final = stoi(campos_cep[2]);
                cidades_cep.push_back(aux);
            }
        }
        cep.close();
        dados.open("dados.csv", ios::out | ios::trunc);
        for (int c = 0; c < cidades_temp.size(); c++){ //busca e compara os nomes e Ufs das cidades e preenche a struct
            for (int i = 0; i < cidades_cep.size(); i++){
                if ((cidades_temp[c].nome == cidades_cep[i].nome) && (cidades_temp[c].uf == cidades_cep[i].uf)){
                    cidades_temp[c].cep_inicial = cidades_cep[i].cep_inicial;
                    cidades_temp[c].cep_final = cidades_cep[i].cep_final;
                    break;
                }
            }
        }
        for (int c = 0; c < cidades_temp.size(); c++){ //escreve o arquivo de pré-processamento
            if (cidades_temp[c].cep_inicial != 0){
                dados << cidades_temp[c].cep_inicial << ';' << cidades_temp[c].cep_final << ';' << cidades_temp[c].nome << ';' << cidades_temp[c].uf << ';' << cidades_temp[c].latitude << ';' << cidades_temp[c].longitude << endl;
            }
        }
    }
    dados.close();
}

void ler_cidades(){
    fstream dados;
    dados.open("dados.csv", ios::in);
    while (dados.good()){
        string linha;
        getline(dados, linha);
        if (linha != ""){
            vector<string> campos_dados = separador(linha, ';');
            Cidade aux;
            aux.cep_inicial = stoi(campos_dados[0]);
            aux.cep_final = stoi(campos_dados[1]);
            aux.nome = campos_dados[2];
            aux.uf = campos_dados[3];
            aux.latitude = stod(campos_dados[4]);
            aux.longitude = stod(campos_dados[5]);
            cidades.push_back(aux);
        }
    }
    dados.close();
}

bool consulta_cep(int cep, int *index){ //faz a validação de um CEP e retorna se ele consta na base de dados ou não
    for (int c = 0; c < cidades.size(); c++){
        if ((cep >= cidades[c].cep_inicial) && (cep <= cidades[c].cep_final)){
            *index = c;
            return true;
        }
    }
    return false;
}

int menu(){
    //construção do menu "tela inicial"
    int op;
    cout << "\e[1;37m+-------------------------------------------+\n";
    cout << "|                 Zangão S/A                |\n";
    cout << "|                 \"A ";
    cout << "\e[1;33mMEL";
    cout << "\e[1;37mhor\"                |\n";
    cout << "+----+--------------------------------------+\n";
    cout << "| 1  |       Cálculo Individual             |\n";
    cout << "+----+--------------------------------------+\n";
    cout << "| 2  |        Cálculo em Lote               |\n";
    cout << "+----+--------------------------------------+\n";
    cout << "| 0  |              Sair                    |\n";
    cout << "+----+--------------------------------------+\n\n";
    cin >> op;
    return op;
}

void limpa_tela(){
   system("clear");
}

double calcular_distancia(Cidade *cidade1, Cidade *cidade2){//calcula a distância entre duas cidades já validadas e lidas e retorna o valor em Km
    const double R = 6372.795477598;
    const double PI = 3.1415926536;
    double alfa = sin(cidade1->latitude * PI / 180) * sin(cidade2->latitude * PI / 180);
    double beta = cos(cidade1->latitude * PI / 180) * cos(cidade2->latitude * PI / 180);
    double delta = cos((cidade1->longitude * PI / 180) - (cidade2->longitude  * PI / 180));
    return R * acos(alfa + beta * delta);
    /*return 6371 *acos(cos(PI * (90 - cidade2->latitude) / 180) * cos((90 - cidade1->latitude) * PI / 180) + sin((90 - cidade2->latitude) * PI / 180) * sin((90 - cidade1->latitude) * PI / 180) * cos((cidade1->longitude - cidade2->longitude) * PI / 180));*/
}

bool valida_arq(string arq, fstream* arquivo){ //verifica se o arquivo existe e não está vazio
    arquivo->open(arq, ios::in);
    if (arquivo->fail()){
        return false;
    } else{
       string linha;
       getline(*arquivo, linha);
       if (linha == ""){
           return false;
       } else{
           arquivo->close();
           arquivo->open(arq, ios::in);
           return true;
       }
    }
}