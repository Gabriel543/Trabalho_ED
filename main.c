#include <stdio.h>
#include <stdlib.h>

#include "TARVB.c"
#include "NODE.c"

#define Const_t 2


TARVB * Retirar_Arquivo(TARVB * arv, NODE node, int t){
    VAL buffer = VAL_Busca(arv,node.no);
    while (buffer.id != -1)
    {
        arv = TARVB_Retira(arv,buffer.id,t);
        buffer = VAL_Busca(arv,buffer.prox_id);
    }
    return arv;
}

int Imprimir_Arquivo(NODE node, TARVB * arv){
    printf("\n");
    VAL buffer = VAL_Busca(arv,node.no);
    int resp = -1;
    while (buffer.id != -1)
    {
        printf("%d: %s\n",buffer.id,buffer.texto);
        resp = buffer.id;
        buffer = VAL_Busca(arv,buffer.prox_id);
    }
    printf("\n");
    return resp;
}


TARVB * Inserir_na_Posicao(TARVB * arv,NODE * node, int t,VAL k,int ant, int prox,int last){
    printf("\n");
    if(ant == -1 && prox != -1){
        if(prox == node->no){
            k.prox_id = node->no;
            node->no = k.id;
            arv = TARVB_Insere(arv,k,t);
        }
    }else if(ant != -1 && prox == -1){
        if(ant == last){
            TARVB * tmp = TARVB_Busca(arv,last);
            for (int i = 0; i < tmp->nchaves; i++){
                printf("%d %d",tmp->chave[i].id, last);
                if(tmp->chave[i].id == last){
                    tmp->chave[i].prox_id = k.id;
                    arv = TARVB_Insere(arv,k,t);
                }
            }
       }
       
    }
    return arv;

}

TARVB * Retira_na_Posicao(TARVB * arv,NODE * node, int size,int pos){
    for (int i = 0; i < size; i++)
    {
        if(node[i].no == pos){
            VAL buffer = VAL_Busca(arv,pos);
            node[i].no = buffer.prox_id;
            break;
        }
    }
    return TARVB_Retira(arv,pos,Const_t);

}

void main(int argc, char ** argv){
    FILE * fp;
    TARVB * arv = TARVB_Inicializa();
    int counter = 1;
    int size = 0;
    NODE * node = (NODE *)malloc(sizeof(NODE) * size);
    for (int i = 1; i < argc; i++)
    {
        fp = fopen(argv[i], "rb");
        if(!fp)exit(1);
        if(fp){
            fseek(fp,0,SEEK_SET);
            int lenght;
            ARQ arquivo;
            node = NODE_Insere(node,counter,argv[i],&size);
            while(1){
                VAL buffer;
                lenght = fread(buffer.texto,SEEK_CUR,TAM - 1,fp);
                if(!lenght){
                    break;
                }
                buffer.texto[lenght] = '\0';
                buffer.id = counter;
                buffer.prox_id = ++counter;
                if(lenght < TAM - 1) buffer.prox_id = -1;
                arv = TARVB_Insere(arv,buffer,Const_t);
            }
        }
        fclose(fp);
    }
    TARVB_Imprime(arv);
    int num = 0;
    char string[255];
    int last_pos;
    while(1){
        printf("Formato numero arquivo\n0: adicionar\n1: retirar\n2: apagar arquivo\n3: imprimir arquivo\n4: imprimir tabela\n5: imprimir arvore\n-1: sair\n");
        scanf("%d %s",&num,string);
        int pos =  NODE_Busca(node,size,string);
        if(num == -1) break;
        if(pos == -1 && num >= 0 && num <= 3) {
            printf("Arquivo nao existe\n\n");
            continue;
        }
        switch (num)
        {
        case 0:
            last_pos = Imprimir_Arquivo(node[pos],arv);
            int pos1,pos2;
            printf("adicionar entre as posicoes: ");
            scanf("%d %d",&pos1, &pos2);
            printf("Insira o texto de tamanho %d: ",TAM - 1);
            char string[TAM];
            scanf("%s",string);
            if(strlen(string) > TAM){
                printf("Tamanho maior que %d\n",TAM - 1);
                break;
            }
            VAL buffer;
            buffer.id = counter++;
            buffer.prox_id = pos2;
            strcpy(buffer.texto,string);
            arv = Inserir_na_Posicao(arv,&node[pos],Const_t,buffer,pos1,pos2,last_pos);
            break;
        case 1:
            Imprimir_Arquivo(node[pos],arv);
            pos1;
            printf("Insira a posicao a retirar\n");
            scanf("%d",&pos1);
            arv = Retira_na_Posicao(arv,node,size,pos1);
            break;
        case 2:
            arv = Retirar_Arquivo(arv,node[pos],Const_t);
            node = NODE_Retira(node,node[pos].nome,&size);
            break;
        case 3:
            Imprimir_Arquivo(node[pos],arv);
            break;
        case 4:
            NODE_Imprime(node,size);
            break;
        case 5:
            TARVB_Imprime(arv);
            break;
        default:
            break;
        }
    }
}