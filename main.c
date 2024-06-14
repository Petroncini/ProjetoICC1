#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int encodeComand(char* comando);
void criarArquivos();
void abrirVoo(int* numAssentos);
int vooAberto();
void realizarReserva(passageiro** reservas, int* n, int assentos, int* numReservasVal);
void consultarReserva(passageiro* reservas, int n);
void modificarReserva(passageiro* reservas, int* n);
void cancelarReserva(passageiro* reservas, int* n, int* numReservasVal);
void fechamentoDia(passageiro* reservas, int* n, int numReservas);
void fechamentoVoo(passageiro* reservas, int* n);
void carregarReservas(passageiro** reservas, int* n, int* numReservasVal);
int verificarVoo();
void free_passageiros(passageiro* reservas, int n);

int main(void)
{
    char comando[3];
    int comandoEncoded = -1;

    passageiro* reservas = malloc(10 * sizeof(passageiro));

    int numReservas = 0;
    int numReservasVal = 0;

    criarArquivos();

    carregarReservas(&reservas, &numReservas, &numReservasVal);
    int numAssentos = vooAberto();

    if (verificarVoo()) {
        fechamentoVoo(reservas, &numReservas);
    }

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
            fechamentoDia(reservas, &numReservas, numReservas);
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

void fechamentoDia(passageiro* reservas, int* n, int numReservas)
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
    printf("Quantidade de reservas: %d\n", numReservas);
    printf("Posição: %.2f\n", posicao);
    printf("--------------------------------------------------\n");
}

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

void free_passageiros(passageiro* reservas, int n)
{
    for (int i = 0; i < n; i++) {
        free(reservas[i].nome);
        free(reservas[i].sobrenome);
    }
    free(reservas);
}