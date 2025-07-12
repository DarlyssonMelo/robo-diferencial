#ifndef DSTRING_H
#define DSTRING_H

/*
    FILE: dstring.h
    DESCRIPTION: 
        Define o TAD Dstring, utilizado para manipulação de strings dinâmicas.
        Funções para criar, concatenar, obter tamanho e liberar memória.
    AUTHOR: Darlysson Lima
    LAST UPDATE: Julho, 2025
    LICENSE: CC BY-SA
*/

#include "logs.h"     // Para logging de eventos ou erros
#include <stddef.h>    // Para size_t (tipo de dados para o tamanho da string)

// Estrutura de uma string dinâmica
typedef struct Dstring {
    char *buffer;      // Ponteiro para o conteúdo da string
    size_t length;     // Tamanho da string (sem o '\0')
} Dstring;

// Funções para criação de Dstrings a partir de diferentes tipos de dados
Dstring *dstring_new_from_char(const char *str);        // Cria Dstring de uma string C
Dstring *dstring_new_from_char_single(char c);          // Cria Dstring de um único caractere
Dstring *dstring_new_from_int(int value);               // Cria Dstring de um inteiro
Dstring *dstring_new_from_long(long value);             // Cria Dstring de um long
Dstring *dstring_new_from_float(float value);           // Cria Dstring de um float
Dstring *dstring_new_from_double(double value);         // Cria Dstring de um double
Dstring *dstring_new_from_dstring(const Dstring *src);  // Cria Dstring a partir de outra Dstring

// Funções para manipulação de Dstrings
void dstring_concat(Dstring *dest, const Dstring *src);    // Concatena src a dest
size_t dstring_length(const Dstring *dstr);               // Retorna o tamanho da Dstring
const char *dstring_c_str(const Dstring *dstr);           // Retorna a string C correspondente

// Função para liberar a memória da Dstring
void dstring_free(Dstring *dstr);

#endif // DSTRING_H
