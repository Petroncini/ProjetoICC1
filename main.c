#include <stdlib.h>
#include <stdio.h>
#include <string.h>


typedef struct{
    int assentos;
    float valorEcon;
    float valorExec;
} voo;

typedef struct{
    char* nome;
    char* sobrenome;
    int CPF;
    char data[11]; //dd/mm/yyyy 
    int numVoo;
    int assento;
    int classe; //econ, 0 | exec, 1;
    float valor;
    char origem[3]; // código de aeroposto e.g. CGH;
    char destino[3];
} passageiro;


int encodeComand(char *comando);
void abrirVoo();
void realizarReserva(passageiro *reservas,  int *n);
void consultarReserva();
void modificarReserva();
void cancelarReserva();
void fechamentoDia(passageiro *reservas, int *n);
void fechamentoVoo();
void carregarReservas(passageiro *reservas, int *n);
char* leiaString();

int main(void){
    
    char *comando;
    int comandoEncoded = -1;

    passageiro *reservas;
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
                fechamentoVoo();
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

    do {
        fgets(linha, 1000, passageiros);
        sscanf(linha, "%s %s %d %s %d %d %d %f %s %s", r.nome, r.sobrenome, &r.CPF, r.data, &r.numVoo, &r.assento, &r.classe, &r.valor, r.origem, r.destino);

        reservas[(*n)++] = r;
    } while(linha != NULL);

}

void realizarReserva(passageiro *reservas,  int *n){
    passageiro r;
   
    scanf("%s %s %d %s %d %d %d %f %s %s", r.nome, r.sobrenome, &r.CPF, r.data, &r.numVoo, &r.assento, &r.classe, &r.valor, r.origem, r.destino);
    reservas[(*n)++] = r;
    
}

void consultarReserva(){

}

void modificarReserva(){

}

void cancelarReserva(){

}

void fechamentoDia(passageiro *reservas, int *n){
    FILE *passageiros;
    passageiros = fopen("passageiros.txt", "w");

    for (int i = 0; i < *n; i++)
    {
        passageiro r = reservas[i];
        fprintf(passageiros, "%s %s %d %s %d %d %d %f %s %s", r.nome, r.sobrenome, r.CPF, r.data, r.numVoo, r.assento, r.classe, r.valor, r.origem, r.destino);
    }
    
}

void fechamentoVoo(){

}