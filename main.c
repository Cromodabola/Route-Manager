#include <stdio.h>
#include "argumentos.h"
#include "dados.h"

/*----------------------------
------------------------------
---------- Grupo 62 ----------
------------------------------
--- Adriano Cardoso 102933 ---
-- Francisco Almeida 102865 --
------------------------------
----------------------------*/
/*Este programa permite calcular as dist�ncias entre as localidades do ficheiro localidades.txt. Estas s�o ordenadas
por ordem alfab�tica num ficheiro escolhido pelo utilizador atrav�s dos comando -LO. � tamb�m poss�vel fazer simples
opera��es como adicionar e remover uma localidade de localidades.txt reescrevendo as localidades por ordem alfab�tica
no ficheiro introduzido pela flag -LO. Por fim, � poss�vel calcular as dist�ncias das rotas (todas ou apenas uma
indicada a seguir do comando -ROTAS) do ficheiro rotas.txt, ordenando-as por ordem crescente das dist�ncias totais
de cada rota, no ficheiro introduzido pela flag -LR.*/

int main(int argc, char *argv[])
{
    TipoArgumentos argumentos;
    LOCAL_BLOC *inf, aux, *topolocal;
    ROTA_BLOC *toporota = NULL;
    FILE *localidades_input, *localidades_output,*rotas_input, *rotas_output;
    topolocal = NULL;

    iniciarArgumentos(&argumentos);                                                 /*Inicia os argumentos e analiza o comportamento a seguir*/
    if (processaArgumentos(&argumentos, argc, argv)){   return 1;}

    abrir_ficheiro(&localidades_input, "localidades.txt");
    localidades_output = fopen(argumentos.localidades_output_txt, "w");
    topolocal = leitura_de_localidades(localidades_input, topolocal);               /*Cria a pilha das localidades*/

    if (argumentos.comportamento == Comportamento_indefinido){
        fprintf(stderr, "Nao foi definido um comportamento\n");
        return 1;
    }
    if (argumentos.localidades_output_txt==NULL){
        fprintf(stderr, "Nao foi definido o comando -LO \n");
        return 1;
    }
    if (argumentos.comportamento == Comportamento_leitura){}                        /*Apesar de aspetualmente nao fazer nada, � condi��o*/
    else if (argumentos.comportamento == Comportamento_adiciona){                   /*necess�ria para executar exclusivamente a leitura*/
        formata(argumentos.localidade_adi_nome);
        strcpy(aux.loc.nom, argumentos.localidade_adi_nome);                        /*O comportamento_adiciona adiciona � pilha a*/
        aux.loc.lat = argumentos.localidade_adi_latitude;                           /*localidade, caso cumpra os requisitos*/
        aux.loc.lon = argumentos.localidade_adi_longitude;
        inf = &aux;
        teste_parametros(inf->loc.lat,inf->loc.lon, -1);
        topolocal = guarda_dados_na_pilha(topolocal, inf);
    }
    else if (argumentos.comportamento == Comportamento_remocao){                        /*O comportamento_remove remove da pilha a*/
        formata(argumentos.localidade_rem_nome);                                        /*localidade, caso exista*/
        topolocal = remove_dados_na_pilha(topolocal, argumentos.localidade_rem_nome);
    }
    else if (argumentos.comportamento == Comportamento_rota){                               /*O comportamento_rota permite ler e calcular*/
        if (argumentos.rotas_output_txt==NULL) {                                            /*uma ou todas as rotas do ficheiro rotas.txt*/
            fprintf(stderr, "Nao foi definido o comando -LR \n");
            return 1;
        }
        abrir_ficheiro(&rotas_input,"rotas.txt");                                           /*Inicializa os ficheiros de leitura e escrita*/
        rotas_output = fopen(argumentos.rotas_output_txt, "w");
        if (argumentos.rotas_nome==-1)  toporota = leitura_rotas(rotas_input, topolocal);   /*O nome(numero da rota) � inicializado como -1*/
        else    toporota = leitura_rota(rotas_input, argumentos.rotas_nome, topolocal);     /*sendo por isso usado como par�metro decisivo na*/
        escreve_rotas(toporota, rotas_output);                                              /*escolha da rota*/
        fclose(rotas_output);
        fclose(rotas_input);                                                                /*Escreve as rotas no ficheiro de output e fecha os*/
    }                                                                                       /* ficheiros abertos pelo comportamento*/
    escreve_localidades(topolocal, localidades_output);
    fclose(localidades_input);                                                              /*Escreve as localidades no respetivo output e fecha*/
    fclose(localidades_output);
    printf("Programa executado com sucesso.\n");

    return 0;
}
