#include "argumentos.h"
#include <stdio.h>

void iniciarArgumentos(TipoArgumentos *argumentos)
{
    argumentos->localidades_output_txt = NULL;
    argumentos->comportamento = Comportamento_indefinido;
    argumentos->localidade_adi_nome = NULL;
    argumentos->localidade_adi_latitude = 0;
    argumentos->localidade_adi_longitude = 0;
    argumentos->localidade_rem_nome = NULL;
    argumentos->rotas_nome = -1;
    argumentos->rotas_output_txt = NULL;
}

int verificaComportamentoUnico(TipoArgumentos *argumentos)
{
    if (argumentos->comportamento != Comportamento_indefinido) {
        fprintf(stderr, "Foi definido mais do que um comportamento\n");
        return 1;
    }
    return 0;
}

int processaArgumentos(TipoArgumentos *argumentos, int argc, char *argv[])
{
    int contador;

    for (contador = 1; contador < argc; contador++)
    {
        if (strncmp(argv[contador], "-LO", 4) == 0)
        {
            if (processaOutput(argumentos, argc, argv, &contador))
                return 1;
        }
        else if (strncmp(argv[contador], "-TL", 5) == 0)
        {
            if (verificaComportamentoUnico(argumentos)) {
                return 1;
            }
            processaLeitura(argumentos);
        }
        else if (strncmp(argv[contador], "-ADI", 5) == 0)
        {
            if (verificaComportamentoUnico(argumentos) || processaAdiciona(argumentos, argc, argv, &contador))
                return 1;
        }
        else if (strncmp(argv[contador], "-REM", 5) == 0)
        {
            if (verificaComportamentoUnico(argumentos) || processaRemove(argumentos, argc, argv, &contador))
                return 1;
        }
        else if (strncmp(argv[contador], "-LR", 4) == 0)
        {
            if (processaOutputRota(argumentos, argc, argv, &contador))
                return 1;
        }
        else if (strncmp(argv[contador], "-ROTAS", 7) == 0)
        {
            if (verificaComportamentoUnico(argumentos) || processaRota(argumentos, argc, argv, &contador))
                return 1;
        }
        else
        {
            fprintf(stderr, "Nao sabemos como processar: %s\n", argv[contador]);
            return 1;
        }
    }

    return 0;
}

int processaOutput(TipoArgumentos *argumentos, int argc, char *argv[], int *contador)
{
    (*contador)++;

    if (*contador >= argc)
    {
        fprintf(stderr, "O ficheiro de output esta em falta a seguir a -LO\n");
        return 1;
    }

    argumentos->localidades_output_txt = argv[*contador];
    return 0;
}

int processaOutputRota(TipoArgumentos *argumentos, int argc, char *argv[], int *contador)
{
    (*contador)++;
    if (*contador >= argc)
    {
        fprintf(stderr, "O ficheiro de output esta em falta a seguir a -LR\n");
        return 1;
    }

    argumentos->rotas_output_txt = argv[*contador];
    return 0;
}

int processaAdiciona(TipoArgumentos *argumentos, int argc, char *argv[], int *contador)
{
    int err;

    argumentos->comportamento = Comportamento_adiciona;

    (*contador)++;
    if (*contador >= argc)
    {
        fprintf(stderr, "Nome de localidade em falta a seguir a -ADI\n");
        return 1;
    }
    argumentos->localidade_adi_nome = argv[*contador];
    (*contador)++;
    if (*contador >= argc)
    {
        fprintf(stderr, "Latitude de localidade em falta a seguir a -ADI\n");
        return 1;
    }

    err = sscanf(argv[*contador], "%f", &argumentos->localidade_adi_latitude);
    if (err != 1)
    {
        fprintf(stderr, "Não foi possivel ler latitude depois de nome. Recebido: %s -ADI\n", argv[*contador]);
        return 1;
    }

    (*contador)++;
    if (*contador >= argc)
    {
        fprintf(stderr, "Longitude de localidade em falta a seguir a -ADI\n");
        return 1;
    }

    err = sscanf(argv[*contador], "%f", &argumentos->localidade_adi_longitude);
    if (err != 1)
    {
        fprintf(stderr, "Não foi possivel ler longitude depois de latitude. Recebido: %s -ADI\n", argv[*contador]);
        return 1;
    }

    return 0;
}

int processaRemove(TipoArgumentos *argumentos, int argc, char *argv[], int *contador)
{
    argumentos->comportamento = Comportamento_remocao;
    (*contador)++;
    if (*contador >= argc)
    {
        fprintf(stderr, "Nome da localidade esta em falta a seguir a -REM\n");
        return 1;
    }
    argumentos->localidade_rem_nome = argv[*contador];

    return 0;
}

int processaRota(TipoArgumentos *argumentos, int argc, char *argv[], int *contador)
{
    int nome, nc;
    argumentos->comportamento = Comportamento_rota;
    (*contador)++;
    if (*contador < argc)
    {
        nc = sscanf(argv[*contador], "%d", &nome);
        if (nc==1){
            argumentos->rotas_nome = nome;

        }
        else{
            (*contador)--;
        }
        return 0;
    }
    return 0;
}

int processaLeitura(TipoArgumentos *argumentos)
{
    argumentos->comportamento = Comportamento_leitura;
    return 0;
}

