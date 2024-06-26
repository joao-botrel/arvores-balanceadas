#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h> 
#include "arvores.h"

//FUNÇÕES DA ÁRVORE AVL

int rotacoesAVL = 0;
int rotacoesRB = 0;

typedef struct noavl{
    int chave;
    int fb;
    struct noavl *Fesq;
    struct noavl *Fdir;
    struct noavl *pai;

} noavl;

typedef struct avl{
    struct noavl *sentinela;
    int numElementos;
} avl;


avl *criaArvoreAvl(){
    avl *novaArvore = (avl *)malloc(sizeof(avl));
    if (novaArvore == NULL) exit(1);
    novaArvore->sentinela = (noavl *)malloc(sizeof(noavl));
    if (novaArvore->sentinela == NULL) exit(1);    
    novaArvore->sentinela->chave = -1000;
    novaArvore->sentinela->Fesq = NULL;
    novaArvore->sentinela->Fdir = NULL;
    novaArvore->sentinela->pai = NULL;
    novaArvore->sentinela->fb = 0;
    novaArvore->numElementos = 0;
    return novaArvore;
}

int insereNoAvl(avl *arv, int valor){
    if (arv == NULL) return 0;
    noavl *novoNo = (noavl *)malloc(sizeof(noavl));
    novoNo->chave = valor;
    novoNo->fb = 0;
    novoNo->Fdir = NULL;
    novoNo->Fesq = NULL;
    novoNo->pai = NULL;

    if (arv->sentinela->Fdir == NULL){
        arv->sentinela->Fdir = novoNo;
        novoNo->pai = arv->sentinela;
        arv->numElementos++;
        return 0;
    }else{
        noavl *pai = NULL;
        noavl *atual = arv->sentinela->Fdir;
        while (atual != NULL){
            pai = atual;
            if (valor < atual->chave) atual = atual->Fesq;
            else atual = atual->Fdir;
        }
        if (pai == NULL){
            arv->sentinela->Fdir = novoNo;
            novoNo->pai = arv->sentinela;
        } else if (valor < pai->chave){
            pai->Fesq = novoNo;
            novoNo->pai = pai;
        } else {
            pai->Fdir = novoNo;
            novoNo->pai = pai;
        }
        arv->numElementos++;
        atualizaFB_insercao(arv, novoNo);
        return 1;
    }
}

int removeNoAvl(avl *arv, int valor){
    noavl *pai = NULL;
    noavl *atual = arv->sentinela;
    if (atual == NULL) return 0;
    while (atual != NULL && atual->chave != valor){
        pai = atual;
        if (valor < atual->chave) atual = atual->Fesq;
        else atual = atual->Fdir;
    }

    if (atual == NULL) return -1;

    if (atual->Fesq == NULL && atual->Fdir == NULL){
        if (pai == NULL) arv->sentinela = NULL;
        else if (atual == pai->Fesq) pai->Fesq = NULL;
        else pai->Fdir = NULL;
        atualizaFB_remocao(arv, pai, atual->chave);
        free(atual);
    }

    else if (atual->Fesq == NULL || atual->Fdir == NULL){
        noavl *filho;
        if (atual->Fesq == NULL){
            filho = atual->Fdir;
            filho->pai = pai;
        }else{
            filho = atual->Fesq;
            filho->pai = pai;
        }
        if (pai == NULL) arv->sentinela = filho;
        else if (atual == pai->Fesq) pai->Fesq = filho;
        else pai->Fdir = filho;
        atualizaFB_remocao(arv, pai, atual->chave);
        free(atual);
    } else {
        noavl *sucessor = atual->Fdir;
        pai = atual;
        while (sucessor->Fesq != NULL){
            pai = sucessor;
            sucessor = sucessor->Fesq;
        }
        atual->chave = sucessor->chave;
        if (pai->Fesq == sucessor){
            pai->Fesq = sucessor->Fdir;
            if (sucessor->Fdir != NULL) sucessor->Fdir->pai = atual;
        }else{
            pai->Fdir = sucessor->Fdir;
            if (sucessor->Fdir != NULL) sucessor->Fdir->pai = atual;

        }

        atualizaFB_remocao(arv, sucessor->pai, sucessor->chave);
        free(sucessor);
        sucessor = NULL;
    }
    arv->numElementos--;
    return 1;
}

void imprimeOrdemAvl(noavl *raiz){
    if (raiz == NULL) return;
    if (raiz != NULL){
        printf("%d - %d - %d\n", raiz->chave, raiz->pai->chave, raiz->fb);
        imprimeOrdemAvl(raiz->Fesq);
        imprimeOrdemAvl(raiz->Fdir);
    }
}

void imprimePreOrdemAvl(noavl *raiz){
    if (raiz == NULL) return;
    if (raiz->pai == NULL){
        imprimePreOrdemAvl(raiz->Fesq);
        imprimePreOrdemAvl(raiz->Fdir);
    }
    else if (raiz->pai != NULL){
        printf("%d - %d - %d\n", raiz->chave, raiz->pai->chave, raiz->fb);
        imprimePreOrdemAvl(raiz->Fesq);
        imprimePreOrdemAvl(raiz->Fdir);
    }
}

noavl *getRaizAvl(avl *arv){
    return arv->sentinela;
}

int getNumElementosAvl(avl *arv){
    return arv->numElementos;
}

void atualizaFB_insercao(avl *arv, noavl *novoNo){
    noavl *aux;
    aux = novoNo;
    do{
        if (aux->chave < aux->pai->chave) aux->pai->fb--;
        else aux->pai->fb++;
        aux = aux->pai;
    } while ((aux->fb != 0) && (aux->fb != 2) && (aux->fb != -2) && (aux->pai != arv->sentinela));
    if ((aux->fb == 2) || (aux->fb == -2)) balanceamento(arv, aux);

}

void balanceamento(avl *arv, noavl *noDesbalanceado){
    noavl *filho;
    if (noDesbalanceado->fb == -2){
        filho = noDesbalanceado->Fesq;
        if (filho->fb == 1){
            int fbNeto = filho->Fdir->fb;
            rotacaoEsqAvl(filho);
            rotacaoDirAvl(noDesbalanceado);
            if (fbNeto == 0){
                noDesbalanceado->fb = 0;
                filho->fb = 0;
            }
            else if (fbNeto == -1){
                noDesbalanceado->fb = 1;
                filho->fb = 0;
                noDesbalanceado->pai->fb = 0;
            }else{
                noDesbalanceado->fb = 0;
                filho->fb = -1;
                noDesbalanceado->pai->fb = 0;
            }
        }else{
            int fbFilho = filho->fb;
            rotacaoDirAvl(noDesbalanceado);
            if (fbFilho != 0){
                filho->fb = 0;
                noDesbalanceado->fb = 0;
            }else{
                filho->fb = 1;
                noDesbalanceado->fb = -1;
            }
        }
    }

    if (noDesbalanceado->fb == 2){
        filho = noDesbalanceado->Fdir;
        if (filho->fb == -1){
            int fbNeto = filho->Fesq->fb;
            rotacaoDirAvl(filho);
            rotacaoEsqAvl(noDesbalanceado);
            if (fbNeto == 0){
                noDesbalanceado->fb = 0;
                filho->fb = 0;
            }
            else if (fbNeto == -1){
                noDesbalanceado->fb = 0;
                filho->fb = 1;
                noDesbalanceado->pai->fb = 0;
            }else{
                noDesbalanceado->fb = -1;
                filho->fb = 0;
                noDesbalanceado->pai->fb = 0;
            }
        }else{
            int fbFilho = filho->fb;
            rotacaoEsqAvl(noDesbalanceado);
            if (fbFilho != 0){
                filho->fb = 0;
                noDesbalanceado->fb = 0;
            }else{
                filho->fb = 0;
                noDesbalanceado->fb = 0;
            }
        }
    }
}

void rotacaoDirAvl(noavl *noDesbal){
    rotacoesAVL++;
    noavl *x = noDesbal->Fesq;
    noDesbal->Fesq = x->Fdir;
    if (x->Fdir != NULL){
        x->Fdir->pai = noDesbal;
    }
    x->pai = noDesbal->pai;
    if (noDesbal == noDesbal->pai->Fdir){
        noDesbal->pai->Fdir = x;
    }else{
        noDesbal->pai->Fesq = x;
    }
    x->Fdir = noDesbal;
    noDesbal->pai = x;
}

void rotacaoEsqAvl(noavl *noDesbal){
    rotacoesAVL++;
    noavl *y = noDesbal->Fdir;
    noDesbal->Fdir = y->Fesq;
    if (y->Fesq != NULL){
        y->Fesq->pai = noDesbal;
    }
    y->pai = noDesbal->pai;
    if (noDesbal == noDesbal->pai->Fesq){
        noDesbal->pai->Fesq = y;
    }else{
        noDesbal->pai->Fdir = y;
    }
    y->Fesq = noDesbal;
    noDesbal->pai = y;
}

void atualizaFB_remocao(avl *arv, noavl *pai, int chaveRemovida){
    if (pai == arv->sentinela) return;
    noavl *aux = pai;
    if (chaveRemovida < aux->chave) aux->fb++;
    else aux->fb--;
    while (aux != arv->sentinela->Fdir && aux->fb == 0){
        aux = aux->pai;
        if (chaveRemovida < aux->chave) aux->fb++;
        else aux->fb--;
    }

    if (aux->fb == 2 || aux->fb == -2){
        balanceamento(arv, aux);
        if ((aux->pai != arv->sentinela) && (aux->pai->fb == 0)){
            atualizaFB_remocao(arv, aux->pai->pai, chaveRemovida);
        }
    }
}

//FUNÇÕES DA ÁRVORE RUBRO NEGRA

struct norb {
    int chave;
    char cor;
    struct norb *esq;
    struct norb *dir;
    struct norb *pai;
};

struct rb {
    struct norb *raiz;
    int numElementos;
};

rb *criaArvoreRB() {
    rb *arv = (rb *) malloc(sizeof(rb));
    if (arv != NULL) {
        arv->raiz = NULL;
        arv->numElementos = 0;
    }
    return arv;
}

int insereNoRB(rb *arv, int valor) {
    if (arv == NULL)
        return 0;
    
    norb *novo_no = (norb*)malloc(sizeof(norb));
    novo_no->chave = valor;
    novo_no->cor = 'R';
    novo_no->esq = novo_no->dir = novo_no->pai = NULL;
    norb *y = NULL;
    norb *x = arv->raiz;

    while (x != NULL) {
        y = x;
        if (novo_no->chave < x->chave) {
            x = x->esq;
        } else {
            x = x->dir;
        }
    }

    novo_no->pai = y;
    if (y == NULL) {
        arv->raiz = novo_no;
    } else if (novo_no->chave < y->chave) {
        y->esq = novo_no;
    } else {
        y->dir = novo_no;
    }

    arv->numElementos++;
    balanceamentoInsercaoRB(arv, novo_no);

    return 1;
}


int removeNoRB(rb *arv, int valor) {
    if (arv == NULL || arv->raiz == NULL)
        return 0;

    norb *noRemover = arv->raiz;
    while (noRemover != NULL && noRemover->chave != valor) {
        if (valor < noRemover->chave)
            noRemover = noRemover->esq;
        else
            noRemover = noRemover->dir;
    }

    if (noRemover == NULL)
        return -1;

    char cor = noRemover->cor;
    if (noRemover->esq != NULL && noRemover->dir != NULL) {
        norb *sucessor = noRemover->dir;
        while (sucessor->esq != NULL)
            sucessor = sucessor->esq;

        noRemover->chave = sucessor->chave;
        noRemover = sucessor;
    }

    norb *filho = (noRemover->esq != NULL) ? noRemover->esq : noRemover->dir;

    if (filho != NULL) {
        filho->pai = noRemover->pai;
    }

    if (noRemover->pai == NULL) {
        arv->raiz = filho;
    } else {
        if (noRemover == noRemover->pai->esq)
            noRemover->pai->esq = filho;
        else
            noRemover->pai->dir = filho;
    }

    if (noRemover->cor == 'B') {
        if (filho != NULL && filho->cor == 'R') {
            filho->cor = 'B';
        } else {
            balanceamentoRemocaoRB(arv, filho, noRemover->pai, valor);
        }
    }

    if (cor == 'B'){
        balanceamentoRemocaoRB(arv, filho, noRemover->pai, valor);
    }

    free(noRemover);
    arv->numElementos--;
    return 1;
}


void imprimeOrdemRB(norb *raiz) {
    if (raiz == NULL) return;
    if (raiz != NULL) {
        printf("COLOR: %c | KEY: %d  PARENT: %d\n", raiz->cor, raiz->chave, raiz->pai->chave);
        imprimeOrdemRB(raiz->esq);
        imprimeOrdemRB(raiz->dir);
    }
}

void imprimePreOrdemRB(rb *arv, norb *raiz) {
    if (raiz != NULL) {
        printf("COLOR: %c | KEY: %d  PARENT: %d\n", raiz->cor, raiz->chave, raiz->pai != NULL ? raiz->pai->chave : -1000);
        imprimePreOrdemRB(arv, raiz->esq);
        imprimePreOrdemRB(arv, raiz->dir);
    }
}

norb *getRaizRB(rb *arv) {
    return arv->raiz;
}

int getNumElementosRB(rb *arv) {
    return arv->numElementos;
}

void balanceamentoInsercaoRB(rb *arv, norb *z) {
    while (z->pai != NULL && z->pai->cor == 'R') {
        if (z->pai == z->pai->pai->esq) {
            norb *tio = z->pai->pai->dir;
            if (tio != NULL && tio->cor == 'R') {
                z->pai->cor = 'B';
                tio->cor = 'B';
                z->pai->pai->cor = 'R';
                z = z->pai->pai;
            } else {
                if (z == z->pai->dir) {
                    z = z->pai;
                    rotacaoEsqRB(arv, z);
                }
                z->pai->cor = 'B';
                z->pai->pai->cor = 'R';
                rotacaoDirRB(arv, z->pai->pai);
            }
        } else {
            norb *tio = z->pai->pai->esq;
            if (tio != NULL && tio->cor == 'R') {
                z->pai->cor = 'B';
                tio->cor = 'B';
                z->pai->pai->cor = 'R';
                z = z->pai->pai;
            } else {
                if (z == z->pai->esq) {
                    z = z->pai;
                    rotacaoDirRB(arv, z);
                }
                z->pai->cor = 'B';
                z->pai->pai->cor = 'R';
                rotacaoEsqRB(arv, z->pai->pai);
            }
        }
    }
    arv->raiz->cor = 'B';
}

void balanceamentoRemocaoRB(rb *arv, norb *noDesbal, norb *pai, int valor) {
    while (noDesbal != arv->raiz && (noDesbal == NULL || noDesbal->cor == 'B')) {
        if (noDesbal == pai->esq) {
            norb *irmao = pai->dir;

            if (irmao != NULL && irmao->cor == 'R') {
                irmao->cor = 'B';
                pai->cor = 'R';
                rotacaoEsqRB(arv, pai);
                irmao = pai->dir;
            }

            if (irmao == NULL || (irmao->esq == NULL || irmao->esq->cor == 'B') && (irmao->dir == NULL || irmao->dir->cor == 'B')) {
                if (irmao != NULL) irmao->cor = 'R';
                noDesbal = pai;
                pai = noDesbal->pai;
            } else {
                if (irmao->dir == NULL || irmao->dir->cor == 'B') {
                    if (irmao->esq != NULL) irmao->esq->cor = 'B';
                    irmao->cor = 'R';
                    rotacaoDirRB(arv, irmao);
                    irmao = pai->dir;
                }

                irmao->cor = pai->cor;
                pai->cor = 'B';
                if (irmao->dir != NULL) irmao->dir->cor = 'B';
                rotacaoEsqRB(arv, pai);
                noDesbal = arv->raiz;
            }
        } else {
            norb *irmao = pai->esq;

            if (irmao != NULL && irmao->cor == 'R') {
                irmao->cor = 'B';
                pai->cor = 'R';
                rotacaoDirRB(arv, pai);
                irmao = pai->esq;
            }

            if (irmao == NULL || (irmao->dir == NULL || irmao->dir->cor == 'B') && (irmao->esq == NULL || irmao->esq->cor == 'B')) {
                if (irmao != NULL) irmao->cor = 'R';
                noDesbal = pai;
                pai = noDesbal->pai;
            } else {
                if (irmao->esq == NULL || irmao->esq->cor == 'B') {
                    if (irmao->dir != NULL) irmao->dir->cor = 'B';
                    irmao->cor = 'R';
                    rotacaoEsqRB(arv, irmao);
                    irmao = pai->esq;
                }

                irmao->cor = pai->cor;
                pai->cor = 'B';
                if (irmao->esq != NULL) irmao->esq->cor = 'B';
                rotacaoDirRB(arv, pai);
                noDesbal = arv->raiz;
            }
        }
    }

    if (noDesbal != NULL)
        noDesbal->cor = 'B';
}


void rotacaoDirRB(rb *arv, norb *noDesbal) {
    rotacoesRB++;
    printf("Oiii");
    norb *x = noDesbal->esq;
    noDesbal->esq = x->dir;

    if (x->dir != NULL) {
        x->dir->pai = noDesbal;
    }

    x->pai = noDesbal->pai;

    if (noDesbal->pai == NULL) {
        arv->raiz = x;
    } else if (noDesbal == noDesbal->pai->esq) {
        noDesbal->pai->esq = x;
    } else {
        noDesbal->pai->dir = x;
    }

    x->dir = noDesbal;
    noDesbal->pai = x;
}

void rotacaoEsqRB(rb *arv, norb *noDesbal) {
    rotacoesRB++;
    norb *y = noDesbal->dir;
    noDesbal->dir = y->esq;

    if (y->esq != NULL) {
        y->esq->pai = noDesbal;
    }

    y->pai = noDesbal->pai;

    if (noDesbal->pai == NULL) {
        arv->raiz = y;
    } else if (noDesbal == noDesbal->pai->esq) {
        noDesbal->pai->esq = y;
    } else {
        noDesbal->pai->dir = y;
    }

    y->esq = noDesbal;
    noDesbal->pai = y;
}

noavl* buscaAVL(noavl* raiz, int chave) {
    if (raiz == NULL || raiz->chave == chave)
        return raiz;

    if (chave < raiz->chave)
        return buscaAVL(raiz->Fesq, chave);
    else
        return buscaAVL(raiz->Fdir, chave);
}

norb* buscaRB(norb* raiz, int chave) {
    norb* atual = raiz;

    while (atual != NULL && atual->chave != chave) {
        if (chave < atual->chave)
            atual = atual->esq;
        else
            atual = atual->dir;
    }

    return atual;
}


void benchmark_avl(int num_elements) {
    FILE *arq;
    arq = fopen("resultadosAVL.txt", "a");
    avl *arvore_avl = criaArvoreAvl();

    double start_time, end_time, total_time;

    // Inserção na Árvore avl
    clock_t start = clock();
    for (int i = 0; i < num_elements; i++) {
        insereNoAvl(arvore_avl, i);
    }
    clock_t end = clock();
    double tempo = ((double)(end - start)) * 1000 / CLOCKS_PER_SEC;
    printf("Imprimendo em ordem\n");
    imprimePreOrdemAvl(getRaizAvl(arvore_avl));
    printf("\n Num elementos: %d\n",getNumElementosAvl(arvore_avl));
    fprintf(arq, "Inserção de 10000 elementos ordenados: %d rotações em %f milissegundos\n", rotacoesAVL, tempo);


    // Busca na Árvore AVL
    start = clock();
    for (int i = 0; i < 1000; i++) {
        int num=rand() % num_elements;
        buscaAVL(arvore_avl->sentinela, num);
    }
    end = clock();		

    tempo = ((double)(end - start)) * 1000 / CLOCKS_PER_SEC;

    fprintf(arq, "Busca de 1000 elementos aleatórios: %f milissegundos\n", tempo);


    rotacoesAVL = 0;

    // Remoção na Árvore Avl
    start = clock();
    for (int i = 0; i < num_elements; i++) {
        removeNoAvl(arvore_avl, i);
    }
    end = clock();		

    tempo = ((double)(end - start)) * 1000 / CLOCKS_PER_SEC;
    printf("\n------------------------------\n");

    printf("Imprimendo em ordem\n");
    imprimePreOrdemAvl(getRaizAvl(arvore_avl));
    printf("\n Num elementos: %d\n",getNumElementosAvl(arvore_avl));
    fprintf(arq, "Remoção de 10000 elementos ordenados: %d rotações em %f milissegundos\n", rotacoesAVL, tempo);



    fclose(arq);
    free(arvore_avl);
}

void benchmark_rb(int num_elements) {
    FILE *arq;
    arq = fopen("resultadosRB.txt", "a");
    rb *arvore_rb = criaArvoreRB();

    double start_time, end_time, total_time;

    // Inserção na Árvore RB
    clock_t start = clock();
    for (int i = 0; i < num_elements; i++) {
        insereNoRB(arvore_rb, i);
    }
    clock_t end = clock();
    double tempo = ((double)(end - start)) * 1000 / CLOCKS_PER_SEC;
    printf("Imprimendo em ordem\n");
    imprimePreOrdemRB(arvore_rb,getRaizRB(arvore_rb));
    printf("Num de elementos: %d\n",getNumElementosRB(arvore_rb));
    fprintf(arq, "Inserção de 10000 elementos ordenados: %d rotações em %f milissegundos\n", rotacoesRB, tempo);

    // Busca na Árvore RB
    start = clock();
    for (int i = 0; i < 1000; i++) {
        int num=rand() % num_elements;
        buscaRB(arvore_rb->raiz, num);
    }
    end = clock();		

    tempo = ((double)(end - start)) * 1000 / CLOCKS_PER_SEC;

    fprintf(arq, "Busca de 1000 elementos aleatórios: %f milissegundos\n", tempo);

    rotacoesRB = 0;

    // Remoção na Árvore RB
    start = clock();
    for (int i = 0; i < num_elements; i++) {
        removeNoRB(arvore_rb, i);
    }
    end = clock();		

    tempo = ((double)(end - start)) * 1000 / CLOCKS_PER_SEC;

    printf("\n------------------------------\n");

    printf("Imprimendo em ordem\n");
    imprimePreOrdemRB(arvore_rb,getRaizRB(arvore_rb));
    printf("Num de elementos: %d\n",getNumElementosRB(arvore_rb));
    fprintf(arq, "Remoção de 10000 elementos ordenados: %d rotações em %f milissegundos\n", rotacoesRB, tempo);




    fclose(arq);
    free(arvore_rb);
}