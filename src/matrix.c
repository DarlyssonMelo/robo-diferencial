/*
    FILE: matrix.c
    DESCRIPTION:
        Implementa operações matriciais utilizadas na linearização e controle do sistema.
    AUTHOR: Darlysson Lima
    LAST UPDATE: Julho, 2025
    LICENSE: CC BY-SA
*/

#include "matrix.h"

// Função para criar uma matriz
Matrix* create_matrix(int rows, int cols) {
    LOG_DEBUG("create_matrix - Entrada: rows=%d, cols=%d\n", rows, cols);

    if (rows <= 0 || cols <= 0) {
        LOG_ERROR_AND_EXIT("create_matrix - Erro: Dimensões inválidas (rows=%d, cols=%d)\n", rows, cols);
        return NULL;
    }

    // Aloca a matriz e verifica erros
    Matrix* matrix = (Matrix*)malloc(sizeof(Matrix));
    if (!matrix) {
        LOG_ERROR_AND_EXIT("create_matrix - Erro: Falha ao alocar memória para struct Matrix.\n");
    }

    matrix->rows = rows;
    matrix->cols = cols;
    matrix->data = (float**)malloc(rows * sizeof(float*));
    if (!matrix->data) {
        free(matrix);
        LOG_ERROR_AND_EXIT("create_matrix - Erro: Falha ao alocar memória para ponteiros das linhas.\n");
    }

    // Aloca memória para cada linha
    for (int i = 0; i < rows; i++) {
        matrix->data[i] = (float*)malloc(cols * sizeof(float));
        if (!matrix->data[i]) {
            // Libera memória caso falhe
            for (int j = 0; j < i; j++) {
                free(matrix->data[j]);
            }
            free(matrix->data);
            free(matrix);
            LOG_ERROR_AND_EXIT("create_matrix - Erro: Falha ao alocar memória para linha %d.\n", i);
        }
    }

    LOG_DEBUG("create_matrix - Saída: Matriz criada com sucesso (rows=%d, cols=%d)\n", rows, cols);
    return matrix;
}

// Função para criar e preencher a matriz com um valor
Matrix* create_matrix_fill(int rows, int cols, float value) {
    LOG_DEBUG("create_matrix_fill - Preenchendo matriz %dx%d com valor %.2f\n", rows, cols, value);

    Matrix* matrix = create_matrix(rows, cols);
    if (!matrix) {
        LOG_ERROR_AND_EXIT("create_matrix_fill - Erro: Falha ao criar matriz.\n");
        return NULL;
    }

    // Preenche a matriz com o valor especificado
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            matrix->data[i][j] = value;

    LOG_DEBUG("create_matrix_fill - Matriz preenchida com sucesso.\n");
    return matrix;
}

// Função para criar uma matriz de zeros
Matrix* create_matrix_zeros(int rows, int cols) {
    LOG_DEBUG("create_matrix_zeros - Preenchendo matriz %dx%d com valor 0.0\n", rows, cols);
    return create_matrix_fill(rows, cols, 0.0f);
}

// Função para criar uma matriz de uns
Matrix* create_matrix_ones(int rows, int cols) {
    LOG_DEBUG("create_matrix_ones - Preenchendo matriz %dx%d com valor 1.0\n", rows, cols);
    return create_matrix_fill(rows, cols, 1.0f);
}

// Função para criar uma matriz a partir de um vetor
Matrix* create_matrix_from_array(int rows, int cols, float* array) {
    LOG_DEBUG("create_matrix_from_array - Entrada: rows=%d, cols=%d, array=%p\n", rows, cols, (void*)array);

    if (array == NULL) {
        LOG_ERROR_AND_EXIT("create_matrix_from_array - Erro: Ponteiro para array é NULL.\n");
    }

    Matrix* matrix = create_matrix(rows, cols);
    int index = 0;
    // Preenche a matriz a partir do array
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            matrix->data[i][j] = array[index++];

    LOG_DEBUG("create_matrix_from_array - Saída: Matriz criada com sucesso (rows=%d, cols=%d)\n", rows, cols);
    return matrix;
}

// Função para destruir uma matriz
void destroy_matrix(Matrix* matrix) {
    if (matrix == NULL) {
        LOG_DEBUG("destroy_matrix - Entrada: NULL (matriz não existe)\n");
        return;
    }

    LOG_DEBUG("destroy_matrix - Entrada: Deletando matriz (rows=%d, cols=%d)\n", matrix->rows, matrix->cols);

    // Libera a memória das linhas
    for (int i = 0; i < matrix->rows; i++) {
        free(matrix->data[i]);
    }

    // Libera a memória da estrutura da matriz
    free(matrix->data);
    free(matrix);

    LOG_DEBUG("destroy_matrix - Saída: Matriz destruída com sucesso.\n");
}

// Função para somar duas matrizes
Matrix* add_matrices(const Matrix* m1, const Matrix* m2) {
    if (m1 == NULL || m2 == NULL) {
        LOG_ERROR_AND_EXIT("add_matrices - Erro: Uma ou ambas as matrizes são NULL.\n");
    }

    LOG_DEBUG("add_matrices - Entrada: m1(rows=%d, cols=%d), m2(rows=%d, cols=%d)\n",
              m1->rows, m1->cols, m2->rows, m2->cols);

    if (m1->rows != m2->rows || m1->cols != m2->cols) {
        LOG_ERROR_AND_EXIT("add_matrices - Erro: Matrizes com dimensões incompatíveis (m1=%dx%d, m2=%dx%d).\n",
                           m1->rows, m1->cols, m2->rows, m2->cols);
    }

    // Cria a matriz de resultado
    Matrix* result = create_matrix(m1->rows, m1->cols);
    if (result == NULL) {
        LOG_ERROR_AND_EXIT("add_matrices - Erro: Falha ao criar a matriz de resultado.\n");
    }

    // Soma os elementos das matrizes
    for (int i = 0; i < m1->rows; i++) {
        for (int j = 0; j < m1->cols; j++) {
            result->data[i][j] = m1->data[i][j] + m2->data[i][j];
        }
    }

    LOG_DEBUG("add_matrices - Saída: Matrizes somadas com sucesso.\n");
    return result;
}

// Função para subtrair duas matrizes
Matrix* subtract_matrices(const Matrix* m1, const Matrix* m2) {
    if (m1 == NULL || m2 == NULL) {
        LOG_ERROR_AND_EXIT("subtract_matrices - Erro: Uma ou ambas as matrizes são NULL.\n");
    }

    LOG_DEBUG("subtract_matrices - Entrada: m1(rows=%d, cols=%d), m2(rows=%d, cols=%d)\n",
              m1->rows, m1->cols, m2->rows, m2->cols);

    if (m1->rows != m2->rows || m1->cols != m2->cols) {
        LOG_ERROR_AND_EXIT("subtract_matrices - Erro: Matrizes com dimensões incompatíveis (m1=%dx%d, m2=%dx%d).\n",
                           m1->rows, m1->cols, m2->rows, m2->cols);
    }

    Matrix* result = create_matrix(m1->rows, m1->cols);
    if (result == NULL) {
        LOG_ERROR_AND_EXIT("subtract_matrices - Erro: Falha ao criar a matriz de resultado.\n");
    }

    for (int i = 0; i < m1->rows; i++) {
        for (int j = 0; j < m1->cols; j++) {
            result->data[i][j] = m1->data[i][j] - m2->data[i][j];
        }
    }

    LOG_DEBUG("subtract_matrices - Saída: Matrizes subtraídas com sucesso.\n");
    return result;
}

// Função para multiplicar duas matrizes
Matrix* multiply_matrices(const Matrix* m1, const Matrix* m2) {
    if (m1 == NULL || m2 == NULL) {
        LOG_ERROR_AND_EXIT("multiply_matrices - Erro: Uma ou ambas as matrizes são NULL.\n");
    }

    LOG_DEBUG("multiply_matrices - Entrada: m1(rows=%d, cols=%d), m2(rows=%d, cols=%d)\n",
              m1->rows, m1->cols, m2->rows, m2->cols);

    if (m1->cols != m2->rows) {
        LOG_ERROR_AND_EXIT("multiply_matrices - Erro: Matrizes com dimensões incompatíveis (m1.cols=%d, m2.rows=%d).\n",
                           m1->cols, m2->rows);
    }

    Matrix* result = create_matrix(m1->rows, m2->cols);
    if (result == NULL) {
        LOG_ERROR_AND_EXIT("multiply_matrices - Erro: Falha ao criar matriz resultado.\n");
    }

    // Multiplicação das matrizes
    for (int i = 0; i < m1->rows; i++) {
        for (int j = 0; j < m2->cols; j++) {
            result->data[i][j] = 0;
            for (int k = 0; k < m1->cols; k++) {
                result->data[i][j] += m1->data[i][k] * m2->data[k][j];
            }
        }
    }

    LOG_DEBUG("multiply_matrices - Saída: Matrizes multiplicadas com sucesso.\n");
    return result;
}
