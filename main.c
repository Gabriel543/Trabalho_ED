#include <stdio.h>
#include <stdlib.h>

#include "TARVB.c"
#include "NODE.c"



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
        printf("%d: ",buffer.id);
        print_without_newline(buffer.texto);
        printf("\n");
        resp = buffer.id;
        buffer = VAL_Busca(arv,buffer.prox_id);
    }
    printf("\n");
    return resp;
}

void Arq_Out(TARVB *arv, NODE node){
    if(!arv){printf("Árvore não encontrada");return;}
    FILE *arq;
    char string [255];
    strcpy(string,strtok(node.nome,".")); 
    strcat(string,"_output.txt");
    arq=fopen(string,"wb");
    rewind(arq);
    VAL buffer = VAL_Busca(arv,node.no);

    while (buffer.id != -1)
    {   
        fwrite(buffer.texto,strlen(buffer.texto),1,arq);
        buffer = VAL_Busca(arv,buffer.prox_id);
    }
    fclose(arq);
    printf("\n");
}

void Arq_Out_Inv(TARVB *arv, NODE node){//A terminar, problema na logica?
    if(!arv){printf("Árvore não encontrada");return;}
    int count=0;
    FILE *arq;
    char string [255];
    strcpy(string,strtok(node.nome,".")); 
    strcat(string,"_output.txt");
    arq=fopen(string,"wb");
    rewind(arq);
    VAL buffer = VAL_Busca(arv,node.no);
    

    while (buffer.prox_id != -1)
    {   count+=1;
        buffer = VAL_Busca(arv,buffer.prox_id);
    }
    for (int i = count; i >= 0 ; i--)
    {
        fwrite(buffer.texto,strlen(buffer.texto),1,arq);
        buffer = VAL_Busca(arv,buffer.ant_id);
    }
    
    fclose(arq);
    printf("\n");
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
    
    }else{
        VAL * buffer = VAL_Busca_Ponteiro(arv,ant);
        if(buffer->prox_id == prox){
            buffer->prox_id = k.id;
            arv = TARVB_Insere(arv,k,t);
        }
    }
    return arv;

}

TARVB * Retira_na_Posicao(TARVB * arv,NODE * node, int size,int pos, int Const_t){
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
    int size = 0,Const_t;
    NODE * node = (NODE *)malloc(sizeof(NODE) * size);
    printf("Defina um valor inteiro para t(2 é o menor valor aceito) ");
    scanf("%d",&Const_t);
    if(Const_t<2){printf("Valor menor que 2 não aceito, prosseguiremos com o valor 2 por padrão ");Const_t=2;}
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
                lenght = fread(buffer.texto,1,TAM - 1,fp);
                if(!lenght){
                    break;
                }
                buffer.texto[lenght] = '\0';
                buffer.id = counter;
                buffer.prox_id = ++counter;
                arv = TARVB_Insere(arv,buffer,Const_t);
            }
            VAL * v = VAL_Busca_Ponteiro(arv,counter - 1);
            v->prox_id = -1;
        }
        fclose(fp);
        
    }
    TARVB_Imprime(arv);
    int num = 0;
    char string[255];
    int last_pos;
    while(1){
        printf("------------------\n"
               "0: Adicionar\n"
               "1: Retirar\n"
               "2: Apagar arquivo\n"
               "3: Imprimir arquivo\n"
               "4: Imprimir tabela\n"
               "5: Imprimir arvore\n"
               "-1: Sair\n"
               "------------------\n");
        scanf("%d",&num);
        if(num==4 || num==5)goto there;
        if(num==-1)goto here;
        printf("Informe o arquivo\n");
        scanf("%s",string);
        int pos =  NODE_Busca(node,size,string);
        here:
            if(num == -1) {
                for (int i = 0; i < size; i++)
                {
                    Arq_Out(arv,node[i]);
                }
                TARVB_Libera(arv);
                free(node);

                break;
            }
        if(pos == -1 && num >= 0 && num <= 3) {
            printf("Arquivo nao existe\n\n");
            continue;
        }
        there:
            switch (num)
            {
            case 0:
                last_pos = Imprimir_Arquivo(node[pos],arv);
                int pos1,pos2;
                printf("Adicionar entre as posicoes: ");
                scanf("%d %d",&pos1,&pos2);
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
                arv = Retira_na_Posicao(arv,node,size,pos1,Const_t);
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
            //case 6:
            //for (int i = 0; i < size; i++)
                //{
               // Arq_Out_Inv(arv,node[pos]);
              //  }
               // break;
            default:
                break;
            }
        }
    }