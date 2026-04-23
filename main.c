/* iaed26 - ist1117809 - project */
/**
 * This software project is designed for the development of a product
 * invoicing system.
 * @file: project.c
 * @author: ist1117809 (Margarida Isabel Farinha Nunes)
 * @date: March 2026
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "tipos.h"
#include "validacoes.h"
#include "auxiliares.h"
#include "comandos.h"

/**
 * Função main é responsável por inicializar o inventário, avaliar se for dado,
 * ou não, um ficheiro para o iva e ler os comandos.
 */
int main (int argc, const char *const argv[]){
    char buffer[BUFMAX];
    Inventario inventario = {0};
    if (argc > 2)
        exit(1);
    analisa_situacao_iva(&inventario,argc, argv);

    while (fgets(buffer, BUFMAX, stdin)) {  /**< ler a linha toda */
        switch (buffer[0]) {    /**< analisar o 1º caracter */
			case 'q':   liberta_memoria(&inventario);                 return 0;
            case 'p':   adiciona_novo_produto(&inventario, buffer);      break;
            case 'l':   lista_produtos_disponiveis(&inventario, buffer); break;
            case 'a':   incrementa_quantidade_cesto(&inventario, buffer);break;
            case 'r':   resume_faturacao(&inventario, buffer);           break;
            case 'f':   fatura_produtos_cesto(&inventario, buffer);      break;
            case 'c':   lista_faturas_sistema(&inventario, buffer);      break;
            case 'd':   apaga_registo(&inventario, buffer);              break;
        }
    }
    liberta_memoria(&inventario);
    return 0;
}
