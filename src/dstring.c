/*
    FILE: dstring.c
    DESCRIPTION:
        Implementa as funções da TAD Dstring (string dinâmica).
    AUTHOR: Darlysson Lima
    LAST UPDATE: Julho, 2025
    LICENSE: CC BY-SA
*/

#include "dstring.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// ==========================
// Funções de criação
// ==========================

/* Cria uma Dstring a partir de uma string C */
Dstring *dstring_new_from_char(const char *str) {
    if (!str) {
        LOG_ERROR_AND_EXIT("str é NULL\n");
        return NULL;
    }

    size_t len = strlen(str);
    Dstring *dstr = malloc(sizeof(Dstring));
    if (!dstr) {
        LOG_ERROR_AND_EXIT("Falha ao alocar Dstring\n");
        return NULL;
    }

    dstr->buffer = malloc(len + 1);
    if (!dstr->buffer) {
        free(dstr);
        LOG_ERROR_AND_EXIT("Falha ao alocar buffer\n");
        return NULL;
    }

    strcpy(dstr->buffer, str);
    dstr->length = len;
    return dstr;
}

/* Cria uma Dstring a partir de um único caractere */
Dstring *dstring_new_from_char_single(char c) {
    char str[2] = {c, '\0'};
    return dstring_new_from_char(str);
}

/* Cria uma Dstring a partir de um inteiro */
Dstring *dstring_new_from_int(int value) {
    char buffer[12];
    snprintf(buffer, sizeof(buffer), "%d", value);
    return dstring_new_from_char(buffer);
}

/* Cria uma Dstring a partir de um valor long */
Dstring *dstring_new_from_long(long value) {
    char buffer[21];
    snprintf(buffer, sizeof(buffer), "%ld", value);
    return dstring_new_from_char(buffer);
}

/* Cria uma Dstring a partir de um float */
Dstring *dstring_new_from_float(float value) {
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%.6f", value);
    return dstring_new_from_char(buffer);
}

/* Cria uma Dstring a partir de um double */
Dstring *dstring_new_from_double(double value) {
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%.6lf", value);
    return dstring_new_from_char(buffer);
}

/* Cria uma Dstring a partir de outra Dstring */
Dstring *dstring_new_from_dstring(const Dstring *src) {
    if (!src) {
        LOG_ERROR_AND_EXIT("src é NULL\n");
        return NULL;
    }
    return dstring_new_from_char(src->buffer);
}

// ==========================
// Funções de manipulação
// ==========================

/* Concatena a Dstring src ao final de dest */
void dstring_concat(Dstring *dest, const Dstring *src) {
    if (!dest || !src || !src->buffer) {
        LOG_ERROR_AND_EXIT("Argumentos inválidos\n");
        return;
    }

    size_t new_length = dest->length + src->length;
    char *new_buffer = realloc(dest->buffer, new_length + 1);
    if (!new_buffer) {
        LOG_ERROR_AND_EXIT("Falha ao realocar memória\n");
        return;
    }

    strcat(new_buffer, src->buffer);
    dest->buffer = new_buffer;
    dest->length = new_length;
}

/* Retorna o tamanho da Dstring */
size_t dstring_length(const Dstring *dstr) {
    if (!dstr) {
        LOG_ERROR_AND_EXIT("dstr é NULL\n");
        return 0;
    }
    return dstr->length;
}

/* Retorna a string C correspondente à Dstring */
const char *dstring_c_str(const Dstring *dstr) {
    if (!dstr) {
        LOG_ERROR_AND_EXIT("dstr é NULL\n");
        return NULL;
    }
    return dstr->buffer;
}

// ==========================
// Função de liberação
// ==========================

/* Libera a memória da Dstring */
void dstring_free(Dstring *dstr) {
    if (!dstr) {
        LOG_ERROR_AND_EXIT("dstr é NULL\n");
        return;
    }

    free(dstr->buffer);
    free(dstr);
}
