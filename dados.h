#ifndef DADOS_H_INCLUDED
#define DADOS_H_INCLUDED
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#define MAX_NOM_LENGTH 50

typedef struct dado
{
    char nom[MAX_NOM_LENGTH];
    float lat, lon;

} DADO;

typedef struct bloco
{
    DADO loc;
    struct bloco *prox;
} LOCAL_BLOC;                       /* Bloco da memoria dinâmica das localidades */

typedef struct paragem
{
    char nom[MAX_NOM_LENGTH];
    float dist;

} PARAGEM;

typedef struct blocop
{
    PARAGEM par;
    struct blocop *prox;
} PARAG_BLOC;                       /* Bloco da memoria dinâmica das paregens */

typedef struct dador
{
    int num;
    float disTot;
    PARAG_BLOC *topo;
} ROTA;

typedef struct blocor
{
    ROTA rot;
    struct blocor *prox;
} ROTA_BLOC;                       /* Bloco da memoria dinâmica das rotas */

/*Funções de processamento da memoria dinâmica das localidades*/
LOCAL_BLOC *guarda_dados_na_pilha(LOCAL_BLOC *topo, LOCAL_BLOC *adi);
LOCAL_BLOC *remove_dados_na_pilha(LOCAL_BLOC *topo, char *nom);
LOCAL_BLOC *leitura_de_localidades(FILE *fpl, LOCAL_BLOC *topo);

/*Funções de processamento da memoria dinâmica das rotas e localidades e rotas*/
ROTA_BLOC *criaRota(int n, char *s, PARAG_BLOC **ant);
ROTA_BLOC *guarda_rotas_na_pilha(ROTA_BLOC *topo, ROTA_BLOC *novo);
ROTA_BLOC *leitura_rota(FILE *fpr, int rotas_nome, LOCAL_BLOC *topo);
ROTA_BLOC *leitura_rotas(FILE *fpr, LOCAL_BLOC *topo);
PARAG_BLOC *adicionaParagem(PARAG_BLOC *ant, char *s);
int completa_distancias(LOCAL_BLOC *localidade, ROTA_BLOC *rota);

/*Funções de escrita de memoria dinâmica*/
void escreve_localidades(LOCAL_BLOC *topo, FILE *fpl);
void escreve_rotas(ROTA_BLOC *topor, FILE *fp);

/*Funções diversas*/
float distancia(float lon, float lat);
void abrir_ficheiro(FILE **file, char* nome);
float num_local(LOCAL_BLOC *topo);
void formata(char* nome);
int ja_existe(LOCAL_BLOC *topo, char *nom);
void teste_parametros(float lat,float lon,int contador);

#endif
