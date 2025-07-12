#ifndef INTEGRAL_H
#define INTEGRAL_H

/*
    FILE: integral.h
    DESCRIPTION:
        Cabeçalho com funções auxiliares para integração numérica
        (ex.: regras de soma) utilizadas no controle e simulação do robô.
    AUTHOR: Darlysson Lima
    LAST UPDATE: Julho, 2025
    LICENSE: CC BY-SA
*/

#include "logs.h"     // Para registro de logs de integração
#include <stdlib.h>   // Para alocação de memória
#include <stdio.h>    // Para entrada e saída de dados
#include <math.h>     // Para funções matemáticas como o cálculo de potências e funções trigonométricas

// Tipo de ponteiro para função a ser integrada
typedef double (*function_ptr)(double);

// ==========================
// Funções de integração
// ==========================

/* Regra do ponto médio para integração numérica */
double midpoint_rule(function_ptr f, double a, double b, int n);

/* Regra trapezoidal composta para integração numérica */
double composite_trapezoidal(function_ptr f, double a, double b, int n);

#endif // INTEGRAL_H
