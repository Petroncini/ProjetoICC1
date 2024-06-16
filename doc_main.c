/*
Junho, 2024

O objetivo desse programa é sistematizar a gestão da reserva de voos de
uma companhia aérea.

Nele, é possível abrir um voo para reservas, realizar reservas de passageiros,
modificar, cancelar e consultar essas reservas, além de armazenar essas 
informações em arquivos no fim de cada dia até o fechamento do voo para reservas.

Autores:
Caio Petroncini
Nicolas Maia
Vitor Veiga
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Armazena os dados de um passageiro
typedef struct {
    char* nome;
    char* sobrenome;
    char CPF[20];
    int dia;
    int mes;
    int ano;
    char numVoo[10];
    char assento[10];
    char classe[20];
    float valor;
    char origem[5];
    char destino[5];
    int cancelado;
} passageiro;

// Declaração das funções
int encodeComand(char* comando);
void criarArquivos();
void abrirVoo(int* numAssentos);
int vooAberto();
void realizarReserva(passageiro** reservas, int* n, int assentos, int* numReservasVal);
void consultarReserva(passageiro* reservas, int n);
void modificarReserva(passageiro* reservas, int* n);
void cancelarReserva(passageiro* reservas, int* n, int* numReservasVal);
void fechamentoDia(passageiro* reservas, int* n, int numReservasVal);
void fechamentoVoo(passageiro* reservas, int* n);
void carregarReservas(passageiro** reservas, int* n, int* numReservasVal);
int verificarVoo();
void free_passageiros(passageiro* reservas, int n);

int main(void)
{
    // Cria e aloca espaços iniciais para a array de passageiros
    passageiro* reservas = malloc(10 * sizeof(passageiro));

    // Número de Reservas Total e Número de Reservas Válidas
    int numReservas = 0;
    int numReservasVal = 0;

    criarArquivos();

    carregarReservas(&reservas, &numReservas, &numReservasVal);
    int numAssentos = vooAberto();

    // Se voo está fechado, retorna as informações necessárias
    if (verificarVoo()) {
        fechamentoVoo(reservas, &numReservas);
    }

    // Declara comando
    char comando[3];
    int comandoEncoded = -1;

    // Loop para tratar cada comando lido
    do {
        scanf("%s", comando);
        getchar();

        comandoEncoded = encodeComand(comando);

        switch (comandoEncoded) {
        case 0:
            abrirVoo(&numAssentos);
            free_passageiros(reservas, numReservas);
            reservas = malloc(10 * sizeof(passageiro));
            numReservas = 0;
            numReservasVal = 0;
            break;
        case 1:
            realizarReserva(&reservas, &numReservas, numAssentos, &numReservasVal);
            break;
        case 2:
            consultarReserva(reservas, numReservas);
            break;
        case 3:
            modificarReserva(reservas, &numReservas);
            break;
        case 4:
            cancelarReserva(reservas, &numReservas, &numReservasVal);
            break;
        case 5:
            fechamentoDia(reservas, &numReservas, numReservasVal);
            break;
        case 6:
            fechamentoVoo(reservas, &numReservas);
            break;
        }
    } while (comandoEncoded != 5 && comandoEncoded != 6);
    
    free_passageiros(reservas, numReservas);
    return 0;
}

/*
Transforma a string do comando em um inteiro para ser tratado no switch.

@param comando: comando em forma de string.
@return um inteiro diferente para cada comando.
*/
int encodeComand(char* comando)
{
    if (comando[0] == 'A' && comando[1] == 'V') {
        return 0; // AV
    } else if (comando[0] == 'C') {
        if (comando[1] == 'R') {
            return 2; // CR
        } else {
            return 4; // CA
        }
    } else if (comando[1] == 'R') {
        if (comando[0] == 'R') {
            return 1; // RR
        } else {
            return 3; // MR
        }
    } else if (comando[1] == 'V') {
        return 6; // FA
    } else {
        return 5; // FD
    }
}

/*
Cria os arquivos necessários para a execução do programa caso eles ainda não existam.
*/
void criarArquivos()
{
    FILE* voos = fopen("voos.txt", "r");
    FILE* passageiros = fopen("passageiros.txt", "r");

    if (!voos) {
        voos = fopen("voos.txt", "w");
    }

    if (!passageiros) {
        passageiros = fopen("passageiros.txt", "w");
    }

    fclose(voos);
    fclose(passageiros);
}

/*
Abrir um novo voo.

Funcionamento: Abre e apaga os conteúdos dos arquivos necessários, lê as informações
do voo, coloca elas no arquivo e atualiza o número de assentos total.

@param numAssentos: ponteiro para o número total de assentos.
*/
void abrirVoo(int* numAssentos)
{
    FILE* voos = fopen("voos.txt", "w");
    if (!voos) {
        printf("Erro na abertura do arquivo voos.txt!\n");
        exit(1);
    }

    FILE* passageiros = fopen("passageiros.txt", "w");
    if (!passageiros) {
        printf("Erro na abertura do arquivo passageiros.txt!\n");
        exit(1);
    }

    int assentos;
    float valorEcon, valorExec;
    scanf("%d %f %f", &assentos, &valorEcon, &valorExec);
    getchar();

    fprintf(voos, "%d, %f, %f", assentos, valorEcon, valorExec);

    fclose(voos);
    fclose(passageiros);
    *numAssentos = assentos;
}

/*
Conseguir o número de assentos.

@return -1 se o voo não foi aberto, número total de assentos caso contrário.
*/
int vooAberto()
{
    FILE* voos = fopen("voos.txt", "r");
    if (!voos) {
        return -1;
    }

    int assentos;
    fscanf(voos, "%d", &assentos);

    fclose(voos);
    return assentos;
}

void carregarReservas(passageiro** reservas, int* n, int* numReservasVal)
{
    FILE* passageiros = fopen("passageiros.txt", "r");
    if (!passageiros) {
        printf("Erro na abertura do arquivo passageiros.txt!\n");
        exit(1);
    }

    char linha[200];

    while (fgets(linha, sizeof(linha), passageiros) != NULL) {
        passageiro r;
        char nome[50], sobrenome[50];

        sscanf(linha, "%s %s %s %d %d %d %s %s %s %f %s %s",
            nome, sobrenome, r.CPF, &r.dia, &r.mes, &r.ano,
            r.numVoo, r.assento, r.classe, &r.valor, r.origem, r.destino);

        r.nome = malloc((strlen(nome) + 1) * sizeof(char));
        r.sobrenome = malloc((strlen(sobrenome) + 1) * sizeof(char));
        r.cancelado = 0;

        strcpy(r.nome, nome);
        strcpy(r.sobrenome, sobrenome);

        (*reservas)[(*n)++] = r;
        (*numReservasVal)++;

        if (*n % 10 == 0) {
            *reservas = realloc(*reservas, (*n + 10) * sizeof(passageiro));
        }
    }
    fclose(passageiros);
}

void realizarReserva(passageiro** reservas, int* n, int assentos, int* numReservasVal)
{
    if (assentos == -1) {
        printf("Nenhum voo aberto\n");
        printf("--------------------------------------------------\n");
        return;
    }

    passageiro r;
    char nome[50], sobrenome[50];

    scanf("%s %s %s %d %d %d %s %s %s %f %s %s",
        nome, sobrenome, r.CPF, &r.dia, &r.mes, &r.ano,
        r.numVoo, r.assento, r.classe, &r.valor, r.origem, r.destino);
    getchar();

    if (verificarVoo()) {
        return;
    }

    if (*numReservasVal >= assentos) {
        printf("Nenhum assento disponível\n");
        printf("--------------------------------------------------\n");
        return;
    }

    r.nome = malloc((strlen(nome) + 1) * sizeof(char));
    r.sobrenome = malloc((strlen(sobrenome) + 1) * sizeof(char));
    r.cancelado = 0;

    strcpy(r.nome, nome);
    strcpy(r.sobrenome, sobrenome);

    (*reservas)[(*n)++] = r;
    (*numReservasVal)++;

    if (*n % 10 == 0) {
        *reservas = realloc(*reservas, (*n + 10) * sizeof(passageiro));
    }
}

/*
Consulta a reserva de um passageiro a partir de seu cpf.

Funcionamento: Lê o cpf. Depois percorre o vetor de reservas, verificando se o
cpf é o mesmo do buscado. Caso seja, escreve os dados do passageiro.

@param *reservas: Array de reservas já realizadas.
@param n: Número total de reservas realizadas.
*/
void consultarReserva(passageiro* reservas, int n)
{
    char cpf[20];
    scanf(" %s", cpf);

    for (int i = 0; i < n; i++) {
        passageiro r;
        r = reservas[i];
        if (strcmp(r.CPF, cpf) == 0 && r.cancelado != 1) {
            printf("%s\n%s %s\n%d/%d/%d\nVoo: %s\nAssento: %s\nClasse: %s\nTrecho: %s %s\nValor: %0.2f\n",
                r.CPF, r.nome, r.sobrenome, r.dia, r.mes, r.ano, r.numVoo, r.assento, r.classe, r.origem, r.destino, r.valor);
            printf("--------------------------------------------------\n");
        }
    }
}

/*
Modifica a reserva de um passageiro a partir de seu cpf.

Funcionamento: Lê o cpf antigo e as informações de mudança, percorre a base de
dados buscando o cpf que desejamos modificar. Se achar esse cpf, modificamos suas
informações e escrevemos na tela as novas informações do passageiro.

@param *reservas: Array de reservas já realizadas.
@param *n: ponteiro para o número total de reservas realizadas.
*/
void modificarReserva(passageiro* reservas, int* n)
{
    char CPFAntigo[20], nome[50], sobrenome[50], CPF[20], assento[10];
    scanf("%s %s %s %s %s", CPFAntigo, nome, sobrenome, CPF, assento);

    if (verificarVoo()) {
        return;
    }

    for (int i = 0; i < *n; i++) {
        if (!strcmp(CPFAntigo, reservas[i].CPF)) {
            free(reservas[i].nome);
            free(reservas[i].sobrenome);

            reservas[i].nome = malloc((strlen(nome) + 1) * sizeof(char));
            reservas[i].sobrenome = malloc((strlen(sobrenome) + 1) * sizeof(char));

            strcpy(reservas[i].nome, nome);
            strcpy(reservas[i].sobrenome, sobrenome);
            strcpy(reservas[i].CPF, CPF);
            strcpy(reservas[i].assento, assento);

            printf("Reserva Modificada:\n");
            printf("%s\n", reservas[i].CPF);
            printf("%s %s\n", reservas[i].nome, reservas[i].sobrenome);
            printf("%02d/%02d/%04d\n", reservas[i].dia, reservas[i].mes, reservas[i].ano);
            printf("Voo: %s\n", reservas[i].numVoo);
            printf("Assento: %s\n", reservas[i].assento);
            printf("Classe: %s\n", reservas[i].classe);
            printf("Trecho: %s %s\n", reservas[i].origem, reservas[i].destino);
            printf("Valor: %.2f\n", reservas[i].valor);
            printf("--------------------------------------------------\n");
            return;
        }
    }
}

/*
Cancela a reserva de um passageiro a partir de seu cpf.

Funcionamento: Lê o cpf. Depois percorre o vetor de reservas, verificando se o
cpf é o mesmo do buscado. Caso seja, muda o atributo "cacelado" do passageiro
para 1, depois, diminui o número de reservas válidas em 1.

@param *reservas: Ponteiro para o vetor de reservas já realizadas.
@param *n: Ponteiro para o número total de reservas realizadas.
@param *numReservasVal: Ponteiro para o número de reservas válidas.
*/
void cancelarReserva(passageiro* reservas, int* n, int* numReservasVal)
{
    char cpf[20];
    scanf(" %s", cpf);

    if (verificarVoo()) {
        return;
    }

    for (int i = 0; i < *n; i++) {
        if (!strcmp(reservas[i].CPF, cpf)) {
            reservas[i].cancelado = 1;
            (*numReservasVal)--;
        }
    }
}

/*
Fecha o dia, finalizando a atual execução do programa.

Funcionamento: Abre o arquivo de passageiros, copia os dados dos passageiros atuais
para o arquivo e escreve na tela as informações importantes do dia.

@param *reservas: Ponteiro para o vetor de reservas já realizadas.
@param *n: Ponteiro para o número total de reservas realizadas.
@param numReservas: Número de reservas totais.
*/
void fechamentoDia(passageiro* reservas, int* n, int numReservasVal)
{
    FILE* passageiros = fopen("passageiros.txt", "w");
    if (!passageiros) {
        printf("Erro na abertura do arquivo passageiros.txt!\n");
        exit(1);
    }

    float posicao = 0;

    for (int i = 0; i < *n; i++) {
        if (reservas[i].cancelado) {
            continue;
        }

        posicao += reservas[i].valor;
        passageiro r = reservas[i];
        fprintf(passageiros, "%s %s %s %d %d %d %s %s %s %f %s %s\n",
            r.nome, r.sobrenome, r.CPF, r.dia, r.mes, r.ano, r.numVoo, r.assento, r.classe, r.valor, r.origem, r.destino);
    }

    fclose(passageiros);

    printf("Fechamento do dia:\n");
    printf("Quantidade de reservas: %d\n", numReservasVal);
    printf("Posição: %.2f\n", posicao);
    printf("--------------------------------------------------\n");
}

/*
Fecha o voo, finalizando a atual execução do programa.

Funcionamento: Atualiza o status de finalizado do voo no arquivo, atualiza o arquivo
de passageiros com as novas informações de cada um e escreve na tela as informações
dos passageiros e do voo, como o valor total arrecadado e o nome dos passageiros.

@param *reservas: Ponteiro para o vetor de reservas já realizadas.
@param *n: Ponteiro para o número total de reservas realizadas.
*/
void fechamentoVoo(passageiro* reservas, int* n)
{
    FILE* voos = fopen("voos.txt", "a+");
    if (!voos) {
        printf("Erro na abertura do arquivo voos.txt!\n");
        exit(1);
    }

    int v;
    float f;
    fscanf(voos, "%d, %f, %f, %d", &v, &f, &f, &v);

    if (v != 1) {   
        fprintf(voos, ", 1");
    }
    fclose(voos);

    FILE* passageiros = fopen("passageiros.txt", "w");
    if (!passageiros) {
        printf("Erro na abertura do arquivo passageiros.txt!\n");
        exit(1);
    }

    for (int i = 0; i < *n; i++) {
        if (reservas[i].cancelado) {
            continue;
        }

        passageiro r = reservas[i];
        fprintf(passageiros, "%s %s %s %d %d %d %s %s %s %f %s %s\n",
            r.nome, r.sobrenome, r.CPF, r.dia, r.mes, r.ano, r.numVoo, r.assento, r.classe, r.valor, r.origem, r.destino);
    }

    fclose(passageiros);

    printf("Voo Fechado!\n\n");
    passageiro r;
    float valorTotal = 0;

    for (int i = 0; i < *n; i++) {
        if (reservas[i].cancelado) {
            continue;
        }
        r = reservas[i];
        printf("%s\n", r.CPF);
        printf("%s %s\n", r.nome, r.sobrenome);
        printf("%s\n\n", r.assento);
        valorTotal += r.valor;
    }
    printf("Valor Total: %0.2f\n", valorTotal);
    printf("--------------------------------------------------\n");
}

/*
Verifica se o voo já está fechado.

Funcionamento: Abre o arquivo "voos.txt" no modo de leitura. Depois, lê os
valores presentes para verificar se o atributo "fechado", que está no final da
linha, é 1. Caso seja, significa que o voo está fechado.

@return retorna 1 se o voo estiver fechado e 0 se estiver aberto.
*/
int verificarVoo()
{
    FILE* voos = fopen("voos.txt", "r");
    if (!voos) {
        printf("Erro na abertura do arquivo voos.txt!\n");
        exit(1);
    }

    int assentos, fechado = 0;
    float valorEcon, valorExec;

    fscanf(voos, "%d, %f, %f, %d", &assentos, &valorEcon, &valorExec, &fechado);

    fclose(voos);
    return fechado;
}

/*
Libera o espaço na memória heap ocupado pela array de passageiros.

@param *reservas: Array de passageiros.
@param n: número total de reservas realizadas.
*/
void free_passageiros(passageiro* reservas, int n)
{
    for (int i = 0; i < n; i++) {
        free(reservas[i].nome);
        free(reservas[i].sobrenome);
    }
    free(reservas);
}