#ifndef SIMU_H
#define SIMU_H

/*
    FILE: simu.h
    DESCRIPTION:
        Cabeçalho que define as estruturas e funções principais
        da simulação do robô diferencial, incluindo controle e
        cálculo das variáveis de saída.
    AUTHOR: Darlysson Lima
    LAST UPDATE: Julho, 2025
    LICENSE: CC BY-SA
*/

#ifndef M_PI
    #define M_PI 3.14159265358979323846  // Define M_PI se não estiver definido
#endif

#include <stdio.h>    // Para operações de entrada e saída
#include <stdlib.h>   // Para alocação de memória e funções utilitárias
#include <pthread.h>  // Para uso de threads
#include <unistd.h>   // Para operações de pausa e controle
#include <math.h>     // Para funções matemáticas como trigonometria e exponenciais
#include "logs.h"     // Para o sistema de logs
#include "matrix.h"   // Para manipulação de matrizes (estado do robô)
#include "dstring.h"  // Para manipulação de strings (se necessário)
#include "integral.h" // Para integração numérica (se necessário)

#define DT_MS 50  // Tempo de amostragem em ms

// Estrutura para dados compartilhados entre threads
typedef struct {
    Matrix *x;          // Estado do robô (posição e orientação)
    double v, w;        // Controle de entrada (velocidade linear e angular)
    double y1, y2;      // Saídas do sistema (posições ou outras variáveis)
    double t;           // Tempo atual da simulação
    int ready_u;        // Sinalizador indicando que os dados de controle estão prontos
    int ready_y;        // Sinalizador indicando que a simulação gerou saída
    int done;           // Sinalizador indicando que o controle foi concluído
    pthread_mutex_t mutex;  // Mutex para sincronização entre threads
    pthread_cond_t cond_u; // Condição de variável para controle de entrada
    pthread_cond_t cond_y; // Condição de variável para controle de saída
} SharedData;

// Função que gera as entradas de controle (v e w) baseadas no tempo
void get_control_input(double t, double *v, double *w, double amplitude, double frequency);

// Função para gravar os resultados de saída (y1, y2) no arquivo
void write_output(FILE *file, double t, double v, double w, double y1, double y2);

// Função que calcula a derivada do estado do robô
void state_derivative(Matrix *x, double v, double w, double dt);

// Função que calcula as saídas do sistema a partir do estado
void calculate_output(Matrix *x, double *y1, double *y2, double D);

// Função principal de simulação do robô
void simulate_robot(const char *output_file, double amplitude, double frequency);

// Função da thread de simulação (realiza o avanço do estado do robô)
void *sim_thread(void *arg);

// Função da thread de controle (calcula as entradas de controle)
void *control_thread(void *arg);

#endif // SIMU_H
