//FUNÇÕES DA ÁRVORE AVL

typedef struct noavl noavl;
typedef struct avl avl;

//Função que cria e retorna uma estrutura do tipo árvore balanceada utilizando o algoritmo AVL
avl *criaArvoreAvl();

//Função que insere um elemento na árvore
//Retorna 1 se a inserção foi realizada com sucesso
//Retorna 0 se não foi possível realizar a inserção
int insereNoAvl(avl *arv, int valor);

//Função que remove um elemento da árvore implementando a remoção por cópia usando o sucessor
//Retorna 1 se o elemento foi removido
//Retorna 0 se a árvore estiver vazia
//Retorna -1 caso o elemento a ser removido não esteja na árvore
int removeNoAvl(avl *arv, int valor);

//Função que imprime a árvore em ordem
//printf("%d - %d - %d\n",chave, pai, fb)
void imprimeOrdemAvl(noavl *raiz);

//Função que imprime a árvore em pré-ordem
//printf("%d - %d - %d\n", raiz->chave, raiz->pai->chave, raiz->fb);
void imprimePreOrdemAvl(noavl *raiz);

//Função que retorna a raiz da árvore
noavl *getRaizAvl(avl *arv);

//Função que retorna a quantidade de elementos da árvore
int getNumElementosAvl(avl *arv);

//Função que retorna o elemento buscado
noavl* buscaAVL(noavl* raiz, int chave);

/***********FUNÇÕES DE BALANCEAMENTO DA AVL**************/

//Função que verifica o desbalanceamento na inserção
void atualizaFB_insercao(avl *arv, noavl *novoNo);

//Função que faz o balanceamento após inseção
void balanceamento(avl *arv, noavl *noDesbal);

//Função que implementa a rotação à direita
void rotacaoDirAvl(noavl *noDesbal);

//Função que implementa a rotação à esquerda
void rotacaoEsqAvl(noavl *noDesbal);

//Função que verifica o desbalanceamento na remoção
void atualizaFB_remocao(avl *arv, noavl *pai, int chaveRemovida);


//FUNÇÕES DA ÁRVORE RUBRO NEGRA

typedef struct norb norb;
typedef struct rb rb;

//Função que cria e retorna uma estrutura do tipo árvore balanceada utilizando o algoritmo RB
rb *criaArvoreRB();

//Função que insere um elemento na árvore
//Retorna 1 se a inserção foi realizada com sucesso
//Retorna 0 se não foi possível realizar a inserção
int insereNoRB(rb *arv, int valor);

//Função que remove um elemento da árvore implementando a remoção por cópia usando o sucessor
//Retorna 1 se o elemento foi removido
//Retorna 0 se a árvore estiver vazia
//Retorna -1 caso o elemento a ser removido não esteja na árvore
int removeNoRB(rb *arv, int valor);

//Função que imprime a árvore em ordem
void imprimeOrdemRB(norb *raiz);

//Função que imprime a árvore em pré-ordem
void imprimePreOrdemRB(rb *arv, norb *raiz);

//Função que retorna a raiz da árvore
norb *getRaizRB(rb *arv);

//Função que retorna a quantidade de elementos da árvore
int getNumElementosRB(rb *arv);

//Função que retorna o elemento buscado
norb* buscaRB(norb* raiz, int chave);

/***********FUNÇÕES DE BALANCEAMENTO DA RUBRO NEGRA**************/

//Função que verifica o desbalanceamento na inserção
void balanceamentoInsercaoRB(rb *arv, norb *noDesbal);

//Função que verifica o desbalanceamento na remoção
void balanceamentoRemocaoRB(rb *arv, norb *noDesbal, norb *pai, int valor);

//Função que implementa a rotação à direita
void rotacaoDirRB(rb *arv, norb *noDesbal);

//Função que implementa a rotação à esquerda
void rotacaoEsqRB(rb *arv, norb *noDesbal);

// Funções para medir tempo
double get_time();

//benchmark avl
void benchmark_avl(int num_elements);

//benchmark rb
void benchmark_rb(int num_elements);

norb* minimoRB(norb* no);

void transplantarRB(rb *arvore, norb *u, norb *v);