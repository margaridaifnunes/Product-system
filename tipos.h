/**
 * @file: tipos.h
 * @author: ist1117809 (Margarida Isabel Farinha Nunes)
 * @date: March 2026
 */
#ifndef TIPOS_H
#define TIPOS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * @brief Contantes relevantes
 */
#define BUFMAX			65536	/**< comprimento max de linha de input */
#define MAX_PRODUTOS	10000	/**< nº máximo de produtos registados */
#define MAX_NIF			9		/**< nº máximo de dígitos do nif */
#define MAX_DESC 		50		/**< comprimento máximo da descrição */
#define MAX_EAN			13		/**< comprimento máximo do EAN */
#define EAN_8			8		/**< nº de dígito de EAN-8*/
#define EAN_13			13		/**< nº de dígito de EAN-13 */
#define MAX_TABIVA      26      /**< máximo de entradas da tabela de iva */


/**
 * @brief Mensagens de erro relevantes
 */
#define EMEMORIA		"No memory."		/**< memória esgotada */
#define EINVEAN			"invalid ean"		/**< dígito de verif. errado */
#define EINVIVA     	"invalid iva"		/**< código IVA não existente */      
#define EINVPRECO		"invalid price"		/**< preço não positivo */
#define EINVQUANTIDADE	"invalid quantity"	/**< quantidade negativa */
#define EINVDESCRICAO	"invalid description"	/**< dimensão maior */
#define EPRODEMUSO		"product in use"	/**< produto no cesto */
#define EINVPRODUTO 	"invalid product"	/**< nº excede o max de produtos */
#define EINVNOME        "invalid name"     /**< nome de cliente inválido */

                        /** Estruturas */
/**
 * @brief Estrutra produto com os seguintes campos: descrição, ean, preço,
 * classe_iva, quantidade_vendida e quantidade_stock.
 */
typedef struct produto{
	char descricao[MAX_DESC + 1];	/**< + 1 devido ao '\0' */
	char ean[MAX_EAN + 1];          /**< + 1 devido ao '\0' */
	double preco;
	char classe_iva;
    int quantidade_vendida;
	int quantidade_stock;
}Produto;


/**
 * @brief Estrutura cliente com os seguintes campos: nif e nome.
 */
typedef struct cliente{
    int nif;
	char *nome;
}Cliente;


/**
 * @brief Estrutura fatura com os seguintes campos: custo_total,
 * quantidade_itens_comprados e numero_fatura.
 */
typedef struct fatura{
	double custo_total;
	int quantidade_itens_comprado;
    int numero_fatura;
    Cliente cliente;
}Fatura;


/**
 * @brief Estrutura item_cesto,isto é, cada item individualmenteatravés
 * dos seguintes campos: indice_produto e quantidade_do_produto.
 */
typedef struct item_cesto{
	int indice_produto;
	int quantidade_do_produto;
}Item_cesto;


/**
 * @brief Estrutura entrada_iva com os seguintes campos: letra_maiusc (classe)
 * e taxa_iva (percentagem de taxa a aplicar).
 */
typedef struct entrada_iva{
    char letra_maiusc;
    int taxa_iva;
}Entrada_iva;


/**
 * @brief Estrutura inventario com os seguintes campos: produtos, 
 * quantidade_produtos, tabela_iva com 26 entradas, num_classes_iva_envolvidas,
 * itens_cesto, quantidade_itens_cesto. Relativamente à componente de faturação
 * disponho ainda dos seguintes campos: faturas, quantidade_faturas,
 * valor_total_faturado, total_itens_comprados.
 */
typedef struct inventario{
    Produto produtos[MAX_PRODUTOS];
    int quantidade_produtos;
    Entrada_iva tabela_iva[26];
    int num_classes_iva_envolvidas;
    Item_cesto itens_cesto[MAX_PRODUTOS];
    int quantidade_itens_cesto;

    Fatura **faturas; /**< array dinâmico (s/limite)- ponteiro de ponteiros*/
    int quantidade_faturas;
    double valor_total_faturado;
    int total_itens_comprados;
    int total_faturas_emitidas; /**< só incrementa, valor nunca baixa */
}Inventario;

#endif
