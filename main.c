#include <stdlib.h>
#include <stdio.h>
#include <string.h>


typedef struct{
    int assentos;
    float valorEcon;
    float valorExec;
} voo;

typedef struct{
    char *nome;
    char *sobrenome;
    char CPF[20];
    int dia;
    int mes;
    int ano; //dd/mm/yyyy 
    char numVoo[10];
    char assento[10];
    char classe[20];
    float valor;
    char origem[5]; // código de aeroposto e.g. CGH;
    char destino[5];
    int cancelado;
} passageiro;


int encodeComand(char *comando);
void abrirVoo(int *numAssentos); //DONE
int vooAberto();
void realizarReserva(passageiro **reservas,  int *n, int *numReservasDia, int assentos); //DONE
void consultarReserva(passageiro *reservas, int n); //Done, Nicolas
void modificarReserva(passageiro *reservas, int *n); //TODO, Veiga
void cancelarReserva(passageiro *reservas, int n); //Done, Nicolas
void fechamentoDia(passageiro *reservas, int *n, int numReservasDia); //DONE
void fechamentoVoo(passageiro *reservas, int *n); //DONE
void carregarReservas(passageiro **reservas, int *n); //DONE
char* leiaString();
void free_passageiros(passageiro *reservas, int n);

int main(void){
    
    char comando[3];
    int comandoEncoded = -1;

    passageiro *reservas = malloc(10 * sizeof(passageiro)); //aqui tem que fazer um realloc mais dinamico, DONE
    int numReservas = 0;
    int numReservasDia = 0;
    
    carregarReservas(&reservas, &numReservas);
    int numAssentos;
    numAssentos = vooAberto();
    
    do {
        
        scanf("%s", comando);
        getchar();

        comandoEncoded = encodeComand(comando);
        
        switch(comandoEncoded){
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
        }
    } while(comandoEncoded != 5 && comandoEncoded != 6);
    
    free_passageiros(reservas, numReservas);
    return 0;
}

int encodeComand(char *comando){
    if(comando[0] == 'A' && comando[1] == 'V'){ //AV
        return 0; // AV
    } else if(comando[0] == 'C'){
        if(comando[1] == 'R'){
            return 2; // CR
        } else {
            return 4; // CA
        }
    } else if(comando[1] == 'R'){
        if(comando[0] == 'R'){
            return 1; //RR
        } else {
            return 3; //MR
        }
    } else if(comando[1] == 'V'){
        return 6; //FA
    } else {
        return 5; //FD
    }
}

void abrirVoo(int *numAssentos){
    FILE *voos;
    voos = fopen("voos.txt", "w");

    int assentos;
    float valorEcon;
    float valorExec;

    scanf(" %d %f %f", &assentos, &valorEcon, &valorExec);
    getchar();

    fprintf(voos, "%d, %f, %f\n", assentos, valorEcon, valorExec);
    fclose(voos);
    *numAssentos = assentos;
    //hello, veiga
    //hello, nicolas
}

int vooAberto(){
    FILE *voos;
    voos = fopen("voos.txt", "r");
    char *voos_texto = malloc(50 * sizeof(char));
    int assentos;

    if(fgets(voos_texto, 50, voos) == NULL){
        return -1;
    }
    
    sscanf(voos_texto, "%d", &assentos);
    free(voos_texto);
    return assentos;
}

char* leiaString() {
    char *buffer = malloc(1 * sizeof(char));
    int size = 1, id = 0;
    char ch;

    while ((ch = getchar()) != ' ' && ch != EOF && ch != '\n') {
        if (id == size - 1) {
            size *= 2;
            buffer = realloc(buffer, size * sizeof(char));
        }
        buffer[id++] = ch;
    }
    buffer[id] = '\0';

    return buffer;
}

void carregarReservas(passageiro **reservas, int *n){
    
    FILE *passageiros;
    passageiros = fopen("passageiros.txt", "r");


    passageiro r;
    char * linha = malloc(200 * sizeof(char));

    char *nome = malloc(50 * sizeof(char));
    char *sobrenome = malloc(50 * sizeof(char));

    while(fgets(linha, 200, passageiros) != NULL) {
        printf("%s", linha);
        sscanf(linha, "%s %s %s %d %d %d %s %s %s %f %s %s",
               nome, sobrenome, r.CPF, &r.dia, &r.mes, &r.ano,
               r.numVoo, r.assento, r.classe, &r.valor, r.origem, r.destino);
        
        r.nome = malloc(strlen(nome));
        r.sobrenome = malloc(strlen(sobrenome));
        r.cancelado = 0;

        strcpy(r.nome, nome);
        strcpy(r.sobrenome, sobrenome);

        (*reservas)[(*n)++] = r;

        if(*n%10 == 0){
            *reservas = realloc(*reservas, (*n + 10) * sizeof(passageiro));
        }
    } 
    free(linha);
    free(nome);
    free(sobrenome);
    fclose(passageiros);
}

void realizarReserva(passageiro **reservas,  int *n, int *numReservasDia, int assentos){
    passageiro r;
    // RR Euclides Simon 222.111.333-12 12 12 2024 V001 B01 economica 1200.00 CGH RAO
    // RR Marta Rocha 999.888.222-21 12 12 2024 V001 C02 executiva 2500.00 CGH RAO
    r.nome = leiaString();
    r.sobrenome = leiaString();
    r.cancelado = 0;
    
    
    scanf("%s %d %d %d %s %s %s %f %s %s",
               r.CPF, &r.dia, &r.mes, &r.ano,
               r.numVoo, r.assento, r.classe, &r.valor, r.origem, r.destino);
    getchar();

    if(assentos == -1){
        printf("Nenhum voo aberto\n");
        return;
    } else if(*n >= assentos){
        printf("Nenhum assento disponível\n");
        return;
    }
    
    (*reservas)[(*n)++] = r;

    if(*n%10 == 0){
            *reservas = realloc(*reservas, (*n + 10) * sizeof(passageiro));
    }

    (*numReservasDia)++;
}

void consultarReserva(passageiro *reservas, int n){
    char cpf[20];
    scanf(" %s", cpf);

    for (int i=0; i<n; i++){
        passageiro r;
        r = reservas[i];
        if (strcmp(r.CPF, cpf)==0){
            printf("%s\n%s %s\n%d/%d/%d\nVoo: %s\nAssento: %s\nClasse: %s\nTrecho: %s %s\nValor: %0.2f\n", r.CPF, r.nome, r.sobrenome, r.dia, r.mes, r.ano, r.numVoo, r.assento, r.classe, r.origem, r.destino, r.valor);
            printf("--------------------------------------------------\n");
        }
    }

}

void modificarReserva(passageiro *reservas, int *n){
    // MR Marta Rocha 999.888.222-21 C03
    char *nome, *sobrenome, CPF[20], assento[10];
    nome = malloc(50 * sizeof(char));
    sobrenome = malloc(50 * sizeof(char));

    scanf("%s %s %s %s", nome, sobrenome, CPF, assento);
    int idx = -1;

    for (int i = 0; i < *n; i++) {
        if (!strcmp(nome, reservas[i].nome) || !strcmp(sobrenome, reservas[i].sobrenome) || !strcmp(CPF, reservas[i].CPF) || !strcmp(assento, reservas[i].assento)) {
            idx = i;
            strcpy(reservas[i].nome, nome);
            strcpy(reservas[i].sobrenome, sobrenome);
            strcpy(reservas[i].CPF, CPF);
            strcpy(reservas[i].assento, assento);
            break;
        }
    }

    printf("Reserva Modificada:\n");
    printf("%s\n", reservas[idx].CPF);
    printf("%s %s\n", reservas[idx].nome, reservas[idx].sobrenome);
    printf("%02d/%02d/%04d\n", reservas[idx].dia, reservas[idx].mes, reservas[idx].ano);
    printf("Voo: %s\n", reservas[idx].numVoo);
    printf("Assento: %s\n", reservas[idx].assento);
    printf("Classe: %s\n", reservas[idx].classe);
    printf("Trecho: %s %s\n", reservas[idx].origem, reservas[idx].destino);
    printf("Valor: %.2f\n", reservas[idx].valor);
    printf("--------------------------------------------------\n");
    free(nome);
    free(sobrenome);
}

void cancelarReserva(passageiro *reservas, int n){
    char cpf[20];
    scanf(" %s", cpf);

    for (int i=0; i<n; i++){
        if (strcmp(reservas[i].CPF, cpf)==0){
            reservas[i].cancelado = 1;
        }
    }

}

void fechamentoDia(passageiro *reservas, int *n, int numReservasDia){
    
    FILE *passageiros;
    passageiros = fopen("passageiros.txt", "w");

    float posicao = 0;

    for (int i = 0; i < *n; i++)
    {   
        if(reservas[i].cancelado){continue;}

        posicao += reservas[i].valor;
        passageiro r = reservas[i];
        fprintf(passageiros, "%s %s %s %d %d %d %s %s %s %f %s %s\n", r.nome, r.sobrenome, r.CPF, r.dia, r.mes, r.ano, r.numVoo, r.assento, r.classe, r.valor, r.origem, r.destino);
    }
    
    printf("Fechamento do dia:\n");
    printf("Quantidade de reservas: %d\n", numReservasDia);
    printf("Posição: %.2f\n", posicao);
    printf("--------------------------------------------------\n");
}

void fechamentoVoo(passageiro *reservas, int *n){
    FILE *passageiros;
    passageiros = fopen("passageiros.txt", "w");
    fclose(passageiros);

    FILE *voos;
    voos = fopen("voos.txt", "w");
    fclose(voos);

    printf("Voo Fechado!\n\n\n");
    passageiro r;
    float valorTotal = 0;

    for (int i = 0; i < *n; i++)
    {
        if(reservas[i].cancelado){continue;}
        r = reservas[i];
        printf("%s\n", r.CPF);
        printf("%s %s\n", r.nome, r.sobrenome);
        printf("%s\n\n", r.assento);
        valorTotal += r.valor;
    }
    printf("Valor Total: %0.2f\n", valorTotal);
    printf("--------------------------------------------------\n");
    
}

void free_passageiros(passageiro *reservas, int n){
    for (int i = 0; i < n; i++)
    {
        free(reservas[i].nome);
        free(reservas[i].sobrenome);
    }
    free(reservas);
}