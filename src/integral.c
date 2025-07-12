/*
    FILE: integral.c
    DESCRIPTION:
        Implementa as funções de integração numérica, como a Regra do Ponto Médio e a Regra do Trapézio Composta.
        Essas funções são usadas na simulação do modelo do robô.
    AUTHOR: Darlysson Lima
    LAST UPDATE: Julho, 2025
    LICENSE: CC BY-SA
*/

#include "integral.h"

// ==========================
// Funções de Integração
// ==========================

/* Regra do Ponto Médio para aproximação da integral definida */
double midpoint_rule(function_ptr f, double a, double b, int n) {
    if (f == NULL) {
        LOG_ERROR_AND_EXIT("Midpoint_rule - Erro: Ponteiro para função é NULL.\n");
        return 0.0;
    }
    if (n <= 0) {
        LOG_ERROR_AND_EXIT("Midpoint_rule - Erro: Número de subdivisões inválido: n=%d\n", n);
        return 0.0;
    }
    if (a == b) {
        LOG_ERROR_AND_EXIT("Midpoint_rule - Intervalo nulo: a == b == %lf\n", a);
        return 0.0;
    }

    double delta_x = (b - a) / n;   // Comprimento de cada subintervalo
    double sum = 0.0;

    // Soma dos valores da função no ponto médio de cada subintervalo
    for (int i = 0; i < n; i++) {
        double xi = a + (i + 0.5) * delta_x;
        sum += f(xi);
    }

    double result = sum * delta_x;  // Multiplicação pela largura do subintervalo
    LOG_DEBUG("Midpoint_rule - Integral aproximada: %lf\n", result);
    return result;
}

/* Regra do Trapézio Composta para aproximação da integral definida */
double composite_trapezoidal(function_ptr f, double a, double b, int n) {
    if (f == NULL) {
        LOG_ERROR_AND_EXIT("Composite_trapezoidal - Erro: Ponteiro para função é NULL.\n");
        return 0.0;
    }
    if (n <= 0) {
        LOG_ERROR_AND_EXIT("Composite_trapezoidal - Erro: Número de subdivisões inválido: n=%d\n", n);
        return 0.0;
    }
    if (a == b) {
        LOG_DEBUG("Composite_trapezoidal - Intervalo nulo: a == b == %lf\n", a);
        return 0.0;
    }

    double h = (b - a) / n;  // Comprimento de cada subintervalo
    double sum = f(a) + f(b);  // Adiciona os valores nas extremidades

    // Soma os valores da função multiplicados por 2 para os pontos intermediários
    for (int i = 1; i < n; i++) {
        double xi = a + i * h;
        sum += 2.0 * f(xi);
    }

    double result = (h / 2.0) * sum;  // Multiplica pela largura do subintervalo e divide por 2
    LOG_DEBUG("Composite_trapezoidal - Integral aproximada: %lf\n", result);
    return result;
}
