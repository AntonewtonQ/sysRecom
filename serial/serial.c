#include <stdio.h>
#include <stdlib.h>


#define ALEATORIO ((double)rand() / (double)RAND_MAX)

void preenche_aleatorio_LR(double **L, double **R, int nU, int nI, int nF)
{
    srand(0);
    int i, j;
    for (i = 0; i < nU; i++)
    {
        for (j = 0; j < nF; j++)
        {
            L[i][j] = ALEATORIO / (double)nF;
        }
    }

    for (i = 0; i < nF; i++)
    {
        for (j = 0; j < nI; j++)
        {
            R[i][j] = ALEATORIO / (double)nF;
        }
    }
}

void multiplicacao(double **B, double **L, double **R, int nUL, int nIR, int nIL)
{
    for (int i = 0; i < nUL; i++)
    {
        for (int j = 0; j < nIR; j++)
        {
            B[i][j] = 0;
            for (int k = 0; k < nIL; k++)
            {
                
                B[i][j] += L[i][k] * R[k][j];
            }
        }
    }
}

void calcularL(double **L, double **A, double **B, double **R, double alfa, int nU, int nI, int nF)
{
    double soma = 0.0;

    for (int i = 0; i < nU; i++)
    {
        for (int k = 0; k < nF; k++)
        {
        	soma = 0.0;
            for (int j = 0; j < nI; j++)
            {
                if(A[i][j] == 0)continue;
                soma+=(2 * (A[i][j] - B[i][j]) * (-R[k][j]));
            }
            L[i][k] = L[i][k] - (alfa * soma);
        }
    }
}

void calcularR(double **R, double **A, double **B, double **L, double alfa, int nU, int nI, int nF)
{
    double soma = 0.0;

    for (int k = 0; k < nF; k++)
    {
        for (int j = 0; j < nI; j++)
        {
        	soma = 0.0;
            for (int i = 0; i < nU; i++)
            {
                if(A[i][j] ==0)continue;
                soma+=2 * (A[i][j] - B[i][j]) * (-L[i][k]);
            }
            R[k][j] = R[k][j] - (alfa * soma);
        }
    }
}

void inicializar(double **A, double **B,  double **L, double **R, int nU, int nI, int nF)
{
    for (int i = 0; i < nU; i++)
    {
        for (int j = 0; j < nI; j++)
        {
            A[i][j] = 0.0;
        }
    }

    for (int i = 0; i < nU; i++)
    {
        for (int j = 0; j < nF; j++)
        {
            L[i][j] = 0.0;
        }
    }

    for (int i = 0; i < nF; i++)
    {
        for (int j = 0; j < nI; j++)
        {
            R[i][j] = 0.0;
        }
    }
}

int main(int argc, char *argv[])
{
    FILE *ficheiro;
    int numero_iteracoes, nU, nI, nF, dZ, linha = 0, coluna = 0;
    double alfa, valor = 0;

    ficheiro = fopen(argv[1], "r");
    
    if (ficheiro == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    fscanf(ficheiro, "%d", &numero_iteracoes);
    fscanf(ficheiro, "%lf", &alfa);
    fscanf(ficheiro, "%d", &nF);
    fscanf(ficheiro, "%d %d %d", &nU, &nI, &dZ);

    double **A = (double **)malloc(nU * sizeof(double *));
    for (int i = 0; i < nU; i++)
    {
        A[i] = (double *)malloc(nI * sizeof(double));
    }

    double **L = (double **)malloc(nU * sizeof(double *));
    double **L_auxiliar = (double **)malloc(nU * sizeof(double *));
    
    for (int i = 0; i < nU; i++)
    {
        L[i] = (double *)malloc(nF * sizeof(double));
        L_auxiliar[i] = (double *)malloc(nF * sizeof(double));
    }
    double **R = (double **)malloc(nF * sizeof(double *));
    for (int i = 0; i < nF; i++)
    {
        R[i] = (double *)malloc(nI * sizeof(double));
    }
    double **B = (double **)malloc(nU * sizeof(double *));
    for (int i = 0; i < nU; i++)
    {
        B[i] = (double *)malloc(nI * sizeof(double));
    }

    inicializar(A, B, L, R, nU, nI, nF);

    preenche_aleatorio_LR(L, R, nU, nI, nF);
    
    for (int i = 0; i < dZ; i++)
    {
        fscanf(ficheiro, "%d %d %lf", &linha, &coluna, &valor);
        A[linha][coluna] = valor;
    }

    multiplicacao(B, L, R, nU, nI, nF);

    for (int iteracao = 1; iteracao <= numero_iteracoes; iteracao++)
    {
        for(int i = 0; i  < nU; i++){
            for(int j = 0; j < nF; j++){
                L_auxiliar[i][j] = L[i][j];
            }
        }
        calcularL(L, A, B, R, alfa, nU, nI, nF);
        calcularR(R, A, B, L_auxiliar, alfa, nU, nI, nF);
        multiplicacao(B, L, R, nU, nI, nF);
    }
    
    fclose(ficheiro);
    
    FILE *saida = fopen("saida.txt", "w");
    if (saida == NULL)
    {
        printf("Erro ao abrir o arquivo de saída.\n");
        exit(1);
    }
    double maior = 0.0;
    int indice = 0;

    for (int i = 0; i < nU; i++)
    {
        maior = 0.0;
        for (int j = 0; j < nI; j++)
        {
            if (A[i][j] == 0)
            {
                indice = (B[i][j] > maior) ? j : indice;
                maior = (B[i][j] > maior) ? B[i][j] : maior;
            }
        }
        fprintf(saida, "%d\n", indice);
    }

    return (0);
}