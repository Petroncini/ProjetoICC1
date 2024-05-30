#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int encodeComand(char comando[3]);
void abrirVoo(char *entrada);
void realizarReserva(char *entrada);
void consultarReserva(char *entrada);
void modificarReserva(char *entrada);
void cancelarReserva(char *entrada);
void fechamentoDia();
void fechamentoVoo();

struct voo{
    int assentos;
    float valorEcon;
    float valorExec;
};

struct passageiro{
    char* nome;
    char* sobrenome;
    int CPF;
    char data[10]; //dd/mm/yyyy 
    int numVoo;
    int assento;
    int classe; //econ, 0 | exec, 1;
    float valor;
    char origem[3]; // código de aeroposto e.g. CGH;
    char destino[3];
};

int main(void){
    
    char comando[3];
    int comandoEncoded = -1;

    do {
        char* entrada = malloc(500 * sizeof(char));
        fgets(entrada, 500, stdin);

        sscanf(entrada, "%[^ ]", comando);

        comandoEncoded = encodeComand(comando);
        
        switch(comandoEncoded){
            case 0:
                abrirVoo(entrada);
                break;
            case 1:
                realizarReserva(entrada);
                break;
            case 2:
                consultarReserva(entrada);
                break;
            case 3:
                modificarReserva(entrada);
                break;
            case 4:
                cancelarReserva(entrada);
                break;
            case 5:
                fechamentoDia();
                break;
            case 6:
                fechamentoVoo();
        }
    } while(comandoEncoded != 5 && comandoEncoded != 6);
    
    return 0;
}

int encodeComand(char comando[3]){
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

void abrirVoo(char *entrada){
    FILE *voos;
    voos = fopen("voos.txt", "w");
    char comando[3];

    int assentos;
    float valorEcon;
    float valorExec;

    sscanf(entrada, " %[^ ] %d %f %f", comando, &assentos, &valorEcon, &valorExec);
    printf("Assentos: %d, valorEcon: %f, valorExec %f\n", assentos, valorEcon, valorExec);

    //hello, veiga
}

char* leiaString() {
    char *buffer = malloc(1 * sizeof(char));
    int size = 1, id = 0;
    char ch;
    getchar();

    while ((ch = getchar()) != ',' && ch != EOF) {
        if (id == size - 1) {
            size *= 2;
            buffer = realloc(buffer, size * sizeof(char));
        }
        buffer[id++] = ch;
    }
    buffer[id] = '\0';

    return buffer;
}

void realizarReserva(char *entrada){

}

void consultarReserva(char *entrada){

}

void modificarReserva(char *entrada){

}

void cancelarReserva(char *entrada){

}

void fechamentoDia(){

}

void fechamentoVoo(){

}