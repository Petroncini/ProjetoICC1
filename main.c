#include <stdlib.h>
#include <stdio.h>
#include <string.h>


typedef struct{
    int assentos;
    float valorEcon;
    float valorExec;
} voo;

typedef struct{
    char nome[50];
    char sobrenome[50];
    char CPF[20];
    int dia;
    int mes;
    int ano; //dd/mm/yyyy 
    char numVoo[10];
    char assento[10];
    char classe[20]; //econ, 0 | exec, 1;
    float valor;
    char origem[5]; // código de aeroposto e.g. CGH;
    char destino[5];
} passageiro;


int encodeComand(char *comando);
void abrirVoo();
void realizarReserva(passageiro *reservas,  int *n);
void consultarReserva();
void modificarReserva();
void cancelarReserva();
void fechamentoDia(passageiro *reservas, int *n);
void fechamentoVoo(passageiro *reservas, int *n);
void carregarReservas(passageiro *reservas, int *n);
char* leiaString();

int main(void){
    
    char *comando;
    int comandoEncoded = -1;

    passageiro *reservas = malloc(10 * sizeof(passageiro)); //aqui tem que fazer um realloc mais dinamico
    int numReservas = 0;
    
    carregarReservas(reservas, &numReservas);

    do {
        
        comando = leiaString();
        comandoEncoded = encodeComand(comando);
        
        switch(comandoEncoded){
            case 0:
                abrirVoo();
                break;
            case 1:
                realizarReserva(reservas, &numReservas);
                break;
            case 2:
                consultarReserva();
                break;
            case 3:
                modificarReserva();
                break;
            case 4:
                cancelarReserva();
                break;
            case 5:
                fechamentoDia(reservas, &numReservas);
                break;
            case 6:
                fechamentoVoo(reservas, &numReservas);
        }
    } while(comandoEncoded != 5 && comandoEncoded != 6);
    
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
    } else if(comando[1] == 'A'){
        return 6; //FA
    } else {
        return 5; //FD
    }
}

void abrirVoo(){
    FILE *voos;
    voos = fopen("voos.txt", "w");
    char comando[3];

    int assentos;
    float valorEcon;
    float valorExec;

    scanf(" %d %f %f", &assentos, &valorEcon, &valorExec);
    getchar();

    fprintf(voos, "%d, %f, %f\n", assentos, valorEcon, valorExec);

    //hello, veiga
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

void carregarReservas(passageiro *reservas, int *n){
    
    FILE *passageiros;
    passageiros = fopen("passageiros.txt", "r");


    passageiro r;
    char * linha = malloc(200 * sizeof(char));

    while(fgets(linha, 1000, passageiros) != NULL) {
        printf("%s", linha);
         sscanf(linha, "%s %s %s %d %d %d %s %s %s %f %s %s",
               r.nome, r.sobrenome, r.CPF, &r.dia, &r.mes, &r.ano,
               r.numVoo, r.assento, r.classe, &r.valor, r.origem, r.destino);
        reservas[(*n)++] = r;
    } 

    fclose(passageiros);
}

void realizarReserva(passageiro *reservas,  int *n){
    passageiro r;
    // RR Euclides Simon 222.111.333-12 12 12 2024 V001 B01 economica 1200.00 CGH RAO
    
    scanf("%s %s %s %d %d %d %s %s %s %f %s %s",
               r.nome, r.sobrenome, r.CPF, &r.dia, &r.mes, &r.ano,
               r.numVoo, r.assento, r.classe, &r.valor, r.origem, r.destino);
    getchar();

    reservas[(*n)++] = r;
    
}

void consultarReserva(){

}

void modificarReserva(){

}

void cancelarReserva(){
    // aqui eu acho que é melhor editar o documento e recarregar as reservas... talvez ¯\_(ツ)_/¯
}

void fechamentoDia(passageiro *reservas, int *n){
    FILE *passageiros;
    passageiros = fopen("passageiros.txt", "w");

    for (int i = 0; i < *n; i++)
    {
        passageiro r = reservas[i];
        fprintf(passageiros, "%s %s %s %d %d %d %s %s %s %f %s %s\n", r.nome, r.sobrenome, r.CPF, r.dia, r.mes, r.ano, r.numVoo, r.assento, r.classe, r.valor, r.origem, r.destino);
    }
    
}

void fechamentoVoo(passageiro *reservas, int *n){
    printf("flag\n");
    FILE *passageiros;
    passageiros = fopen("passageiros.txt", "w");

    printf("Voo Fechado!\n");
    passageiro r;
    float valorTotal = 0;

    for (int i = 0; i < *n; i++)
    {
        r = reservas[i];
        printf("%s\n", r.CPF);
        printf("%s %s\n", r.nome, r.sobrenome);
        printf("%s\n\n", r.assento);
        valorTotal += r.valor;
    }
    printf("Valor Total: %f\n", valorTotal);
    printf("--------------------------------------------------\n");
    
}