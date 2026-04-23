/**
 * comandos.h has the prototypes of every command's functions needed.
 * @file comandos.h
 * @author: ist1117809 (Margarida Isabel Farinha Nunes)
 * @date: March 2026
 */
#ifndef COMANDOS_H
#define COMANDOS_H
 
#include "tipos.h"

void adiciona_novo_produto(Inventario *inventario, const char *linha_input);
void lista_produtos_disponiveis(const Inventario *inventario,
    const char *linha_input);
void incrementa_quantidade_cesto(Inventario *inventario,
    const char *linha_input);
void resume_faturacao (const Inventario *inventario, const char *linha_input);
void fatura_produtos_cesto(Inventario *inventario, const char *linha_input);
void lista_faturas_sistema(Inventario *inventario, const char *linha_input);
void apaga_registo(Inventario *inventario, const char *linha_input);

void case_2_a(Inventario *inventario, const char *ean);
void case_3_a(Inventario *inventario, const char *ean, int quantidade);
void case_1_f(Inventario *inventario);
void case_2_f(Inventario *inventario, const char *token1);
void case_3_f(Inventario *inventario, const char *token1, const char *token2);
void case_1_c(Inventario *inventario);
void case_2_c(Inventario *inventario, const char *token1);
void case_1_d(Inventario *inventario, int numero);
void case_2_d(Inventario *inventario, const char *ean, int quantidade);

#endif
