#include "dados.h"
#include <stdio.h>

LOCAL_BLOC *guarda_dados_na_pilha(LOCAL_BLOC *topo, LOCAL_BLOC *adi)  /* Esta função insere um bloco de uma localidade na posição correta na memoria dinamica, ordenando por ordem alfabetica*/
{
    LOCAL_BLOC *step;
    LOCAL_BLOC *aux;
    LOCAL_BLOC *aux2;

    if(ja_existe(topo, adi->loc.nom)==0){                                           /* A função ja_existe é invocada e permite saber se na memória dinamica já existe a localidade a ser adicionada*/
        fprintf(stderr,"Ja existe uma instancia da localidade %s.\n",adi->loc.nom); /* A localiudade não é adicionada se já existir*/
        return topo;
    }

    step = (LOCAL_BLOC *)calloc(1, sizeof(LOCAL_BLOC));
    if (step == NULL)
    {
        printf("Erro - calloc \n");
        exit(0);
    }

    strcpy(step->loc.nom, adi->loc.nom);
    step->loc.lat = adi->loc.lat;
    step->loc.lon = adi->loc.lon;

    if (topo == NULL)
    {
        topo = step;
        topo->prox = NULL;
        return topo;
    }

    if (strcmp(adi->loc.nom, topo->loc.nom) < 0)
    {
        step->prox = topo;
        topo = step;
        return topo;
    }
    aux2 = topo;
    aux = topo->prox;
    while ((aux != NULL) && (strcmp(adi->loc.nom, aux->loc.nom) > 0))   /* ciclo de procura da posição da localidade*/
    {
        aux2 = aux;
        aux = aux->prox;
    }

        aux2->prox = step;
        step->prox = aux;

    return topo;
}

LOCAL_BLOC *remove_dados_na_pilha(LOCAL_BLOC *topo, char *nom)  /* Esta função procura na memória dinâmica o bloco da localidade a ser removida e de seguida remove o bloco*/
{
    LOCAL_BLOC *aux;
    LOCAL_BLOC *aux2;

    if (strcmp(nom, topo->loc.nom) == 0)
    {
        aux = topo;
        topo = topo->prox;
        free(aux);
        return topo;
    }
    aux = topo;
    while (aux != NULL)
    {
        if (strcmp(nom, aux->loc.nom) == 0)     /* Ciclo para encontrar a localidade a ser removida */
        {
            aux2->prox = aux->prox;
            free(aux);
            return topo;
        }
        aux2 = aux;
        aux = aux->prox;
    }
    fprintf(stderr,"Nao foi encontrada a localidade %s",nom);     /* Se não foi encontrada a localidade é impresso um erro */
    return topo;
}

LOCAL_BLOC *leitura_de_localidades(FILE *fpl, LOCAL_BLOC *topo)  /* Esta função lê as localidades do ficheiro e envia os valores para serem adicionadas a memória dinâmica*/
{
    LOCAL_BLOC locaux;
    LOCAL_BLOC *aux;
    int nc, count;
    char char_teste;
    count = 0;
    aux = &locaux;

    while (feof(fpl) == 0)
    {
        count++;
        nc = fscanf(fpl, "%s %f %f", aux->loc.nom, &aux->loc.lat, &aux->loc.lon);
        teste_parametros(aux->loc.lat,aux->loc.lon, count);                 /* Esta função aqui chamada permite determinar se os valores estão dentro dos parâmetros permitidos*/
        formata(aux->loc.nom);                              /* Formatação da string*/
        char_teste = fgetc(fpl);
        while (nc == 3 && char_teste == ' ')         /* Vai buscar o proximo carater sem ser um espaço*/
        {
            char_teste = fgetc(fpl);
        }
        if (char_teste == '\n' || char_teste == '\r' || feof(fpl) != 0)
        {
            topo = guarda_dados_na_pilha(topo, aux);
        }
        else
        {
            printf("Linha %d: Erro no numero de parametros", count);
            exit(0);
        }
    }
    return topo;
}

ROTA_BLOC *criaRota(int numrota, char *nomelocal, PARAG_BLOC **ant) /* A funçâo permite criar um bloco para a rota que aponta para um bloco da primeira paragem tambem criado */
{
    ROTA_BLOC *rota;
    PARAG_BLOC *paragem;
    rota = (ROTA_BLOC *)calloc(1, sizeof(ROTA_BLOC));
    if (rota == NULL)
    {
        printf("Erro - calloc \n");
        exit(0);
    }

    paragem = (PARAG_BLOC *)calloc(1, sizeof(PARAG_BLOC));
    if (paragem == NULL)
    {
        printf("Erro - calloc \n");
        exit(0);
    }
    rota->rot.num = numrota;
    strncpy(paragem->par.nom, nomelocal, MAX_NOM_LENGTH);
    rota->rot.topo = paragem;
    *ant = paragem;

    return rota;
}

ROTA_BLOC *guarda_rotas_na_pilha(ROTA_BLOC *topo, ROTA_BLOC *novo)  /* A função insere o bloco na pilha de rotas em função da sua distância total*/
{
    ROTA_BLOC *aux;
    ROTA_BLOC *aux2;

    if (topo == NULL)
    {
        topo = novo;
        topo->prox = NULL;
        return topo;
    }

    if (topo->rot.disTot > novo->rot.disTot)
    {
        novo->prox = topo;
        topo = novo;
        return topo;
    }
    aux2 = topo;
    aux = topo->prox;
    while ((aux != NULL) && (aux->rot.disTot < novo->rot.disTot))
    {
        aux2 = aux;
        aux = aux->prox;
    }

    aux2->prox = novo;
    novo->prox = aux;

    return topo;
}

ROTA_BLOC *leitura_rota(FILE *fpr, int rotas_nome, LOCAL_BLOC *topo) /* A função procura no ficheiro rotas.txt uma rota expecifíca e fecha o programa se nâo a encontrar */
{
    int nc, nrota, linha, existe;
    char teste_rota[6], nome[MAX_NOM_LENGTH];
    ROTA_BLOC *topor;
    PARAG_BLOC *ant;
    topor = NULL;
    linha = 0;
    existe = 0;
    fscanf(fpr, "%s %d", teste_rota, &nrota);
    while (feof(fpr) == 0)
    {
        if (strcmp(teste_rota, "#ROTA") == 0)    /* Testa se tresta se encontra a rota */
        { do
            { if (nrota == rotas_nome)
                { nc = fscanf(fpr, "%s\n", nome);
                    if (strcmp(nome, "#ROTA") == 0 )       /* Lê as localidades até encontrar a próxima rota */
                    {
                        if(completa_distancias(topo, topor)==0){            /* Completa o bloco com as distancias quando encontra a proxima rota*/
                        fprintf(stderr,"Rota inserida tem localidade que nao existe no ficheiro localidades.txt \n");
                        exit(0);}
                        fscanf(fpr, "%d\n", &nrota);
                        nc = 0;
                    }
                    else
                    {
                        formata(nome);
                        if (linha == 0)
                        {
                            topor = criaRota(nrota, nome, &ant);      /* Quando lê a primeira paragem invoca a função para criar o bloco da rota a apontar para a primeira paragem */
                            existe = 1;
                        }
                        else
                        {
                            ant = adicionaParagem(ant, nome);
                        }
                        if (feof(fpr) != 0) {
                        if(completa_distancias(topo, topor)==0){            /* Se chegar ao fim do ficheiro completa o bloco */
                        fprintf(stderr,"Rota inserida tem localidade que nao existe no ficheiro localidades.txt \n");
                        exit(0);}      /* Se chegar ao fim do ficheiro completa o bloco */
                        }
                        linha++; }}
                else
                {
                    while (nrota != rotas_nome && feof(fpr) == 0)
                    {
                        fscanf(fpr, "%s %d", teste_rota, &nrota);    /* Fica à procura da localidade */
                    }
                    nc = 1;
                }
            } while (nc == 1 && feof(fpr) == 0);
        }}
    if(existe == 0){
        fprintf(stderr,"A rota inserida nao foi encontrada\n");      /* Se não encontrar a rota dá erro e fecha o programa */
        exit(0);
    }
    return topor;}

ROTA_BLOC *leitura_rotas(FILE *fpr, LOCAL_BLOC *topo)    /* A funçâo lê o ficheiro rotas.txt e guarda os dados para todas as rotas e suas paragens para serem guardados numa lista de listas*/
{                                                           /* Funciona de forma semelhante ao leitura rota no entanto le todas as rotas */
    int nc, nrota, linha;
    char teste_rota[6], nome[MAX_NOM_LENGTH];
    ROTA_BLOC *topor, *novo;
    PARAG_BLOC *ant;
    topor = NULL;
    linha = 0;

    fscanf(fpr, "%s %d", teste_rota, &nrota);
        while (feof(fpr) == 0)
        {
            if (strcmp(teste_rota, "#ROTA") == 0)
            {
                do{
                    nc = fscanf(fpr, "%s\n", nome);
                    if (strcmp(nome, "#ROTA") == 0)
                    {
                        if(completa_distancias(topo, novo)==1){
                            topor = guarda_rotas_na_pilha(topor, novo);}
                        else {fprintf(stderr,"Rota %d tem localidade que nao existe no ficheiro localidades.txt \n", nrota);}
                        fscanf(fpr, "%d\n", &nrota);
                        ant = NULL;
                        linha = 0;
                        nc = 0;
                    }
                    else
                    {
                        formata(nome);
                        if (linha == 0)
                        {
                            novo = criaRota(nrota, nome, &ant);
                        }
                        else
                        {
                            ant = adicionaParagem(ant, nome);
                        }
                        linha++;
                    }
                } while (nc == 1 && feof(fpr) == 0);
            }
            else{
                fprintf(stderr,"Rota 1 inserida incorretamente. Insira por exemplo: #ROTA 1");
                exit(0);
            }
        }
        if(completa_distancias(topo, novo)==1){
            topor = guarda_rotas_na_pilha(topor, novo);}
        else {fprintf(stderr,"Rota %d tem localidade que nao existe no ficheiro localidades.txt \n", nrota);}

    return topor;
}

PARAG_BLOC *adicionaParagem(PARAG_BLOC *ant, char *nomeparag)   /* A função permite criar um bloco para a rota que aponta para um bloco da primeira paragem também criado */
{
    PARAG_BLOC *novo;
    novo = (PARAG_BLOC *)calloc(1, sizeof(PARAG_BLOC));
    if (novo == NULL)
    {
        printf("Erro - calloc \n");
        exit(0);
    }
    strncpy(novo->par.nom, nomeparag, MAX_NOM_LENGTH);
    ant->prox = novo;
    return novo;
}

int completa_distancias(LOCAL_BLOC *localidade, ROTA_BLOC *rota) /* Depois de todas as paragens serem adicionadas esta função é chamada e insere as distancias parciais e a distancia total */
{
    int teste_paragem;
    float antlat, antlon;
    float varlat, varlon;
    float dis;
    PARAG_BLOC *auxpar;
    LOCAL_BLOC *auxloc;
    dis = 0;
    teste_paragem=0;
    for (auxpar = rota->rot.topo; auxpar != NULL; auxpar = auxpar->prox)   /* Ciclo das paragens */
    {
        teste_paragem=0;
        for (auxloc = localidade; auxloc != NULL; auxloc = auxloc->prox)   /* Ciclo das localidades */
        {
            if (strcmp(auxloc->loc.nom, auxpar->par.nom) == 0)             /* Encontra-se a localidade correspondente para calcular as distâncias */
            {
                if (auxpar == rota->rot.topo)
                {
                    auxpar->par.dist = 0.0;
                    antlat = auxloc->loc.lat;
                    antlon = auxloc->loc.lon;
                }
                else
                {
                    varlat = auxloc->loc.lat - antlat;
                    varlon = auxloc->loc.lon - antlon;
                    auxpar->par.dist = dis + distancia(varlat, varlon);
                    dis = auxpar->par.dist;
                    antlat = auxloc->loc.lat;
                    antlon = auxloc->loc.lon;
                }
                teste_paragem=1;
                rota->rot.disTot = dis;
            }

        }
        if(teste_paragem==0){                               /* Mecanismo para detetar se existem todas as localidades no ficheiro localidades*/
                    return teste_paragem;}
    }
    return teste_paragem;
}

void escreve_localidades(LOCAL_BLOC *topo, FILE *fpl)       /* A função permite escrever o número de localidades, a memória utilizada, e todas as localidades */
{
    float nlocal;
    nlocal=num_local(topo);
    fprintf(fpl, "Numero de localidades: %.0f \n", nlocal);
    fprintf(fpl, "Memoria requesitada: %.0f bytes \n", nlocal*sizeof(LOCAL_BLOC));

    for (; topo != NULL; topo = topo->prox)
    {
        fprintf(fpl, "%s %.2f %.2f\n", topo->loc.nom, topo->loc.lat, topo->loc.lon);
    }
}

void escreve_rotas(ROTA_BLOC *topor, FILE *fp)       /* A função permite escrever todas as rotas e as suas paragens */
{
    PARAG_BLOC *topo;
    for (; topor != NULL; topor = topor->prox)
    {
        fprintf(fp, "#ROTA %d\n", topor->rot.num);
        topo = topor->rot.topo;
        for (; topo != NULL; topo = topo->prox)
        {
            fprintf(fp, "%s %.2f km\n", topo->par.nom, topo->par.dist);
        }
        fprintf(fp, "\n");
    }
}

float distancia(float lon, float lat)              /* Cálculo da distância com base nas variações de latitude e longitude */
{
    float x, y, r = 6371.0, pi = 3.14159;
    lon = lon * 2 * pi / 360;
    lat = lat * 2 * pi / 360;
    x = r * tan(lon);
    y = r * tan(lat);
    return (sqrt(x * x + y * y));
}

void abrir_ficheiro(FILE **file, char *nome)       /* Abre ficheiro indicado e verifica se a abertura é bem sucedida */
{
    *file=fopen(nome,"r");
    if (*file == NULL){
        printf("Ficheiro %s nao encontrado",nome);
        exit(0);
    }
}

float num_local(LOCAL_BLOC *topo)      /* Determina o número de localidades */
{
    float nblocos;
    for (nblocos=0; topo != NULL; topo = topo->prox, nblocos++){}
    return nblocos;
}

void formata(char* nome)               /* Para mais facil comparação das strings todas são formatadas com esta função que coloca a primeira letra maiúscula e as seguintes minúsculas */
{
    int contador;
    for(contador=0;nome[contador]!='\0';contador++){
        if (contador==0){
            if (nome[contador]>='a' && nome[contador]<='z'){
                nome[contador]= nome[contador]-'a'+'A';
            }
        }
        else{
            if (nome[contador]>='A' && nome[contador]<='Z'){
                nome[contador]= nome[contador]-'A'+'a';
            }
        }
        if (nome[contador]=='_'){               /* Depois de um underscore mete a próxima letra maiúscula */
            contador++;
            if (nome[contador]>='a' && nome[contador]<='z'){
                nome[contador]= nome[contador]-'a'+'A';
            }
        }
    }
}

int ja_existe(LOCAL_BLOC *topo, char *nom)         /* Verifica se uma localidade já existe na pilha de localidades para não ser inserida duas vezes */
{
    LOCAL_BLOC *aux;

    aux=topo;
    while (aux != NULL)
    {
        if (strcmp(nom, aux->loc.nom) == 0)
        {
            return 0;
        }
        aux = aux->prox;
    }
    return 1;
}

void teste_parametros(float lat,float lon,int contador){    /* Testa se os valores de latitude e longitude estão dentro dos parâmetros definidos */
    if ((lat <= -90) || (lat >= 90))
        {
            if (contador==-1){
            fprintf(stderr,"Linha de comando: Erro latitude fora dos limites");
            exit(0);
            }
            fprintf(stderr,"Linha %d: Erro latitude fora dos limites", contador);
            exit(0);
        }

    if (lon <= -180 || lon >= 180)
        {
            if (contador==-1){
            fprintf(stderr,"Linha de comando: Erro longitude fora dos limites");
            exit(0);
            }
            fprintf(stderr,"Linha %d: Erro longitude fora dos limites", contador);
            exit(0);
        }
}
