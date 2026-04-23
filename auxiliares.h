/**
 * auxiliares.h has all prototypes related to auxiliary functions.
 * @file auxiliares.h
 * @author: ist1117809 (Margarida Isabel Farinha Nunes)
 * @date: March 2026
 */
#ifndef AUXILIARES_H
#define AUXILIARES_H
 
#include "tipos.h"

int procura_produto(const Inventario *inventario, const char *ean_recebido,
    int *encontrado_cesto);
void introduz_produto(Inventario *inventario, const char *ean, char iva,
                    double preco, int quantidade, const char *descricao);
void atualiza_produto(Inventario *inventario, const char *ean, char iva,
                    double preco, int quantidade, const char *descricao,
                    int indice_produto_procurado);
void ordena_cesto_pelo_ean (Inventario *inventario);
int obter_taxa_iva(char classe_iva, Inventario *inventario);
int obter_indice_cesto(const Inventario *inventario, int indice_produtos);
double arredonda(double valor);
void impressao_item_cesto(Inventario *inventario, int indice,
    int indice_cesto);
int obter_indice_produto(const Inventario *inventario, const char *ean);
void adiciona_taxa_iva (Inventario *inventario, char classe_iva, int taxa);
void adiciona_taxas_by_default (Inventario *inventario);
double calcula_custo_fatura(Inventario *inventario);
void devolucao_cesto(Inventario *inventario);
void preenche_fatura(Inventario * inventario, int num_fatura,
                    int nif, char *nome_cliente);
void emite_fatura(Inventario *inventario, int nif, char *nome_cliente);
void liberta_memoria(Inventario *inventario);
void slot_f(Inventario *inventario,char *nome_cliente,
    char *nome_final, const char *token, int nif);
void impressao_produtos_disponiveis(const Inventario *inventario, int j );
void analisa_situacao_iva(Inventario *inventario, int argc, const char * const argv[]);

#endif
