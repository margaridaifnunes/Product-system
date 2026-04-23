/**
 * validacoes.h has the prototypes of every function in terms of
 * necessary validation.
 * @file validacoes.h
 * @author: ist1117809 (Margarida Isabel Farinha Nunes)
 * @date: March 2026
 */

#ifndef VALIDACOES_H
#define VALIDACOES_H
 
#include "tipos.h"

int valida_tamanho_ean (const char *ean);
int valida_dig_verific (const char *ean, int tamanho, char ultimo_dig);
int valida_ean(const char *ean);
int valida_iva(char iva, const Inventario *inventario);
int valida_preco(double preco);
int valida_quantidade(int quantidade);
int valida_descricao(char *descricao);
int trata_asterisco (const char *padrao, const char *ean, int *i, int *j);
int trata_interrogacao (const char *ean, int *i, int *j);
int valida_padrao_wildcard(const char *padrao, const char *ean);
int valida_nome_cliente (const char *nome);
int valida_nif(const char *nif);
int valida_novo_produto(const Inventario *inventario, const char *ean, char iva,
    double preco, int quantidade, char *descricao);

#endif
