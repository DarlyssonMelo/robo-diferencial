#ifndef MATRIX_H
#define MATRIX_H

/*
    FILE: matrix.h
    DESCRIPTION:
        Cabeçalho com definições e operações para matrizes e vetores.
        Inclui funções como multiplicação, inversão e operações com escalares.
        Usado na linearização e controle do sistema.
    AUTHOR: Darlysson Lima
    LAST UPDATE: Julho, 2025
    LICENSE: CC BY-SA
*/

#include "logs.h"    // Para uso de logs
#include <stdlib.h>  // Para alocação de memória
#include <stdio.h>   // Para entrada e saída de dados
#include <math.h>    // Para operações matemáticas (como cálculo de determinante)

// Definição da estrutura Matrix
typedef struct {
    float **data;   // Ponteiro para os dados da matriz (matriz 2D)
    int rows;       // Número de linhas da matriz
    int cols;       // Número de colunas da matriz
} Matrix;

// ==========================
// Funções de Criação e Destruição
// ==========================

/* Cria uma matriz com o número especificado de linhas e colunas */
Matrix* create_matrix(int rows, int cols);

/* Cria uma matriz preenchida com um valor específico */
Matrix* create_matrix_fill(int rows, int cols, float value);

/* Cria uma matriz de zeros */
Matrix* create_matrix_zeros(int rows, int cols);

/* Cria uma matriz de uns */
Matrix* create_matrix_ones(int rows, int cols);

/* Cria uma matriz a partir de um array unidimensional */
Matrix* create_matrix_from_array(int rows, int cols, float* array);

/* Destrói a matriz e libera a memória alocada */
void destroy_matrix(Matrix* matrix);

// ==========================
// Operações com Matrizes
// ==========================

/* Soma duas matrizes */
Matrix* add_matrices(const Matrix* m1, const Matrix* m2);

/* Subtrai duas matrizes */
Matrix* subtract_matrices(const Matrix* m1, const Matrix* m2);

/* Multiplica duas matrizes */
Matrix* multiply_matrices(const Matrix* m1, const Matrix* m2);

// ==========================
// Operações com Escalar
// ==========================

/* Multiplica uma matriz por um escalar */
Matrix* multiply_by_scalar(const Matrix* matrix, float scalar);

/* Soma um escalar a cada elemento de uma matriz */
Matrix* add_scalar_to_matrix(const Matrix* matrix, float scalar);

/* Subtrai um escalar de cada elemento de uma matriz */
Matrix* subtract_scalar_from_matrix(const Matrix* matrix, float scalar);

// ==========================
// Matrizes Especiais
// ==========================

/* Transposta de uma matriz */
Matrix* transpose_matrix(const Matrix* matrix);

// ==========================
// Funções Auxiliares
// ==========================

/* Calcula o determinante de uma matriz */
float determinant(const Matrix* matrix);

/* Inverte uma matriz */
Matrix* invert_matrix(const Matrix* matrix);

/* Imprime uma matriz */
void print_matrix(const Matrix* matrix);

// ==========================
// Funções de Acesso e Modificação
// ==========================

/* Obtém o valor de um elemento na matriz */
float get_element(const Matrix* matrix, int row, int col);

/* Define o valor de um elemento na matriz */
void set_element(Matrix* matrix, int row, int col, float value);

#endif // MATRIX_H
