/**
 * validacoes.c includes every single function in terms of
 * necessary validation.
 * @file validacoes.c
 * @author: ist1117809 (Margarida Isabel Farinha Nunes)
 * @date: March 2026
 */
#include "validacoes.h"


/**
 * @brief valida_tamanho_ean garante que o ean tem tamanho 8 ou 13; isto é,
 * um tamanho válido.
 * 
 * @param ean
 * @return int 
 */
int valida_tamanho_ean (const char *ean){
	int tamanho;
	tamanho = strlen(ean);  /**< tamanho do ean */
	if (tamanho == EAN_8 || tamanho == EAN_13)
		return 1;
	return 0;
}


/**
 * @brief valida_dig_verific confirma que o digito de verificação corresponde
 * ao valor obtido pela soma dos elementos de índices pares somado a 3 vezes a
 * soma dos elementos de índice ímpar aplicado à fórmula (10 - (soma % 10))
 * % 10. Para a operação ser válida, este dígito de verificação deve ser igual
 * ao último dígito do código ean.
 * 
 * @param ean 
 * @param tamanho 
 * @param ultimo_dig 
 * @return int 
 */
int valida_dig_verific (const char *ean, int tamanho, char ultimo_dig){
	int soma_par = 0, soma_imp = 0, dig_verif_obt;
	for (int i=0; i < tamanho - 1; i++){
        /** inteiro equivalente ao caracter */
		int digito = ean[i] - '0'; 
		if (i % 2 == 0) /**< índice par */
			soma_par += digito;
		else            /**< índice ímpar */
			soma_imp += digito;
	}

    /** calculo e verificação do último dígito */
	dig_verif_obt = soma_par + 3*soma_imp;
    if ((ultimo_dig - '0') == (10- (dig_verif_obt % 10)) % 10)
		return 1;
	return 0;
}


/**
 * @brief valida_ean confirma o tamanho e garante que o último dígito
 * corresponde ao dígito de verificação.
 * 
 * @param ean 
 * @return int 
 */
int valida_ean(const char *ean){
	char ultimo_dig; 
	int tamanho;
    tamanho = strlen(ean);
	if (!valida_tamanho_ean(ean))
		return 0;
	ultimo_dig = ean[tamanho - 1];	/**< índice começa em 0*/
	if (!valida_dig_verific(ean,tamanho,ultimo_dig))
		return 0;
	return 1;
}


/**
 * @brief valida_iva garante que existe um código IVA com tal designação.
 * 
 * @param iva 
 * @param inventario 
 * @return int 
 */
int valida_iva(char iva, const Inventario *inventario){
    int i;
    /** percorrer as classes e confirmar se é a correta */
    for (i=0; i<(inventario->num_classes_iva_envolvidas); i++){
        if ((inventario->tabela_iva[i].letra_maiusc) == iva){
            return 1;
        }
    }
    return 0;
}


/**
 * @brief valida_preco verifica se o preço é um número não-positivo.
 * 
 * @param preco 
 * @return int 
 */
int valida_preco(double preco){
    if (preco > 0)  /**< garantir que positivo */
        return 1;
    return 0;
}


/**
 * @brief valida_quantidade garante que a quantidade não é negativa.
 * 
 * @param quantidade 
 * @return int 
 */
int valida_quantidade(int quantidade){
    if (quantidade >= 0)
        return 1;
    return 0;
}


/**
 * @brief valida_descricao garante que a descrição começa por uma letra
 * maiúscula (A a Z), mesmo que acentuada e que não exceda o comprimento máximo
 * de 50 bytes.
 * 
 * @param descricao 
 * @return int 
 */
int valida_descricao(char *descricao){
    int tamanho = strlen(descricao);    /**< tamanho em bytes */
    if (tamanho <= MAX_DESC && ((descricao[0] >= 'A' && descricao[0] <= 'Z') ||
        /** e considerar ainda maiúsculas acentuadas */
        ((unsigned char)descricao[0] == 0xC3 &&
         (unsigned char)descricao[1] >= 0x80 &&
         (unsigned char)descricao[1] <= 0x9F)))
        return 1;
    return 0;
}

/**
 * @brief trata_asterisco é uma função auxiliar vital na valida_padrao_wildcard
 * e está encarregue do tratamento dos padrões com '*'.
 * 
 * @param padrao 
 * @param ean 
 * @param i
 * @param j 
 * @return int 
 */
int trata_asterisco (const char *padrao, const char *ean, int *i, int *j){
    int dig_encontrado = 0;
    (*i)++;
    if(padrao[*i] == '\0')
        return 1;

    /** ao encontrar um dígito */
    char digito = padrao[*i]; /**/
    for (; ean[*j] != '\0'; (*j)++)    /**< procurar no código ean */
        if(ean[*j] == digito)   /**< encontramos o dígito */
            dig_encontrado = 1;
    /** chegou ao fim do padrao */
    if (dig_encontrado == 0)    /**< chegou ao fim, sem sucesso */
        return -1;
    (*i)++;
    return 0;
}


/**
 * @brief trata_interrogação é responsável pelo tratamento do wildcard '?'.
 * 
 * @param ean 
 * @param i 
 * @param j 
 * @return int 
 */
int trata_interrogacao (const char *ean, int *i, int *j){
    if (ean[*j] == '\0')    /**< chegou ao fim sem sucesso */
        return 0;
    /** caso contrário */
    (*i)++;
    (*j)++;
    return 1;
}

/**
 * @brief valida_padrao_wildcard verifica que códigos ean válidos,
 * se enquadram no padrão com os wildcards.
 *  * -> uma sequência de caracteres (dígitos 0-9)
 *  ? -> um caracter (um dígito 0-9)
 * 
 * @param padrao 
 * @param ean 
 * @return int 
 */
int valida_padrao_wildcard(const char *padrao, const char *ean){
    int i=0,j=0, tamanho_padrao, tamanho, resultado;
    tamanho = strlen(ean);
    tamanho_padrao = strlen(padrao);

    /**< confirmar logo o 1º e último dígito */
    if ((isdigit(padrao[0]) && (padrao[0] != ean[0])) || 
        (isdigit(padrao[tamanho_padrao -1]) &&
        (padrao[tamanho_padrao -1] != ean[tamanho -1])))
        return 0;

    while (padrao[i] != '\0'){ /**< até ao fim do padrão */
        /** o caracter pode ser dígito, * ou ? */
        if (padrao[i] == '*'){
            resultado = trata_asterisco(padrao,ean,&i,&j);
            if (resultado == 1) /**< sucesso */
                return 1;
            if (resultado == -1)    /**< insucesso */
                return 0;
        }
        else if (padrao[i] == '?'){  /**< avança 1, exceto sendo o último */ 
            if (!trata_interrogacao(ean,&i,&j))
                return 0;
        }
        else{
            if (padrao[i]!= ean[j]) /**< padrao  falha */
                return 0;
            /**< se o padrao coincidir vê o próximo dígito */
            i++;
            j++;
        }
    }
    return (ean[j]=='\0');  /**< só há sucesso se o ean também tiver acabado */   
}   

/**
 * @brief valida_letra_i trata de garantir que a caracter inicial é uma
 * letra que pode ser, ou não, acentuada.
 * 
 * @param nome 
 * @return int 
 */
static int valida_letra_i(const char *nome) {
    return (isalpha((unsigned char)nome[0]) ||
           ((unsigned char)nome[0] == 0xC3 &&
            (unsigned char)nome[1] >= 0x80 &&
            (unsigned char)nome[1] <= 0x9F));
}

/**
 * @brief valida_nome garante que o nome de cliente é valido, isto é,
 * o nome pode ter espaços e '\t', está entre aspas mas se não tiver espaços 
 * pode ter, ou não, aspas. Também verifica que as aspas são somente 
 * delimitadoras, nunca sendo um caracter do prórpio nome, o 1º caracter tem de
 * ser uma letra. O nome de cliente, não tem limite de comprimento.
 * 
 * @param nome 
 * @return int 
 */
int valida_nome_cliente (const char *nome){
    int i, tamanho = strlen(nome),fim;
    /** se começar em aspa tem de começar em aspa */
    if (nome[0]== '"'){ /**< se começa com aspas */
        if (!isalpha(nome[1]))  /**< se o 1º caracter não for letra */
            return 0;
        if (nome[tamanho - 1] != '"')   /**< tem de terminar com aspas */
            return 0;
        fim = tamanho - 2; 
        for(i=1; i <= fim; i++){
            if (nome[i] == '"')
                return 0;
        }
    }
    else{   /**< deve começar por uma letra(acentuada ou não) */
        if (!valida_letra_i(nome))
            return 0;
        fim = tamanho -1;
        for (i=0; i <= fim; i++){
            if (nome[i] == '\t' || nome[i] == ' ')
                return 0;
        }
    }
    return 1; /**< se chegar ao fim é válido */   
}


/**
 * @brief valida_nif garante que o nif é um número inteiro com
 * 9 dígitos.
 * 
 * @param nif 
 * @return int 
 */
int valida_nif(const char *nif){
    int tamanho = strlen(nif), i;
    if (tamanho != MAX_NIF ||nif[0] == '0') /**< não pode começar em zero */
        return 0;
    for (i=0; i<MAX_NIF; i++){
        if (!isdigit(nif[i]))
            return 0;
    }
    return 1;
}


/**
 * @brief valida_novo_produto engloba todas as validações necessárias ao bom
 * funcionamento do comando p.
 * 
 * @param inventario 
 * @param ean 
 * @param iva 
 * @param preco 
 * @param quantidade 
 * @param descricao 
 * @return int 
 */
int valida_novo_produto(const Inventario *inventario, const char *ean,
    char iva, double preco,int quantidade, char *descricao){
    if (!valida_ean(ean)){
        printf("%s\n", EINVEAN);
        return 0;
    }
    if (!valida_iva(iva,inventario)){
        printf("%s\n", EINVIVA);
        return 0;
    }
    if (!valida_preco(preco)){
        printf("%s\n", EINVPRECO);
        return 0;
    }
    if (!valida_quantidade(quantidade)){
        printf("%s\n", EINVQUANTIDADE);
        return 0;
    }
    if (!valida_descricao(descricao)){
        printf("%s\n", EINVDESCRICAO);
        return 0;
    }
    return 1;
}
