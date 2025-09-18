#ifndef ARGUMENTOS_H
#define ARGUMENTOS_H
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

enum Comportamento          /*Os comportamentos estão categorizados segundo este enum. Cada comportamento tem um número associado*/
{
    Comportamento_indefinido,
    Comportamento_leitura,
    Comportamento_adiciona,
    Comportamento_remocao,
    Comportamento_distancia,
    Comportamento_rota
};

typedef struct Argumentos   /*Estrutura que armazena a informação da linha de comandos*/
{
    /* Nome do ficheiro de output das localidades */
    char *localidades_output_txt;
    /* Com base nas flags, ajuda a consultar o comportamento esperado do programa */
    enum Comportamento comportamento;
    /* Valores para adicao de localidade */
    char *localidade_adi_nome;
    float localidade_adi_latitude;
    float localidade_adi_longitude;
    /* Valores para remocao de localidade */
    char *localidade_rem_nome;
    /* Valores para processamento de rotas */
    int rotas_nome;
    char *rotas_output_txt;
} TipoArgumentos;

void iniciarArgumentos(TipoArgumentos *argumentos);
int verificaComportamentoUnico(TipoArgumentos *argumentos);
int processaArgumentos(TipoArgumentos *argumentos, int argc, char *argv[]);
int processaOutput(TipoArgumentos *argumentos, int argc, char *argv[], int *contador);
int processaOutputRota(TipoArgumentos *argumentos, int argc, char *argv[], int *contador);
int processaAdiciona(TipoArgumentos *argumentos, int argc, char *argv[], int *contador);
int processaRemove(TipoArgumentos *argumentos, int argc, char *argv[], int *contador);
int processaRota(TipoArgumentos *argumentos, int argc, char *argv[], int *contador);
int processaLeitura(TipoArgumentos *argumentos);

#endif /* ARGUMENTOS_H */
