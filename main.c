/* 
 * File:   main.c
 * Author: Lucas Borges Teixeira dos Santos
 *
 * Created on 4 de Novembro de 2014, 19:10
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>

#define NTABHASH  11
#define MAX 100000

// Protótipos de funcoes
void name_arqs(int argc, char** argv);

/*estrutura para vetor de apontadores*/
typedef struct vetor {
    struct NoPalavra *vet; /*ponteiro para estruturas do tipo NoPalavra*/
} vetor;

vetor vet[NTABHASH]; /*definição do vetor de apontadores para as listas*/

/* estrutura para palavra no idioma de origem*/
typedef struct NoPalavra {
    char palavra[30]; /*palavra no idioma de origem*/
    struct NoPalavra *proxPalavra;
    struct NoArquivo *listaArquivos;
} NoPalavra, *Palavra;

/* estrutura para lita de arquivos onde a palavra aparece*/
typedef struct NoArquivo {
    char nomeArquivo[100];
    struct NoArquivo *proxArq;
    struct NoPosicao *listaPosicoes;
} NoArquivo, *Arquivo;

/* estrutura da lista de posicoes da palavra no texto*/
typedef struct NoPosicao {
    int posicao; // posicao em que a palvara foi lida no texto
    struct NoPosicao *proxPos; // proximas posicoes em que a palavra aparece no texto
} NoPosicao, *Posicao;

/**
 * Cria vetor de apontadores 
 */
void inicializaVetor() {
    int i;

    for (i = 0; i < NTABHASH; i++) {
        vet[i].vet = NULL; /*coloca um ponteiro na posicao do vetor*/
    }
}

/**
 * funcao para calcular o indice de armazenamento no vetor de apontadores
 * 
 * @param palavra
 * @return hash [inteiro que será indice do vetor de apontadores]
 */
int hashing(char palavra[]) {
    int i, soma, hash;
    int letra;

    soma = 0;
    /* varre a string para somar o valor dos caracteres*/
    for (i = 0; palavra[i] != '\0'; i++) {
        letra = (palavra[i] - 96); /*usa a tabela ascii para setar valor sequencial, a partir de 1, nas letras do alfabeto*/
        soma = soma + letra; /*soma os valores das letras de uma palavra para calculo do hash*/
    }

    /*calcula o valor de hash para armazenamento no vetor de apontadores*/
    hash = (soma % NTABHASH);

    fprintf(stderr, "hash => %d \n", hash);
    /*retornando o indice do vetor onde será armaznada a palavra em questao*/
    return hash;
}

/* Leitura de palavras do arquivo */
void read_words(char arquivo[], char** argv) {
    int i = 0, j = 0, hash, countPosicao = 0;
    char caracter, word[MAX];

    fprintf(stderr, "conteudo do arquivo: %s\n", argv);
    do {
        //fprintf(stderr, "aqui > %c\n", arquivo[33] );
        if ((arquivo[i] >= 65) && (arquivo[i] <= 122)) {
            word[j] = tolower(arquivo[i]); //Transforma todas as letras em minúsculas
            //fprintf(stderr, "arquivo > %c; word > %c\n", arquivo[i],word[j] );           
            j++;
            i++;
        } else if (j > 0) {
            word[j] = '\0'; // encerra string
            countPosicao++; // incrementa a posição de palavra
            fprintf(stderr, "palavra lida = %s;\n posicao = %d;\n", word, countPosicao);
            hash = hashing(word);
            if (insere_palavra(hash, word, argv, countPosicao)) {
                fprintf(stderr, "Palavra '%s' inserida com sucesso!\n", word);
            }
            if (arquivo[i + 1] != '\0' || arquivo[i + 1] != EOF) {
                j = 0;
                i++;
            }

        }
    } while (arquivo[i] != '\0');
    fprintf(stderr, "fim da função read_words\n");
}

/**
 * 
 * @param hashing - calculado pela funcao hashing
 * @param word - palavra a ser inserida
 * @param arquivo - nome do arquivo onde a palavra aparece
 * @param posicao - posicao no texto onde a palvra aparece
 * @return 1, caso inserido com sucesso e 0 caso contrário
 */
int insere_palavra(int hash, char** word, char** nomearquivo, int posicao) {
    NoPalavra *plvra;
    NoArquivo *arq;
    NoPosicao *pos;

    fprintf(stderr, "nome do arquivo: %s\t palavra: %s \t hash: %d \t posicao: %d\n", nomearquivo, word, hash, posicao);

    // Lista de palavras NAO-VAZIA
    if (vet[hash].vet != NULL) {
        fprintf(stderr, " LISTA DE PALAVRAS NAO VAZIA \n");
        // fazer busca por palavra para verificar existência e
        // só depois inserir ou não a palavra
        for (plvra = vet[hash].vet; plvra != NULL; plvra = plvra->proxPalavra) {

            // Palavra ja existe na lista
            if (strcmp(plvra->palavra, word) == 0) {
                fprintf(stderr, " palavra já existe na lista \n");
                // se ja existe, testa se o arquivo ja existe na lista de arquivos da palavra               
                for (arq = plvra->listaArquivos; arq != NULL; arq = arq->proxArq) {
                    fprintf(stderr, " palavra já existe - dentro do for \n");
                    // se o arquivo ja existe, acrescenta a posição na lista de posições
                    if (strcmp(arq->nomeArquivo, nomearquivo) == 0) {
                        fprintf(stderr, " palavra já existe - dentro do for - if strcmp \n");
                        if (arq->listaPosicoes != NULL) {
                            fprintf(stderr, " palavra já existe na lista - if arq->listaPosicoes != NULL \n");
                            NoPosicao* pos = (NoPosicao*) malloc(sizeof (NoPosicao));
                            // linhas abaixo causando erro
                            pos->proxPos = arq->listaPosicoes;
                            //fprintf(stderr, " aqui tio\n");
                        }
                        arq->listaPosicoes = pos;
                        return 1;
                    }
                }
                // não encontrou arquivo na lista de arquivos
                if (arq == NULL) {
                    fprintf(stderr, " arquivo não existe na lista\n");
                    NoArquivo* arq = (NoArquivo*) malloc(sizeof (NoArquivo));
                    NoPosicao* pos = (NoPosicao*) malloc(sizeof (NoPosicao));
                    // conteudo
                    plvra->listaArquivos = arq;
                    strcpy(arq->nomeArquivo, nomearquivo);
                    arq->listaPosicoes = pos;
                    arq->proxArq = NULL;
                    pos->posicao = posicao;
                    pos->proxPos = NULL;

                    return 1;
                }
            }
        }
        if (plvra == NULL) {
            fprintf(stderr, " palavra não existe na lista \n");
            // alocacoes
            NoPalavra* plvra = (NoPalavra*) malloc(sizeof (NoPalavra));
            NoArquivo* arq = (NoArquivo*) malloc(sizeof (NoArquivo));
            NoPosicao* pos = (NoPosicao*) malloc(sizeof (NoPosicao));

            // preenchedo conteudo das estruturas e acertando ponteiros       

            strcpy(plvra->palavra, word);
            fprintf(stderr, " inserido string '%s' \n", word);
            strcpy(arq->nomeArquivo, nomearquivo);
            arq->listaPosicoes = pos;
            arq->proxArq = NULL;
            plvra->listaArquivos = arq;
            pos->posicao = posicao;
            pos->proxPos = NULL;

            // Inserindo na lista de palavras   
            vet[hash].vet = plvra;
            plvra->proxPalavra = NULL;

            return 1;
        }

    }
    // Lista de palavras VAZIA
    if (vet[hash].vet == NULL) {
        fprintf(stderr, " LISTA DE PALAVRAS VAZIA \n");

        // alocacoes
        NoPalavra* plvra = (NoPalavra*) malloc(sizeof (NoPalavra));
        NoArquivo* arq = (NoArquivo*) malloc(sizeof (NoArquivo));
        NoPosicao* pos = (NoPosicao*) malloc(sizeof (NoPosicao));

        // preenchedo conteudo das estruturas e acertando ponteiros       

        strcpy(plvra->palavra, word);
        plvra->listaArquivos = arq;
        strcpy(arq->nomeArquivo, nomearquivo);
        arq->listaPosicoes = posicao;
        arq->proxArq = NULL;
        pos->posicao = posicao;
        pos->proxPos = NULL;

        // Inserindo na lista de palavras   
        vet[hash].vet = plvra;
        plvra->proxPalavra = NULL;

        // inserida com sucesso
        return 1;
    }
    // caso nao inserida 
    return 0;
}

/**
 * 
 * @param word - palavra a ser buscada
 * @return ponteiro para palavra buscada ou NULL caso não encontre 
 */
Palavra busca_palavra(char** word) {
    int hash = hashing(word);
    NoPalavra* buscada;
    NoPalavra* p;

    // Vetor não nulo
    if (vet[hash].vet != NULL) {
        buscada = vet[hash].vet;
        for (p = buscada; p != NULL; p = buscada->proxPalavra) {
            if (strcmp(buscada->palavra, word) != 0) {
                return buscada;
            }
        }
    }

    // Vetor nulo - insere na primeira posição
    if (vet[hash].vet == NULL) {
        return NULL;
    }
}

/*
 * Lê todos os arquivos e passa as palavras de cada um para a função de read_words()
 */

void read_files(int argc, char** argv) {
    int i;
    FILE *arquivo;

    for (i = 1; i < argc; i++) {
        arquivo = fopen(argv[i], "r");
        if (arquivo != NULL) {
            // vamos obter o tamanho do arquivo em bytes
            fseek(arquivo, 0, SEEK_END);
            long tam_arquivo = ftell(arquivo);
            rewind(arquivo);

            // vamos alocar memória para todo o conteúdo do arquivo
            char *buffer = (char*) malloc(sizeof (char) * tam_arquivo);
            // a memória foi alocada com sucesso?
            if (buffer != NULL) {
                // copiar o conteúdo do arquivo para o buffer
                size_t resultado = fread(buffer, 1, tam_arquivo, arquivo);

                // verificar se a operação foi feita com sucesso
                if (resultado == tam_arquivo) {
                    printf("O conteudo do arquivo foi lido com sucesso\n\n");
                    //fprintf(stderr, "arquivo: %s\n", buffer);
                }
            }
            // passa o arquivo inteiro lido para a função de 
            read_words(buffer, argv[i]);
            fclose(arquivo); // fecha o arquivo
            free(buffer); // libera a memória do buffer
        }
    }

}

void print_lista_palavras() {
    int i;
    NoPalavra* palavra;

    // i?
    palavra = vet[i].vet;
    for (i = 0; i < NTABHASH; i++) {


    }

}

/*
 * FUNCAO PRINCIPAL
 */
int main(int argc, char** argv) {
    //FILE *arquivo;
    //long tamanho;
    int i;

    if (argc > 1) {
        //fprintf(stderr, "nome do arquivo: %s\n", argv[i] );
        //ler uma palavra e passar para a funcao de hasing
        read_files(argc, argv);



    }

    //que retornara o indice a ser inserido 

    return (EXIT_SUCCESS);
}

/**
 * 
 * @param argc - passado direto da função main
 * @param argv - passado direto da função main
 * 
 * Salva(exibe) os nomes dos arquivos passados como parametro
 */
void name_arqs(int argc, char ** argv) {
    int i;

    printf("arquivos passados como parametros:\n");
    for (i = 1; i < argc; i++) {
        printf("%s\n", argv[i]);
        //strcpy(vetArq[i], argv[i]);
    }
}