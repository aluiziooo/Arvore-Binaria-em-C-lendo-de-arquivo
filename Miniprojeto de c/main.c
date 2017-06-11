#include <stdio.h>
#include <stdlib.h>
#ifdef WIN32
#include <windows.h>

void gotoxy(int coluna, int linha){

   COORD point;
   point.X = coluna;
   point.Y = linha;
   SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), point);

}
#else
void gotoxy(int x, int y){

   printf("\033[%d;%df", y, x);
   fflush(stdout);

}
#endif

//***=================================================================
//====================ESTRUTURA DA ARVORE BINARIA=======================
//***==================================================================
typedef struct elementos{
    char nome[100];
}t_elemento;

typedef struct no{
    struct no *esq;
    t_elemento dado;
    struct no *dir;
}t_no;

typedef t_no * t_arvore;
t_no * criar(){
    t_no * no =(t_no*) malloc(sizeof(t_no));
    if(no)
        no->esq = no->dir = NULL;
    return no;
}
int comparar(t_elemento dado1, t_elemento dado2){
    return(strcmp(dado1.nome,dado2.nome));

}//***********************================================================
//========================MECANISMOS DE BUSCA PARA DETERMINAR SE ALGUM NUMERO EXISTE NA ARVORE==================
//**========================================================================================
t_no *buscaSetPai(t_arvore tree, t_elemento dado, t_no **pai){

    if(tree == NULL){
        *pai = NULL;
        return NULL;
    }

    if(comparar(tree->dado, dado) == 0)
        return tree;

    if(comparar(tree->dado, dado) > 0){
        *pai = tree;
        return buscaSetPai(tree->esq, dado, pai);
    }else{
        *pai = tree;
        return buscaSetPai(tree->dir, dado, pai);
    }

}
//***************************=============================================
int remover_elemento(t_arvore *tree, t_elemento dado){//AQUI ELE

    t_no *no,
           *pai,
           *sub,
           *pai_suc,
           *suc;

    no = *tree;
    pai = NULL;

    no = buscaSetPai(*tree, dado, &pai);

    if(no == NULL)
        return 0;

    if(no->esq == NULL){
        sub = no->dir;
    }else{

        if(no->dir == NULL){
            sub = no->esq;
        }else{

            pai_suc = no;
            sub = no->dir;
            suc = sub->esq;

            while(suc != NULL){

                pai_suc = sub;
                sub = suc;
                suc = sub->esq;

            }

            if(pai_suc != no){

                pai_suc->esq = sub->dir;
                sub->dir = no->esq;
            }
            sub->esq = no->esq;
        }

    }

    if (pai == NULL){
       *tree = sub;
    }else{

        if (no == pai->esq)
            pai->esq = sub;
        else
            pai->dir = sub;

    }
    free(no);
    return 1;
}
//*************************===================================================
///////////////INSERINDO NA ARVORE///////////////
int inserir(t_arvore *tree, t_elemento dado){

    int ok;

    if(*tree == NULL){//VERIFICA SE A JÁ FOI INICIALIZADA

        *tree = criar();//SE NÃO, ELE INICIALIZA, CRIA UM NO E SETA OS PONTEIROS PARA NULL

        if(*tree == NULL)//SE DER ERRO DE MEMORIA INSUFICIENTE ELE RETORNA 0
            return 0;

        (*tree)->dado = dado;//SE NÃO DER ELE SETA O ENDEREÇO DO NÓ RAIZ PARA O PONTEIRO TREE
        return 1;

    }

    if(comparar((*tree)->dado, dado) < 0){//SE A ARVORE FOI INICIALIZADO ELE COMPARA O DADO QUE FOI PASSADO COM O DO PRIMIEOR NÓ
        ok = inserir(&((*tree)->dir), dado);//SEFOR MAIOR ELE DESCE PARA A DIREITA, COMO UM BOM EXEMPLO DE ARVORE BINARIA
    }else{

        if(comparar((*tree)->dado, dado) > 0){//SE O VALOR FOR MENOR ELE DESCE PELA DIREITA
            ok = inserir(&((*tree)->esq), dado);
        }else{
            ok = 0;
        }

    }

    return ok;//RETORNA 0 OU 1

}
void esvaziar(t_arvore *tree){//ESVAZIA A ARVORE, UMA COISA QUE NOTEI É QUE ELE USA O PÓS ORDEM PARA IR DE BAIXO NA ESQUERDA
    if(*tree == NULL)        //ATÉ DE BAIXO DO LADO DIREITO
        return;

    esvaziar(&(*tree)->esq);//É RECUSIVO, BEM PRATICO.
    esvaziar(&(*tree)->dir);
    free(*tree);
    *tree = NULL;

}
void telaPrincipal(){
        printf("\n\nALUNO: ALUIZIO BARBOSA MACIEL NETO\n");
        printf("DISCIPLINA: ESTRUTURA DE DADOS I\n");
        printf("PROFESSOR: WALACE BONFIM\n");
        printf("                 \n\nEDITOR DE ARVORE\n\n");
        printf("\n1 – INSERIR\n");
        printf("2 – REMOVER APENAS UM NÓ\n");
        printf("3 – PESQUISAR\n");
        printf("4 – ESVAZIAR A ÁRVORE\n");
        printf("5 – EXIBIR A ÁRVORE\n");
        printf("0 – SAIR\n");
        printf("DIGITE SUA OPÇÃO\n>");
}
//*==========================================================
//============FORMAS DE ANDAR PELA ARVORE===================
//=========================================================*
void mostrarEmPreOrdem(t_arvore tree){

    if(tree != NULL){

        printf("%s ", tree->dado.nome);
        mostrarEmPreOrdem(tree->esq);
        mostrarEmPreOrdem(tree->dir);

    }

}
void mostrarInOrdem(t_arvore tree){
    if(tree != NULL){
        mostrarInOrdem(tree->esq);
        printf("%s ", tree->dado.nome);
        mostrarInOrdem(tree->dir);
    }

}

void mostrarEmPosOrdem(t_arvore tree){

    if(tree != NULL){

        mostrarEmPosOrdem(tree->esq);
        mostrarEmPosOrdem(tree->dir);
        printf("%s ", tree->dado.nome);

    }

}

void mostrarGraficamente(t_arvore tree, int col, int lin, int Desloc){

    if(tree == NULL)
        return;

    gotoxy(col, lin);

    printf("%s", tree->dado.nome);

    if(tree->esq != NULL)
        mostrarGraficamente(tree->esq, col - Desloc, lin + 2, Desloc/ 2 + 1);

    if(tree->dir != NULL)
        mostrarGraficamente(tree->dir, col + Desloc, lin + 2, Desloc / 2 + 1);

}
//*******=========================================================================
//=========================PESQUISA UM ELEMENTO NA ARVORE====================
//==================================================================
t_no *pesquisa(t_arvore tree, t_elemento dado){


    if(tree == NULL)
        return NULL;
    if(comparar(tree->dado,dado)==0)
        return tree;

    if(comparar(tree->dado, dado) > 0)
        return pesquisa(tree->esq, dado);
    else
        return pesquisa(tree->dir, dado);

}

//*=================================================================
//*============================FUNÇÃO PRINCIPAL=====================
//*=================================================================
int main()
{
    t_arvore tree = NULL;
//*=================================================================
//=======================CARREGAR O ARQUIVO NA MEMORIA==============
//**================================================================
    printf("CARREGANDO A ARVORE NO ARQUIVO...\n");
    FILE *arq;
    arq = fopen("arvore.txt","r");
    if(arq == NULL)
        return 0;
    int c;
    char nome[100];//CRIEI ESSA STRING PARA IR ARMAZENDO CHAR POR CHAR NO INDICE
    int y =0;
    t_elemento arquivar;
    //ENQUANTO TIVER LETRA ELE VAI INTERANDO, O FGETC RETORNA O NUMERO INTEIRO CORRESPONDENTE DA LETRA,
    //QUANDO ELE CHEGA AO FIM, ELE RETORNA EOF(-1), E O LEITOR PARA, POIS CHEGOU NO FIM DO ARQUIVO.
    while((c = fgetc(arq)) != EOF){
        if(c != ';'){                   //AQUI EU SEPARO AS PALAVRAS, QUANDO O CARACTERE FOR IGUAL A ';' SIMPLESMENTE NÃO COLOCO DENTRO DA STRING NOME
            arquivar.nome[y] = c;       //JÁ QUE O FGETC PEGA CHAR POR CHAR ACHEI ESSA FORMA MELHOR
            printf("%c",arquivar.nome[y]);
            y++;
        }else{                          //QUANDO A CONDIÇÃO DE SEPARAÇÃO ATIVA((c=fgetc) == EOF), EU INSIRO A STRING CAPTURADA LETRA POR LETRA NA ARVORE, SEM MEDO DE SER
            printf("\n");              //FELIZ.
            inserir(&tree,arquivar);
            int k;
            for(k=0;k<y;k++)
                arquivar.nome[k] = '\0';//AQUI EU RESETO OS INDICES PARA UTILIZAR DENOVO,
            y=0;
        }
    }
    fclose(arq);

    t_elemento dado;
    char nomes[100];
    int x = 0;
    char op;
    while(x == 0){
        telaPrincipal();
        op = getch();

        switch(op){
            case '1':
                system("cls");
                printf("Digite um nome: ");
                gets(dado.nome);

                int re = inserir(&tree, dado);
                if(re == 1){
                    printf("inserção bem sucedida\n");
                }
                else if(re == 0)
                    printf("Erro");
                system("pause");
                system("cls");
                break;
            case '2':
                system("cls");
                printf("\n\nDigite um nome para ser retirado: ");
                t_elemento dado;
                gets(dado.nome);

                int retorno;
                    retorno = remover_elemento(&tree, dado);

                    if(retorno == 1){
                        printf("\nREMOVIDO COM SUCESSO !\n\n");
                    }else{
                        printf("\nNOME NÃO EXISTE!\n\n");
                        }

                system("pause");

                break;
            case '3':
                //pesquisar valor
                system("cls");
                printf("\n\nDIGITE UM NOME PARA SER PESQUISADO: ");
                gets(dado.nome);
                t_no * aux;
                aux = pesquisa(tree,dado);


                if(aux != NULL){
                    printf("\nO NOME %s EXISTE NA ARVORE!\n\n", aux->dado.nome);
                }else{
                    printf("\nNOME INEXISTENTE!\n\n");
                }
                system("pause");
                break;
            case '4':
                system("cls");
                //esvaziar arvore
                esvaziar(&tree);
                printf("Arvore foi esvaziada\n");
                system("pause");
                system("cls");
                break;
            case '5':
                system("cls");
                if(tree != NULL){

                    printf("\nPRE ORDEM: ");
                    mostrarEmPreOrdem(tree);
                    printf("\nIN ORDEM: ");
                    mostrarInOrdem(tree);
                    printf("\nPOS ORDEM: ");
                    mostrarEmPosOrdem(tree);
                    printf("\n\n");
                    system("pause");
                    system("cls");
                    printf("\nPRE ORDEM GRAFICO: \n\n");
                    mostrarGraficamente(tree, 10, 10, 3);
                    printf("\n\n");
                    system("pause");
                    system("cls");

                }else{
                    printf("\n\nARVORE VAZIA!\n\n");
                    system("pause");
                }

                break;
            case '0':
                exit(0);
                break;
            default:
                system("cls");
                printf("Opção invalida");
        }
    }
    return 1;
}
