/*
    FILE: sim_thread.c
    DESCRIPTION:
        Implementa a thread de simulação do robô diferencial, integrando o modelo
        e gerando x(t) e y(t) a partir dos comandos de controle e do estado atual.
    AUTHOR: Darlysson Lima
    LAST UPDATE: Julho, 2025
    LICENSE: CC BY-SA
*/

#define _POSIX_C_SOURCE 199309L
#include <time.h>
#include <unistd.h>
#include <math.h>
#include "monitors.h"  // Para acessar dados compartilhados entre threads
#include "logs.h"      // Para log de eventos

#define R 0.3           // Distância do centro geométrico à frente do robô
#define PERIOD_MS 30    // Período de execução da thread em ms

#ifndef M_PI
#define M_PI 3.14159265358979323846  // Define M_PI se não estiver definido
#endif

/* Função da thread de simulação do robô */
void *sim_thread(void *arg) {
    ArgsSim *args = (ArgsSim *)arg;

    LOG_DEBUG("Thread de simulação iniciada.\n");

    struct timespec next_activation;
    clock_gettime(CLOCK_MONOTONIC, &next_activation);  // Define o tempo inicial

    double dt = PERIOD_MS / 1000.0;  // Intervalo de tempo em segundos

    while (1) {
        // Verifica o tempo e se a simulação deve ser encerrada
        pthread_mutex_lock(&args->t->mutex);
        if (args->t->encerrar) {
            pthread_mutex_unlock(&args->t->mutex);
            pthread_exit(NULL);  // Encerra a thread se a flag 'encerrar' for setada
        }
        pthread_mutex_unlock(&args->t->mutex);

        // Captura os comandos de controle u(t)
        double u1, u2;
        pthread_mutex_lock(&args->l->mutex);
        u1 = args->l->u1;
        u2 = args->l->u2;
        pthread_mutex_unlock(&args->l->mutex);

        // Captura o estado atual do robô (x1, x2, x3)
        double x1, x2, x3;
        pthread_mutex_lock(&args->e->mutex);
        x1 = args->e->x1;
        x2 = args->e->x2;
        x3 = args->e->x3;
        pthread_mutex_unlock(&args->e->mutex);

        // Dinâmica do robô: integração por Euler para calcular as derivadas
        double dx1 = cos(x3) * u1;  // Derivada de x1 (velocidade na direção X)
        double dx2 = sin(x3) * u1;  // Derivada de x2 (velocidade na direção Y)
        double dx3 = u2;            // Derivada de x3 (velocidade angular)

        // Atualiza o estado do robô
        x1 += dx1 * dt;
        x2 += dx2 * dt;
        x3 += dx3 * dt;

        // Correção do ângulo: mantém θ ∈ [-π, π]
        while (x3 > M_PI) x3 -= 2 * M_PI;
        while (x3 < -M_PI) x3 += 2 * M_PI;

        // Cálculo da saída do robô: y(t) = x + deslocamento frontal
        double y1 = x1 + R * cos(x3);  // Posição Y do robô
        double y2 = x2 + R * sin(x3);  // Posição X do robô

        // Atualiza o estado no monitor compartilhado
        pthread_mutex_lock(&args->e->mutex);
        args->e->x1 = x1;
        args->e->x2 = x2;
        args->e->x3 = x3;
        args->e->y1 = y1;
        args->e->y2 = y2;
        pthread_mutex_unlock(&args->e->mutex);

        // Registra no log a atualização do estado do robô
        LOG_DEBUG("Simulação: x=(%.2f, %.2f, %.2f), y=(%.2f, %.2f)\n", x1, x2, x3, y1, y2);

        // Dorme até o próximo período de amostragem
        next_activation.tv_nsec += PERIOD_MS * 1e6;
        while (next_activation.tv_nsec >= 1e9) {
            next_activation.tv_sec++;
            next_activation.tv_nsec -= 1e9;
        }
        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &next_activation, NULL);
    }

    pthread_exit(NULL);  // Encerra a thread
}
