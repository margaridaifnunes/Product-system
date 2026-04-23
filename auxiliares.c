/**
 * auxiliares.c includes every other necessary function 
 * used in multiple occasions.
 * @file auxiliares.c
 * @author: ist1117809 (Margarida Isabel Farinha Nunes)
 * @date: March 2026
 */
#include "auxiliares.h"
#include "validacoes.h"


/**
 * @brief procura_produto visa averiguar se o produto, está, ou não no cesto.
 * Usa-se a variável de estado encontrado_cesto para saber se o produto está no 
 * cesto. No fim, se o produto tiver sido encontrado, devolve a sua posição no 
 * array; caso contrário, devolve o -1.
 * 
 * @param inventario
 * @param ean_recebido
 * @param encontrado_cesto
 * @return int
 */
int procura_produto(const Inventario *inventario,const char *ean_recebido,
    int *encontrado_cesto){
    int i, j;
    *encontrado_cesto = 0;  /** variável de estado */

    /** loop pelo produtos existentes; i-> idx no inventário */
    for (i=0; i < (inventario->quantidade_produtos); i++){
        /** ao encontrar o produto */
        if (strcmp(ean_recebido, (inventario->produtos[i].ean)) == 0){
            /** verificar se o produto está no cesto */
            for (j = 0; j < inventario->quantidade_itens_cesto; j++){
                /** j -> idx no cesto */
                if (inventario->itens_cesto[j].indice_produto == i){
                    *encontrado_cesto = 1;  /**< está no cesto */
                }
            }
            return i;   /**< índice do produto no array */
        }
    }
    return -1; /**< não encontrado */
}


/**
 * @brief introduz_produto visa introduzir um produto, bem como os dados a
 * si associados, ao sistema.
 * 
 * @param inventario 
 * @param ean 
 * @param iva 
 * @param preco 
 * @param quantidade 
 * @param descricao 
 */
void introduz_produto(Inventario *inventario, const char *ean, char iva,
                    double preco, int quantidade, const char *descricao){
    /** índice começa em 0 e a quantidade em 1; logo é o índice_novo */
    int posicao_novo_produto = inventario->quantidade_produtos;
    /** Adição dos campos do produto */
    strcpy(inventario->produtos[posicao_novo_produto].ean, ean);    /**< str */
    inventario->produtos[posicao_novo_produto].classe_iva = iva;
    inventario->produtos[posicao_novo_produto].preco = preco;
    inventario->produtos[posicao_novo_produto].quantidade_stock = quantidade;
    strcpy(inventario->produtos[posicao_novo_produto].descricao,descricao);
    /** Aumentar o nº de produtos registados */
    inventario->quantidade_produtos += 1;
}


/**
 * @brief atualiza_produto visa atualizar os dados do produto no sistema.
 * 
 * @param inventario 
 * @param ean 
 * @param iva 
 * @param preco 
 * @param quantidade 
 * @param descricao 
 * @param indice_produto_procurado 
 */
void atualiza_produto(Inventario *inventario, const char *ean, char iva,
                    double preco, int quantidade, const char *descricao,
                    int indice_produto_procurado){
    /** Atualização dos campos do produto */
    strcpy(inventario->produtos[indice_produto_procurado].ean, ean);
    inventario->produtos[indice_produto_procurado].classe_iva = iva;
    inventario->produtos[indice_produto_procurado].preco = preco;

    inventario->produtos[indice_produto_procurado].quantidade_stock
     += quantidade;

    strcpy(inventario->produtos[indice_produto_procurado].descricao,
        descricao);
}

/**
 * @brief ordena_cesto_pelo_ean ordena, como o nome indica, os produtos
 * do cesto por ordem crescente do ean.(insertion sort)
 * 
 * @param inventario 
 */
void ordena_cesto_pelo_ean (Inventario *inventario){
    int i;

    /** insertion sort: 1º posição já está ordenada */
    for (i=1; i<inventario->quantidade_itens_cesto; i++){
        Item_cesto item_atual = inventario->itens_cesto[i];
        int j = i - 1;  /**< j é o elem à esquerda de i */
        while(j>=0 && 
            /**condição de ordenação */
            strcmp(inventario->produtos[item_atual.indice_produto].ean, 
                inventario->
                produtos[inventario->itens_cesto[j].indice_produto].ean) < 0){

            inventario->itens_cesto[j+1] =  inventario->itens_cesto[j];
            j -= 1; /**< vai movendo o j para a esquerda */
        }
        /** encontrou a posição correta e efetua a troca */
        inventario->itens_cesto[j+1] = item_atual;
    }
}


/**
 * @brief obter_taxa_iva visa aferir qual a taxa de iva a aplicar
 * sobre o preço do produto consoante a sua classe.
 * 
 * @param classe_iva 
 * @param inventario 
 * @return int 
 */
int obter_taxa_iva(char classe_iva, Inventario *inventario){
    int i, taxa_a_aplicar;

    /** loop executado ,no máximo, o nº de classes possíveis */
    for (i=0; i<inventario->num_classes_iva_envolvidas; i++){
        /** se a encontrar, descobrimos a taxa */
        if (inventario->tabela_iva[i].letra_maiusc == classe_iva){
            taxa_a_aplicar = inventario->tabela_iva[i].taxa_iva;
            return taxa_a_aplicar;
        }
    }
    return 0;
}


/**
 * @brief obter_indice_cesto visa determinar qual o índice no cesto
 * sabendo o índice do produto em Produtos.No fim, devolve o seu índice
 * no cesto; se não for encontrado devolve -1.
 * 
 * @param inventario 
 * @param indice_produtos 
 * @return int 
 */
int obter_indice_cesto(const Inventario *inventario, int indice_produtos){
    int i;

    /** loop que percorre o cesto */
    for (i=0; i<inventario->quantidade_itens_cesto; i++){
        if(indice_produtos == inventario->itens_cesto[i].indice_produto)
            return i;
    }
    return -1;  /**< não está no cesto */
}


/**
 * @brief arredonda trata do arredondamento simétrico solicitado
 * para as taxas de iva.
 * 
 * @param valor 
 * @return double 
 */
double arredonda(double valor){
    double ajuste = ((valor >= 0) ? 0.0001 : -0.0001);
    /** *100 para não mexer com os cêntimos diretamente */
    return (int)((valor + ajuste) * 100.0 + (valor >= 0 ? 0.5 : -0.5)) / 100.0;
}


/**
 * @brief impressao_item_cesto trata de imprimir o output da forma
 * esperada para o comando a;
 * 
 * @param inventario
 * @param indice    (índice em produtos)
 * @param indice_cesto
 */
void impressao_item_cesto(Inventario *inventario, int indice,
    int indice_cesto){
    printf("%c %.2f %d %.2f %s\n",inventario->produtos[indice].classe_iva,
            inventario->produtos[indice].preco,
            inventario->itens_cesto[indice_cesto].quantidade_do_produto,

            (arredonda(inventario->
                itens_cesto[indice_cesto].quantidade_do_produto*
                (inventario->produtos[indice].preco)*
                (1 + obter_taxa_iva(inventario->produtos
                [indice].classe_iva,inventario)/100.0))),

            inventario->produtos[indice].descricao);  
}


 /**
  * @brief  obter_indice_produto determina o índice do produto
  * em produtos, sabendo o seu código ean.   
  * 
  * @param inventario 
  * @param ean 
  * @return int 
  */
int obter_indice_produto(const Inventario *inventario, const char *ean){
    int i;
    /** loop que percorre os produtos */
    for (i=0; i<inventario->quantidade_produtos; i++){
        if (strcmp(ean, inventario->produtos[i].ean) == 0)  /**< encontrado */
            return i;
    }
    return -1;  /**< caso não esteja em produtos */
}


/**
 * @brief adiciona_taxa_iva adiciona uma entrada_iva composta por:
 * uma classe_iva e por uma taxa a aplicar à tabela. 
 * 
 * @param inventario 
 * @param classe_iva 
 * @param taxa 
 */
void adiciona_taxa_iva (Inventario *inventario, char classe_iva, int taxa){
    /** adicionar os campos necessários: classe e taxa */
    inventario->tabela_iva[inventario->num_classes_iva_envolvidas].
        letra_maiusc = classe_iva;
    inventario->tabela_iva[inventario->num_classes_iva_envolvidas].
        taxa_iva = taxa;

    /** incrementar a quantidade de classes envolvidas */
    inventario->num_classes_iva_envolvidas += 1;
}


/**
 * @brief adiciona_taxas_by_default contem os dados a utilizar
 * das classes e taxas de iva a aplicar, se nenhum ficheiro for fornecido.
 * 
 * @param inventario 
 */
void adiciona_taxas_by_default (Inventario *inventario){
    inventario->num_classes_iva_envolvidas = 4;

    inventario->tabela_iva[0].letra_maiusc = 'A';   
    inventario->tabela_iva[0].taxa_iva = 0;

    inventario->tabela_iva[1].letra_maiusc = 'B';    
    inventario->tabela_iva[1].taxa_iva = 6;

    inventario->tabela_iva[2].letra_maiusc = 'C';    
    inventario->tabela_iva[2].taxa_iva = 13;

    inventario->tabela_iva[3].letra_maiusc = 'D';    
    inventario->tabela_iva[3].taxa_iva = 23;
}


/**
 * @brief calcula_custo_fatura visa determinar o custo total dos produtos
 * que se encontram no cesto.
 * 
 * @param inventario 
 * @return double 
 */
double calcula_custo_fatura(Inventario *inventario){
    double custo_total_fatura = 0;
    int i;
    /** loop que percorre os itens do cesto */
    for (i=0; i<inventario->quantidade_itens_cesto; i++){
        /** índice em produtos */
        int indice_p = inventario->itens_cesto[i].indice_produto;
        /** dados necessários: preço, quantidade, iva */
        double preco_p = inventario->produtos[indice_p].preco;
        int quantidade_p = inventario->itens_cesto[i].quantidade_do_produto;
        char classe_iva = inventario->produtos[indice_p].classe_iva;
        double taxa_iva = obter_taxa_iva(classe_iva, inventario);
        /** preço final de determinado itens (com taxas) */
        double preco_c_iva = arredonda(quantidade_p * preco_p *
                            (1 + taxa_iva/100.0));
        /** incrementar ao preço final da fatura */
        custo_total_fatura += preco_c_iva;
    }
    return custo_total_fatura;
}


/**
 * @brief devolucao_cesto repõe o stock dos produtos, quando é
 * efetuada uma devolução.
 * 
 * @param inventario 
 */
void devolucao_cesto(Inventario *inventario){
    int i,num_itens_a_repor;
    /** repor todos os itens atualmente no cesto */
    num_itens_a_repor = inventario->quantidade_itens_cesto;
    /** loop que percorre o cesto */
    for (i=0; i<num_itens_a_repor; i++){
        int indice = inventario->itens_cesto[i].indice_produto;
        /** repor o stock */
        inventario->produtos[indice].quantidade_stock += 
            inventario->itens_cesto[i].quantidade_do_produto;
        /** repor quantidade_vendida */
        inventario->produtos[indice].quantidade_vendida -= 
            inventario->itens_cesto[i].quantidade_do_produto;
        /** repor a condição de cesto sem o produto */
        inventario->itens_cesto[i].quantidade_do_produto = 0;
    }
    /** repor a condição de cesto vazio */
    inventario->quantidade_itens_cesto = 0;
}

/**
 * @brief preenche_fatura trata da atribuição dos parâmetro necessários
 * para posteriomene imprimir a fatura. Assim, trata-se de todos os dados e
 * atualizações necessárias referentes ao sistema e à fatura em si, decorrentes
 * da emissão da fatura.
 * 
 * @param inventario 
 * @param num_fatura (índice no array)
 * @param nif 
 * @param nome_cliente 
 */
void preenche_fatura(Inventario * inventario, int num_fatura,
                    int nif, char *nome_cliente){
    int i;
    double custo_total_fatura;
    /** Atribuição dos parâmetros por defeito */
    inventario->faturas[num_fatura]->cliente.nif = nif;
    inventario->faturas[num_fatura]->cliente.nome = nome_cliente;

    /** Incrementação do nº de produtos comprados e do custo total */
    custo_total_fatura = calcula_custo_fatura(inventario);
    inventario->faturas[num_fatura]->custo_total = custo_total_fatura;

    inventario->total_faturas_emitidas += 1;
    inventario->faturas[num_fatura]->numero_fatura = 
        inventario->total_faturas_emitidas; /**< faturas começam em 1 */

    inventario->valor_total_faturado += custo_total_fatura;

    /** loop que percorre os itens do cesto */
    for (i=0; i<inventario->quantidade_itens_cesto; i++){
        inventario->faturas[num_fatura]->quantidade_itens_comprado += 
        inventario->itens_cesto[i].quantidade_do_produto;
    }

    inventario->total_itens_comprados += 
        inventario->faturas[num_fatura]->quantidade_itens_comprado;   
}


/**
 * @brief emite_fatura está encarregue de ver os custos associados ao
 * itens do cesto, atualizar o stock, limpar o cesto e garantir que há
 * memória suficiente para garantir que o processo ocorre corretamente.
 * No fim, imprime a fatura e incrementa o número de faturas.
 * 
 * @param inventario 
 * @param nif 
 * @param nome_cliente 
 */
void emite_fatura(Inventario *inventario, int nif, char *nome_cliente){
    int num_fatura = inventario->quantidade_faturas; /** índice a preencher */

    /** aloca espaço para fatura* */
    inventario->faturas = realloc(inventario->faturas,
        (num_fatura + 1)*sizeof(Fatura*));

    if (inventario->faturas == NULL){
        printf("%s\n", EMEMORIA);
        return;
    }
    /** alocar espaço para uma fatura em si (os subcampos da mesma) */
    inventario->faturas[num_fatura] = malloc(sizeof(Fatura));

    if(inventario->faturas[num_fatura] == NULL){
        printf("%s\n", EMEMORIA);
        return;
        }
    /** Inicializar quantidade de itens da fatura */
    inventario->faturas[num_fatura]->quantidade_itens_comprado = 0;

    /** Preenchimento da fatura */
    preenche_fatura(inventario, num_fatura, nif, nome_cliente);   
    /** Impressão da fatura */
    printf("%d %.2f %d\n",inventario->faturas[num_fatura]->
        quantidade_itens_comprado,
        inventario->faturas[num_fatura]->custo_total,
        inventario->quantidade_faturas + 1);    /** índices começam a 0 */

    /** Limpar o cesto e incrementar as faturas */
    inventario->quantidade_faturas += 1;

    for (int i = 0; i < inventario->quantidade_itens_cesto; i++)
        inventario->itens_cesto[i].quantidade_do_produto = 0;
    inventario->quantidade_itens_cesto = 0;
}


/**
 * @brief liberta_memoria visa dar free de todos os campos em que 
 * aloquei memória dinamicamente, me que usei malloc ou realloc's.
 * São eles: o nome dos clientes nas faturas, as próprias faturas e
 * o array de ponteiros com o realloc em emite_fatura.
 * 
 * @param inventario 
 */
void liberta_memoria(Inventario *inventario){
    int i;
    /** libertar os campos em que usei malloc */
    for(i=0; i<inventario->quantidade_faturas; i++){
        if(inventario->faturas[i] != NULL){
            free(inventario->faturas[i]->cliente.nome);
            free(inventario->faturas[i]);
        }
    }
    free(inventario->faturas);  /**< dar free da estrutura */
}


/**
 * @brief free_nomes trata de libertar o nome_cliente e o nome_final.
 * 
 * @param nome_cliente 
 * @param nome_final 
 */
void free_nomes(char *nome_cliente, char *nome_final){
    free(nome_cliente);
    free(nome_final);
}


/**
 * @brief slot_validacoes_f tem agrupado parte da função f comum ao caso 2 e 3.
 * 
 * @param inventario 
 * @param nome_cliente 
 * @param nome_final 
 * @param token 
 * @param nif 
 */
void slot_f(Inventario *inventario, char *nome_cliente,
    char *nome_final, const char *token, int nif){

    if(nome_cliente == NULL || nome_final == NULL){
        free_nomes(nome_cliente, nome_final);
        printf("%s\n", EMEMORIA);
        return;
    }
    nome_final[0] = '\0';
    strcpy(nome_cliente, token);

    /** se o nome for error devolver tudo */
    if(strcmp(nome_cliente, "error") == 0){
        devolucao_cesto(inventario);
        free_nomes(nome_cliente, nome_final);
        return;
    }
    if(!valida_nome_cliente(nome_cliente)){
        printf("%s\n", EINVNOME);
        free_nomes(nome_cliente, nome_final);
        return;
    }

    if (nome_cliente[0] == '"'){    /**< se começa com aspas */
        strcpy(nome_final,nome_cliente + 1);    /**< começa aseguir à aspa */
        /** nome_final resume-se ao inicial sem aspas */
        nome_final[strlen(nome_final) - 1] = '\0';  /**< aspa final apagada */
        free(nome_cliente);
        emite_fatura(inventario,nif,nome_final);
    }
    else{   /**> caso contrário  o nome cliente não precisa de tratamento*/
        free(nome_final);
        emite_fatura(inventario,nif,nome_cliente);
    }
}


/**
 * @brief impressao_produtos_disponiveis trata da impressão de um produto.
 * 
 * @param inventario 
 * @param j 
 */
void impressao_produtos_disponiveis(const Inventario *inventario, int j ){
    printf ("%s %c %.2f %d %d %s\n", inventario->produtos[j].ean, 
    inventario->produtos[j].classe_iva,
    inventario->produtos[j].preco,
    inventario->produtos[j].quantidade_vendida,
    inventario->produtos[j].quantidade_stock,
    inventario->produtos[j].descricao);
}


/**
 * @brief analisa_situacao_iva avalia se efetivamente se recebeu, ou não,
 * um ficheiro com classes e percentagem relativamente à taxa de iva a
 * aplicar. Ou, se devemos considerar as taxas por defeito.
 * 
 * @param inventario 
 * @param argc 
 * @param argv 
 */
void analisa_situacao_iva(Inventario *inventario, int argc,
                        const char * const argv[]){
    char classe_iva;
    int taxa_iva;
        if (argc == 2){ /**< sucesso com o ficheiro */
            /** abrir o ficheiro em modo leitura */
            FILE *fd = fopen(argv[1],"r");
        if (fd == NULL)
            exit (1);   /** erro */
        while(fscanf(fd," %c %d", &classe_iva, &taxa_iva) == 2){
            /** ler linha-a-linha do ficheiro e tratá-lascd p */
            adiciona_taxa_iva(inventario,classe_iva,taxa_iva);
        }
        fclose(fd);   
    }
    else    /**< caso não haja ficheiro */
       adiciona_taxas_by_default(inventario);
}
