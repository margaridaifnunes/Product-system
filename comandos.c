/**
 * comandos.c includes all commands functions related.
 * @file comandos.c
 * @author: ist1117809 (Margarida Isabel Farinha Nunes)
 * @date: March 2026
 */

#include "comandos.h"
#include "auxiliares.h"
#include "validacoes.h"                 


                                /** Comando p */
/**
 * @brief o comando p trata de introduzir no sistema caso seja um novo produto;
 * ou atualiza o mesmo, caso já exista. Há assim 3 casos:
 * - está no cesto: nada a fazer;
 * - não existe: adicionar;
 * - existe e não está no cesto: atualizar;
 * Output: <stock>
 * 
 * @param inventario 
 * @param linha_input 
 */
void adiciona_novo_produto(Inventario *inventario, const char *linha_input){
    char iva, descricao[BUFMAX], ean[MAX_EAN + 1];
    double preco;
    int quantidade, encontrado_cesto, indice_produto_procurado;

    sscanf(linha_input,"%*c %13s %c %lf %d %65535[^\n]",ean, &iva, &preco,
        &quantidade, descricao);
    indice_produto_procurado = procura_produto(inventario, ean,
         &encontrado_cesto);

    /** Validações */
    if (!valida_novo_produto(inventario,ean,iva,preco,quantidade,descricao))
        return;
    if (encontrado_cesto && inventario->
        produtos[indice_produto_procurado].preco != preco){
        printf("%s\n", EPRODEMUSO); return;
    }
    else if (indice_produto_procurado == -1){ /**<produto não existe */
        if (inventario->quantidade_produtos >= MAX_PRODUTOS){
            printf("%s\n",EINVPRODUTO); return;
        }
        introduz_produto(inventario, ean, iva, preco, quantidade, descricao);
        /** Output pedido: (imprimir o stock); -1 índices começam no 0 */
        printf("%d\n",(inventario->produtos[inventario->quantidade_produtos
            - 1].quantidade_stock));
    }      
    else {  /**< existe, não está no cesto, atualizar o produto */
        atualiza_produto(inventario, ean, iva, preco, quantidade, descricao,
            indice_produto_procurado); 
        /** Output pedido: (imprimir o stock) */
        printf("%d\n",(inventario->produtos[indice_produto_procurado]
            .quantidade_stock));
    }
}

                                /** Comando l */
/**
 * @brief lista_produtos_disponiveis está encarregue que listar os produtos
 * disponíveis no sistema (stock > 0), por ordem de criação. Há assim 3 casos:
 *  - sem argumentos: lista todos os produtos em sistema;
 *  - 1 wildcard: lista produtos válidos;
 *  - 2 wildcard: lista produtos válidos;
 * Output: <ean> <iva> <preço> <quantidade-vendida> <quantidade-disponível>
 *         <descrição>
 * 
 * @param inventario 
 * @param linha_input 
 */
void lista_produtos_disponiveis(const Inventario *inventario,
        const char *linha_input){
    char argumentos[BUFMAX];
    int encontrou = 0;
    const char *arg_individual;
    argumentos[0] = '\0';   /**< caso não haja argumentos há um '\0' */
    /** 65535 = 655356(BUFMAX) -1('\0') */
    sscanf(linha_input,"%*c %65535[^\n]", argumentos);   /**< args - string */
    arg_individual = strtok(argumentos, " \t\n");  /**< separa por espaços */

    if ((argumentos[0] =='\0') || (strcmp(argumentos, "*") == 0)){ 
        /** se não há args listo todos os produtos */
        for (int i=0; i< (inventario->quantidade_produtos); i++)
            if (inventario->produtos[i].quantidade_stock > 0){
                impressao_produtos_disponiveis(inventario,i);
                encontrou = 1;
            }
        if (encontrou == 0) printf ("*: no such product\n");
    }
    else{
        while (arg_individual != NULL){ encontrou = 0;
            /** trata- los individualmente */
            for (int j=0; j < inventario->quantidade_produtos; j++)
                if (valida_padrao_wildcard(arg_individual, 
                    inventario->produtos[j].ean)){
                    impressao_produtos_disponiveis(inventario,j);
                    encontrou = 1;
                }
            if (encontrou == 0)
                printf ("%s: no such product\n", arg_individual);
            arg_individual = strtok(NULL, " \t\n"); /**< continua prox. arg */
        }
    }  
}


                            /** Comando a */
/**
 * @brief incrementa_quantidade_cesto é a função associada ao comando a.
 * Esta é responsável por adicionar ao cesto a quantidade do produto indicado. 
 * Há assim 3 casos possíveis:
 *  - sem argumentos: lista os produtos adicionados por ordem crescente de ean;
 *  - ean: aumeta 1 unidade ao produto;
 *  - quantidade e ean: adiciona/remove n unidade ao produto;
 * Output: <iva> <preço-unitário> <quantidade-total> <preço-total-com-iva>
 *         <descrição>
 * 
 * @param inventario 
 * @param linha_input 
 */
void incrementa_quantidade_cesto(Inventario *inventario,
        const char *linha_input){
    char argumentos[BUFMAX], token1[BUFMAX], token2[BUFMAX];
    int num_args;
    argumentos[0] = '\0';
    sscanf(linha_input,"%*c %65535[^\n]", argumentos);  /**< args -str */
    /** CASO 1: */
    if (argumentos[0] =='\0'){  /**< somente o 'a' */
    /** listagem dos produtos adicionados por ordem crescente de ean */
        ordena_cesto_pelo_ean(inventario);
        for (int i=0; i<inventario->quantidade_itens_cesto; i++){
            if(inventario->itens_cesto[i].quantidade_do_produto > 0){
                int idx_produto = inventario->itens_cesto[i].indice_produto;
                impressao_item_cesto(inventario, idx_produto, i);   
            }
        }
        return;
    }
    num_args = sscanf (argumentos, "%65535s %65535s", token1, token2);
    /** CASO 2: só temos o ean (token1 -> ean) */
    if(num_args == 1)
        case_2_a(inventario,token1);
    /** CASO 3: temos a quantidade e o ean; (token1->quantidade;token2->ean)*/
    else{
        int quantidade = atoi(token1);
        case_3_a(inventario, token2, quantidade);
    }
}

                            /** Comando r */
/**
 * @brief resume_faturacao visa, como o nome indica, resumir a 
 * faturação. Há assim 2 casos:
 *  - sem argumentos: de acordo com a ordem da tabela de IVA,
 *    por ordem alfabética do código;
 *   <número-total-de-items-comprados> <número-total-de-facturas-emitidas>
 *   <valor-total-facturado>
 *  - ean: imprimir a informação desse produto;
 *   <quantidade-disponível> <quantidade-vendida> <descrição>
 * 
 * @param inventario 
 * @param linha_input 
 */

void resume_faturacao (const Inventario *inventario, const char *linha_input){
    char argumentos[BUFMAX];
    /** tratamento do input */
    argumentos[0] = '\0';
    sscanf(linha_input,"%*c %65535[^\n]", argumentos);  /**< args - str */
    /** CASO 1 (só temos o comando): */
    if (argumentos[0] =='\0'){  /**< somente o 'r' */
        printf("%d %d %.2f\n", inventario->total_itens_comprados,
            inventario->total_faturas_emitidas,
            inventario->valor_total_faturado);
        for (char letra =  'A'; letra <= 'Z'; letra++){
            for(int i=0; i<inventario->num_classes_iva_envolvidas; i++)
                if(inventario->tabela_iva[i].letra_maiusc == letra)
                    printf("%c %d%%\n", inventario->tabela_iva[i].letra_maiusc,
                        inventario->tabela_iva[i].taxa_iva);
        }
    }
    /** Caso 2: (com o ean) */
    else{
        char ean[MAX_EAN + 1];
        sscanf(argumentos,"%13s",ean);
        if (!valida_ean(ean)){
            printf("%s\n", EINVEAN); return;
        }
        int indice = obter_indice_produto(inventario, ean);
        if (indice == -1){
            printf("%s: no such product\n",ean); return;
        }
        printf("%d %d %s\n",inventario->produtos[indice].quantidade_stock,
            inventario->produtos[indice].quantidade_vendida,
            inventario->produtos[indice].descricao);
    }
}

                            /** Comando f */
/**
 * @brief fatura_produtos_cesto está encarregue de toda a secção de faturação
 * considerando o cliente, definido por nome e nif. No final, tem ainda de se
 * esvaziar o cesto. 
 * Aqui há 3 casos possíveis:
 *  - sem argumentos: nif: 999999999 e nome_cliente: Cliente_final;
 *  - nome_cliente: nif: 999999999;
 *  - nif e nome_cliente:
 * nota: se o nome_cliente for error proceder à devolução. As faturas são 
 * numeradas a partir do 1.
 * Output: <número-de-produtos> <valor-pago> <número-de-factura>
 * 
 * @param inventario 
 * @param linha_input 
 */
void fatura_produtos_cesto(Inventario *inventario, const char *linha_input){
    int num_args;
    char argumentos[BUFMAX], token1[BUFMAX], token2[BUFMAX];
    argumentos[0] = '\0'; /**< caso não haja argumentos há um '\0' */
    token2[0] = '\0';
    sscanf(linha_input,"%*c %65535[^\n]", argumentos);

    /** CASO 1: só temos o comando f */
    if (argumentos[0] =='\0'){  /**< somente o 'f' */
        case_1_f(inventario);
        return;
    }
    /** CASOS 2 e 3 */
    if (argumentos[0]== '"'){
        /** só tiro as aspas na validação */
        sscanf(argumentos, "%65535[^\n]", token1);
        case_2_f(inventario,token1);
        return;
    }
    num_args = sscanf (argumentos, "%65535s %65535[^\n]", token1, token2);
    if(num_args <= 1)
        case_2_f(inventario,token1);
    else
        case_3_f(inventario, token1, token2);
}

                            /** Comando c */
/**
 * @brief lista_faturas_sistema tem a função de listar as faturas todos
 * associadas ao sistema; ou associadas a um determinado cliente.
 * Há 2 casos possíveis:
 *  - sem argumentos: por ordem alfabética de nome_cliente, cronológica mente
 * por ordem de facturação para cada cliente;
 *  - nome_cliente: faturas do cliente por ordem cronológica de facturação;
 * Output: <número-factura> <valor-facturado> <nome-cliente>
 * 
 * @param inventario 
 * @param linha_input 
 */
void lista_faturas_sistema(Inventario *inventario, const char *linha_input){
    char argumentos[BUFMAX];
    argumentos[0] = '\0'; /**< caso não haja argumentos há um '\0' */

    sscanf(linha_input,"%*c %65535[^\n]", argumentos);
    /** CASO 1: só temos o comando c */
    if (argumentos[0] =='\0')
        case_1_c(inventario);  
    /** CASO 2: temos o nome do cliente */
    else{
        char token1[BUFMAX];
        sscanf (argumentos, "%65535s", token1);
        case_2_c(inventario,argumentos);        
    }
}


                            /** Comando d */
/**
 * @brief apaga_registo tem a função de apagar o registo de um produto ou
 * fatura. Aqui há 2 casos possíveis:
 *  - numero (nº de fatura): <valor-pago> <nif> <nome-cliente>;
 *  - numero (código ean) e quantidade (a retirar, valor positivo): 
 *    <quantidade-restante> <descrição>. Se a quantidade em stock ficar a 
 *    zeros deve remover-se o produto de produtos.
 * 
 * @param inventario 
 * @param linha_input 
 */
void apaga_registo(Inventario *inventario, const char *linha_input){
    char argumentos[BUFMAX];
    int num_args, numero, quantidade;
    sscanf(linha_input,"%*c %65535[^\n]", argumentos);  /**< args - str */

    num_args = sscanf (argumentos, "%d %d", &numero, &quantidade);
    /** CASO 1: temos o número da fatura */
    if(num_args == 1)
        case_1_d(inventario,numero);

    /** CASO 2:  temos código e quantidade */
    else{
        char ean[MAX_EAN + 1];  /**< +1 pelo \0 */
        sscanf (argumentos, "%13s %d", ean, &quantidade);
        case_2_d(inventario,ean,quantidade);
    }
}


/**
 * @brief cria_item_cesto está responsável pela criação de um novo item no
 * cesto e pela atualização dos dados necessário.
 * 
 * @param inventario 
 * @param indice 
 * @param quantidade 
 * @param indice_cesto 
 */
static void cria_item_cesto(Inventario *inventario, int indice, int quantidade,
    int *indice_cesto){

    Item_cesto item;
    item.indice_produto = indice;
    item.quantidade_do_produto = quantidade;
    inventario->itens_cesto[inventario->quantidade_itens_cesto] = item;
    inventario->quantidade_itens_cesto++;
    *indice_cesto = inventario->quantidade_itens_cesto - 1;
}


/**
 * @brief case_2_a trata do caso é que só se recebe o código ean como argumento
 * à chamada do comando a.
 * 
 * @param inventario 
 * @param ean 
 */
void case_2_a(Inventario *inventario, const char *ean){
    int indice, encontrado_cesto, indice_cesto;
    if (!valida_ean(ean)){ printf("%s\n", EINVEAN); return;} 
    
    indice = procura_produto(inventario, ean, &encontrado_cesto);
    if (indice == -1){ printf("%s: no such product\n",ean); return;}

    indice_cesto = obter_indice_cesto(inventario,indice);
    if (inventario->produtos[indice].quantidade_stock <= 0){
        printf("no stock\n"); return;
        }

    if (indice_cesto != -1){    /**< se existe a quantidade aumenta */
        inventario->itens_cesto[indice_cesto].quantidade_do_produto +=1;
        /** Atualizar o stock */
        inventario->produtos[indice].quantidade_stock -= 1;
        inventario->produtos[indice].quantidade_vendida += 1;
    }
    else{
        /** cria item cesto */
        cria_item_cesto(inventario, indice, 1, &indice_cesto);
        inventario->produtos[indice].quantidade_stock -= 1;
        inventario->produtos[indice].quantidade_vendida += 1;
    }
    impressao_item_cesto(inventario, indice, indice_cesto);
}


/**
 * @brief case_3_a trata do caso é que se recebe o código ean e a quantidade
 * como argumento à chamada do comando a.
 * 
 * @param inventario 
 * @param ean 
 * @param quantidade 
 */
void case_3_a(Inventario *inventario, const char *ean, int quantidade){
    int encontrado_cesto, indice, idx_cesto;
    if (!valida_ean(ean)){  printf("%s\n", EINVEAN);    return;}

    indice = procura_produto(inventario, ean, &encontrado_cesto);
    if (indice == -1){  printf("%s: no such product\n", ean);   return;}

    idx_cesto = obter_indice_cesto(inventario,indice);
    if (quantidade<0){  /**< remover */
        if (idx_cesto == -1||inventario->itens_cesto[idx_cesto].
            quantidade_do_produto < -quantidade){
            printf("%s\n", EINVQUANTIDADE); return;}

        /** Remover item do cesto */
        inventario->itens_cesto[idx_cesto].quantidade_do_produto += quantidade;
        if(inventario->itens_cesto[idx_cesto].quantidade_do_produto == 0){
            for(int j=idx_cesto; j < inventario->quantidade_itens_cesto; j++)
                inventario->itens_cesto[j] = inventario->itens_cesto[j+1];

            inventario->quantidade_itens_cesto -= 1;
        }
    }
    else{   /**< adicionar */
        if (inventario->produtos[indice].quantidade_stock < quantidade){
            printf("no stock\n");   return;}
        /** incrementar quantidade */
        if (idx_cesto != -1)
            inventario->itens_cesto[idx_cesto].quantidade_do_produto
                += quantidade;
        else    /**<criar item novo */
            cria_item_cesto(inventario, indice, quantidade, &idx_cesto);
    }
/** Atualizar stock's */
inventario->produtos[indice].quantidade_vendida += quantidade;
inventario->produtos[indice].quantidade_stock -= quantidade;
impressao_item_cesto(inventario, indice, idx_cesto);
}


/**
 * @brief case_1_f está encarregue de asseguar o caso em que o comando f é
 * chamado sem argumentos. Aqui, assume-se o nome_cliente como "Cliente final"
 * e o nif como 999999999.
 * 
 * @param inventario 
 */
void case_1_f(Inventario *inventario){
    char *nome_cliente;
    nome_cliente = malloc(strlen("Cliente final") + 1);
    if (nome_cliente == NULL){
        printf("%s\n", EMEMORIA);
        return;
        }
    strcpy(nome_cliente,"Cliente final");   /**< nome por defeito */
    emite_fatura(inventario,999999999, nome_cliente);   /**< nif por defeito */
}


/**
 * @brief case_2_f está encarregue de asseguar o caso em que o comando f é
 * chamado somente com 1 argumento: o nome_cliente. Aqui, deve considerar-se
 * o nif: 999999999.
 * 
 * @param inventario 
 * @param token1 
 */
void case_2_f(Inventario *inventario, const char *token1){
    char *nome_cliente, *nome_final;
    nome_cliente = malloc(strlen(token1) + 1); /**< nome na memoria */
    nome_final = malloc(strlen(token1) + 1);
    
    slot_f(inventario,nome_cliente, nome_final, token1,999999999);
}

/**
 * @brief case_3_f está encarregue de asseguar o caso em que o comando f é
 * chamado com 2 argumentos: o nome_cliente e o nif.
 * 
 * @param inventario 
 * @param token1 
 * @param token2 
 */
void case_3_f(Inventario *inventario, const char *token1, const char *token2){
    char *nome_cliente, *nome_final;
    int nif_int;

    if(!valida_nif(token1)){
        printf("%s: no such nif\n", token1);
        return;
    }
    nif_int =atoi(token1);

    nome_cliente = malloc(strlen(token2) + 1); /**<nome ocupa memoria */
    nome_final = malloc(strlen(token2) + 1);
    slot_f(inventario,nome_cliente, nome_final,token2, nif_int);
}


/**
 * @brief case_1_c está encarregue do caso em que o comando c é chamado sem
 * argumentos onde é necessário imprimir as faturas ordenadas alfabeticamente
 * consoante o nome_cliente e; para o mesmo cliente por ordem cronológica de
 * faturação. 
 * 
 * @param inventario 
 */
void case_1_c(Inventario *inventario){
    int *indices,i,k;
    indices = malloc(inventario->quantidade_faturas*sizeof(int));
    if(indices == NULL){
        printf("%s\n", EMEMORIA);
        return;
    }
    /** Loop de atribuição dos indices num array dinâmico */
    for (i = 0; i < inventario->quantidade_faturas; i++)
        indices[i] = i;

    /** ordena por ordem alfabética e por ordem cronológica de faturação */
    /** insertion sort começa em 1 pois esse já está ordenado,
     * e vamos comparando elemento 'atual' com os que estão à sua esquerda; se
     * necessário trocam até estar ordenado à esquerda */
    for(i=1; i < inventario->quantidade_faturas; i++){
        int indice_atual = indices[i];
        int j= i-1; /**< i começa a 1; j só pode começar a partir do 0 */
        while(j>=0 && strcmp(inventario->faturas[indices[j]]->cliente.nome,
                inventario->faturas[indice_atual]->cliente.nome)> 0){
            indices[j+1] = indices[j] ; /**< arrasta para a direita j->j+1 */
            j--;    /**< vai movendo o j para a esquerda */
        }
        indices[j+1]= indice_atual; /**< ao descobrir com qual trocar, troca */
    }

    /** loop que percorre todas as faturas */
    for (k=0; k < inventario->quantidade_faturas; k++){
        int num_fatura = inventario->faturas[indices[k]]->numero_fatura;
        double valor_faturado = inventario->faturas[indices[k]]->custo_total;
        const char *nome_cliente = 
                    inventario->faturas[indices[k]]->cliente.nome;
        printf("%d %.2f %s\n",num_fatura, valor_faturado, nome_cliente);
    }
    free(indices);
}


/** case_2_c está encarregue de tratar do caso em que o comando c é chamado com
 * 1 argumento: o nome_cliente. Aqui tem de ser imprimir as faturas por ordem
 * cronológica.
 * @brief 
 * 
 * @param inventario 
 * @param token1 
 */
void case_2_c(Inventario *inventario, const char *token1){
    char *nome_cliente;
    int encontrou=0;
    nome_cliente = malloc(strlen(token1) + 1); /**< nome na memoria */
    if(nome_cliente == NULL){
        printf("%s\n",EMEMORIA);
        return;
    }

    strcpy(nome_cliente, token1);
    if(!valida_nome_cliente(nome_cliente)){
        printf("%s\n",EINVNOME);
        free(nome_cliente);
        return;
    }
    /** loop que encontra todas as faturas dum certo cliente */
    for (int i=0; i<inventario->quantidade_faturas; i++){
        if (strcmp(inventario->faturas[i]->cliente.nome, nome_cliente)==0){
            encontrou = 1; /**< flag acionada */
            int num_fatura = inventario->faturas[i]->numero_fatura;
            double valor_faturado = inventario->faturas[i]->custo_total;
            printf("%d %.2f %s\n",num_fatura, valor_faturado, nome_cliente);
        }
    }
    if (encontrou == 0)
        printf("%s: no such client\n", nome_cliente);

    free(nome_cliente);
}


/**
 * @brief case_1_d tem a função de lidar com o comando d, quando este é chamado
 * com 1 só argumento, o número (nº da fatura).
 * 
 * @param inventario 
 * @param numero 
 */
void case_1_d(Inventario *inventario, int numero){
    int i, indice = -1;
    const char *nome_cliente;
    /** verificar se a fatura existe */
    for (i = 0; i < inventario->quantidade_faturas; i++){
        if (inventario->faturas[i]->numero_fatura == numero){
            indice = i;
            break;
        }
    }
    if(indice == -1){
        printf("%d: no such invoice\n", numero);
        return;
    }

    /** faturas já estão previamente ordenadas */
    double valor_pago = inventario->faturas[numero - 1]->custo_total;
    int nif = inventario->faturas[numero - 1]->cliente.nif;
    nome_cliente = inventario->faturas[numero -1]->cliente.nome;
    printf("%.2f %d %s\n", valor_pago, nif, nome_cliente);

    /** atualizar quantidades e custos */
    inventario->total_itens_comprados -= inventario->faturas[numero - 1]->
        quantidade_itens_comprado;
    inventario->valor_total_faturado  -= inventario->faturas[numero - 1]->
        custo_total;

    /** libertação de ponteiros */
    free(inventario->faturas[numero - 1]->cliente.nome);
    free(inventario->faturas[numero - 1]);

    /** apagar a fatura é mover as faturas da sua da direita para a esq. */
    for (i=numero - 1; i<inventario->quantidade_faturas -1; i++){
        inventario->faturas[i]=inventario->faturas[i+1];
    }
    /** atualizar a quantidade de faturas ao remover uma */
    inventario->quantidade_faturas -= 1;
}


/**
 * @brief case_2_d tem a função de lidar com o comando d, quando este é chamado
 * com 2 argumentos: o número (código ean) e a quantidade que se pretende
 * remover do produto indicado.
 * 
 * @param inventario 
 * @param ean 
 * @param quantidade 
 */
void case_2_d(Inventario *inventario, const char *ean, int quantidade){
    int indice, encontrado_cesto, quantidade_restante,i;
    const char *descricao;

    if (!valida_ean(ean)){  printf("%s\n",EINVEAN); return; }    

    indice = procura_produto(inventario, ean, &encontrado_cesto);
    if (indice == -1){
        printf("%s: no such product\n",ean); return;
    }
    for (i=0; i<inventario->quantidade_itens_cesto; i++){
        if(indice == inventario->itens_cesto[i].indice_produto){
            printf("%s\n", EPRODEMUSO);
            return;
        }
    }
    if(quantidade <= 0 || quantidade > inventario->produtos[indice].
        quantidade_stock){
        printf("%s\n", EINVQUANTIDADE);
        return;
    }
    
    quantidade_restante = inventario->produtos[indice].quantidade_stock;
    descricao = inventario->produtos[indice].descricao;
    /** atualizar o stock */
    quantidade_restante -= quantidade;
    printf("%d %s\n",quantidade_restante, descricao);

    /** atualizar no inventário */
    inventario->produtos[indice].quantidade_stock = quantidade_restante;

    if (quantidade_restante == 0){ 
        /** remoção do produto */
        for (i=indice; i<inventario->quantidade_produtos -1; i++)
            inventario->produtos[i] = inventario->produtos[i+1];

        inventario->quantidade_produtos -= 1;
    }
}
