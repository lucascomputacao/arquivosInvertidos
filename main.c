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
    char nomeArquivo;
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

    /*retornando o indice do vetor onde será armaznada a palavra em questao*/
    return hash;
}

/* Leitura de palavras do arquivo */
void read_words(char arquivo[]) {
    int i = 0, j, hash, countPosicao;
    char caracter, word[MAX];
    countPosicao = 0; // contador de posicao da palavra no texto

    //   printf("caracter = %c\n", arquivo[5]);
    printf("conteudo do arquivo: %s", arquivo);
    while (arquivo[i] != EOF) {
        printf("dentro do while");
        printf("%c", arquivo[i]);
        i++;
    }
    /*
        // loop to read characters
        while (caracter != EOF || caracter != '\0') {

            //printf("conteudo do arquivo: %s\n", arquivo);

            for (i = 0; caracter != '\0' || caracter != EOF ; i++) {
                printf("entrou no for\n");
                caracter = arquivo[i];
                if ((caracter >= 65) && (caracter <= 122)) {
                    word[j] = caracter;
                    j++;
                }
            }

            // ao menos uma letra...
            if (j) {
                word[j] = '\0'; // encerra string
                countPosicao++; // incrementa a posição de palavra
                printf("palavra lida = %s\n posicao = %d\n", word, countPosicao);
                hash = hashing(word);
                printf("hashing = %d\n", hash);
                //insere_palavra(hash, word, argv[i], countPosicao);
            }

            printf("caracter = %c\n", caracter);
        }*/
    fclose(arquivo); // fecha arquivo
    caracter = '\0'; // "limpa" o caracter para o while continuar funcionando

}

/**
 *
 * @param hashing - calculado pela funcao hashing
 * @param word - palavra a ser inserida
 * @param arquivo - nome do arquivo onde a palavra aparece
 * @param posicao - posicao no texto onde a palvra aparece
 * @return 1, caso inserido com sucesso e 0 caso contrário
 */
int insere_palavra(int hash, char word[], char nomearquivo[], int posicao) {
    //NoPalavra * busca;
    //if (busca = busca_palavra(word) != NULL) {

    if (1 == 0) {
        // palavra já existe
        //busca_arquivo(nomearquivo);
        //if palavra e arquivo já existirem apenas acrescenta a posicao


    } else {// palavra não existe
        // alocacoes
        NoPalavra* palavra = (NoPalavra*) malloc(sizeof (NoPalavra));
        NoArquivo* arquivo = (NoArquivo*) malloc(sizeof (NoArquivo));
        NoPosicao* posicao = (NoPosicao*) malloc(sizeof (NoPosicao));

        // preenchedo conteudo das estruturas e acertando ponteiros
        strcpy(palavra->palavra, word);
        palavra->listaArquivos = arquivo;
        strcpy(arquivo->nomeArquivo, nomearquivo);
        arquivo->listaPosicoes = posicao;
        posicao->posicao = posicao;

        // inserindo na lista
        if (vet[hash].vet == NULL) {
            vet[hash].vet = palavra;
            palavra->proxPalavra = NULL;
        } else {
            // inere no inicio da lista - menos custoso/nao necessita de ordem
            palavra->proxPalavra = vet[hash].vet;
            vet[hash].vet = palavra;
        }
        return 1;
    }
}

/**
 *
 * @param word - palavra a ser buscada
 * @return ponteiro para palavra buscada ou NULL caso não encontre
 */

/*
Palavra busca_palavra(char word[]) {
    int hash = hashing(word);
    NoPalavra* buscada;

    // Vetor não nulo
    if (vet[hash].vet != NULL) {
        buscada = vet[hash].vet;
    }

    //return ;
}
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
                    //printf("%s\n", buffer);
                }
            }
            read_words(buffer);
            fclose(arquivo); // fecha o arquivo
            free(buffer); // libera a memória do buffer
        }
    }

}

/*
 * FUNCAO PRINCIPAL
 */
int main(int argc, char** argv) {
    //FILE *arquivo;
    //long tamanho;

    if (argc > 1) {
        //name_arqs(argc, argv);
        //read_words(argc, argv);
        read_files(argc, argv);
    }
    //ler uma palavra e passar para a funcao de hasing
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