#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int assentos;
    float valorEcon;
    float valorExec;
} voo;

typedef struct {
    char* nome;
    char* sobrenome;
    char CPF[20];
    int dia;
    int mes;
    int ano; // dd/mm/yyyy
    char numVoo[10];
    char assento[10];
    char classe[20];
    float valor;
    char origem[5]; // código de aeroposto e.g. CGH;
    char destino[5];
    int cancelado;
} passageiro;

int encodeComand(char* comando);
void criarArquivos();
void abrirVoo(int* numAssentos);
int vooAberto();
void realizarReserva(passageiro** reservas, int* n, int* numReservasDia, int assentos);
void consultarReserva(passageiro* reservas, int n);
void modificarReserva(passageiro* reservas, int* n);
void cancelarReserva(passageiro* reservas, int n);
void fechamentoDia(passageiro* reservas, int* n, int numReservasDia);
void fechamentoVoo(passageiro* reservas, int* n);
void carregarReservas(passageiro** reservas, int* n);
void free_passageiros(passageiro* reservas, int n);

int main(void)
{
    char comando[3];
    int comandoEncoded = -1;

    passageiro* reservas = malloc(10 * sizeof(passageiro));

    int numReservas = 0;
    int numReservasDia = 0;

    criarArquivos();

    carregarReservas(&reservas, &numReservas);
    int numAssentos = vooAberto();

    do {
        scanf("%s", comando);
        getchar();

        comandoEncoded = encodeComand(comando);

        switch (comandoEncoded) {
        case 0:
            abrirVoo(&numAssentos);
            break;
        case 1:
            realizarReserva(&reservas, &numReservas, &numReservasDia, numAssentos);
            break;
        case 2:
            consultarReserva(reservas, numReservas);
            break;
        case 3:
            modificarReserva(reservas, &numReservas);
            break;
        case 4:
            cancelarReserva(reservas, numReservas);
            break;
        case 5:
            fechamentoDia(reservas, &numReservas, numReservasDia);
            break;
        case 6:
            fechamentoVoo(reservas, &numReservas);
            break;
        }
    } while (comandoEncoded != 5 && comandoEncoded != 6);

    free_passageiros(reservas, numReservas);
    return 0;
}

int encodeComand(char* comando)
{
    if (comando[0] == 'A' && comando[1] == 'V') { // AV
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

void criarArquivos()
{
    FILE *voos = fopen("voos.txt", "r");
    FILE *passageiros = fopen("passageiros.txt", "r");

    if(!voos) {
        voos = fopen("voos.txt", "w");
    }

    if(!passageiros) {
        passageiros = fopen("passageiros.txt", "w");
    }

    fclose(voos);
    fclose(passageiros);
}

void abrirVoo(int* numAssentos)
{
    FILE* voos = fopen("voos.txt", "w");
    if (!voos) {
        printf("Erro na abertura do arquivo voos.txt!\n");
        exit(1);
    }

    int assentos;
    float valorEcon;
    float valorExec;

    scanf("%d %f %f", &assentos, &valorEcon, &valorExec);
    getchar();

    fprintf(voos, "%d, %f, %f\n", assentos, valorEcon, valorExec);
    fclose(voos);
    *numAssentos = assentos;
}

int vooAberto()
{
    FILE* voos = fopen("voos.txt", "r");
    if (!voos) {
        printf("Erro na abertura do arquivo voos.txt!\n");
        exit(1);
    }

    int assentos;
    fscanf(voos, "%d", &assentos);

    fclose(voos);
    return assentos;
}

void carregarReservas(passageiro** reservas, int* n)
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

        if (*n % 10 == 0) {
            *reservas = realloc(*reservas, (*n + 10) * sizeof(passageiro));
        }
    }
    fclose(passageiros);
}

void realizarReserva(passageiro** reservas, int* n, int* numReservasDia, int assentos)
{
    if (assentos == -1) {
        printf("Nenhum voo aberto\n");
        return;
    } else if (*n >= assentos) {
        printf("Nenhum assento disponível\n");
        return;
    }

    passageiro r;
    char nome[50], sobrenome[50];

    scanf("%s %s %s %d %d %d %s %s %s %f %s %s",
        nome, sobrenome, r.CPF, &r.dia, &r.mes, &r.ano,
        r.numVoo, r.assento, r.classe, &r.valor, r.origem, r.destino);
    getchar();

    r.nome = malloc((strlen(nome) + 1) * sizeof(char));
    r.sobrenome = malloc((strlen(sobrenome) + 1) * sizeof(char));
    r.cancelado = 0;

    strcpy(r.nome, nome);
    strcpy(r.sobrenome, sobrenome);

    (*reservas)[(*n)++] = r;

    if (*n % 10 == 0) {
        *reservas = realloc(*reservas, (*n + 10) * sizeof(passageiro));
    }

    (*numReservasDia)++;
}

void consultarReserva(passageiro* reservas, int n)
{
    /***Essa função tem o objetivo de consultar a reserva do voo de um passageiro a partir do ceu CPF***/

    /*Realizo a leitura do CPF*/
    char cpf[20];
    scanf(" %s", cpf);

    /*Realizo um loop que busca todos os passageiros já cadastrados*/
    for (int i = 0; i < n; i++) {
        passageiro r;
        r = reservas[i];
        /*Verifico nesse if se o CPF cadastrado é o mesmo buscado, além de verificar se o passageiro já não cancelou seu voo*/
        if (strcmp(r.CPF, cpf) == 0 && r.cancelado!=1) {
            /*Escrevo a resposta esperada*/
            printf("%s\n%s %s\n%d/%d/%d\nVoo: %s\nAssento: %s\nClasse: %s\nTrecho: %s %s\nValor: %0.2f\n", 
                r.CPF, r.nome, r.sobrenome, r.dia, r.mes, r.ano, r.numVoo, r.assento, r.classe, r.origem, r.destino, r.valor);
            printf("--------------------------------------------------\n");
        }
    }
}

void modificarReserva(passageiro* reservas, int* n)
{
    /***Essa função tem o objetivo de modificar os dados de uma reserva já existente de algum dos passageiros***/

    /*Leio o CPF antigo, para fazer a busca, e o nome, sobrenome, CPF e assento que podem ser modificados*/
    char CPFAntigo[20], nome[50], sobrenome[50], CPF[20], assento[10];
    scanf("%s %s %s %s %s", CPFAntigo, nome, sobrenome, CPF, assento);

    /*Faço um loop buscando o CPFAntigo na base de dados*/
    for (int i = 0; i < *n; i++) {
        /*Encontra o CPFAntigo*/
        if (!strcmp(CPFAntigo, reservas[i].CPF)) {
            /*Libera o espaço alocado dinamicamente para o antigo nome e sobrenome*/
            free(reservas[i].nome);
            free(reservas[i].sobrenome);

            /*Aloca espaço na memória para o novo nome e sobrenome*/
            reservas[i].nome = malloc((strlen(nome) + 1) * sizeof(char));
            reservas[i].sobrenome = malloc((strlen(sobrenome) + 1) * sizeof(char));

            /*Modifica os dados necessários do passageiro*/
            strcpy(reservas[i].nome, nome);
            strcpy(reservas[i].sobrenome, sobrenome);
            strcpy(reservas[i].CPF, CPF);
            strcpy(reservas[i].assento, assento);

            /*Escreve para o usuário os novos dados do passageiro cujo a reserva foi modificada*/
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

void cancelarReserva(passageiro* reservas, int n)
{
    /***Essa função tem o objetivo de cancelar a reserva do voo de um passageiro a partir do ceu CPF***/

    /*Realizo a leitura do CPF*/
    char cpf[20];
    scanf(" %s", cpf);

    /*Realizo um loop que busca todos os passageiros já cadastrados*/
    for (int i = 0; i < n; i++) {
        /*Verifico nesse if se o CPF cadastrado é o mesmo buscado*/
        if (!strcmp(reservas[i].CPF, cpf)) {
            /*Coloco o atributo de "cancelado" da pessoa para 1(True)*/
            reservas[i].cancelado = 1;
        }
    }
}

void fechamentoDia(passageiro* reservas, int* n, int numReservasDia)
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
    printf("Quantidade de reservas: %d\n", numReservasDia);
    printf("Posição: %.2f\n", posicao);
    printf("--------------------------------------------------\n");
}

void fechamentoVoo(passageiro* reservas, int* n)
{
    remove("voos.txt");
    remove("passageiros.txt");

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

void free_passageiros(passageiro* reservas, int n)
{
    for (int i = 0; i < n; i++) {
        free(reservas[i].nome);
        free(reservas[i].sobrenome);
    }
    free(reservas);
}