/*
    FILE: linearization_thread.c
    DESCRIPTION:
        Implementa a thread de linearização por realimentação do sistema,
        que gera u(t) a partir de x e v (controle por modelo de referência).
    AUTHOR: Darlysson Lima
    LAST UPDATE: Julho, 2025
    LICENSE: CC BY-SA
*/

#define _POSIX_C_SOURCE 199309L
#include <time.h>
#include <unistd.h>
#include <math.h>
#include "monitors.h"  // Para acessar dados compartilhados
#include "logs.h"      // Para log de eventos

#define R 0.3           // Distância do centro geométrico à frente do robô
#define PERIOD_MS 30    // Período de amostragem em ms
#define U1_MAX 1.0      // Saturação máxima para velocidade linear
#define U2_MAX 3.0      // Saturação máxima para velocidade angular

/* Função da thread de linearização */
void *linearization_thread(void *arg) {
    ArgsLin *args = (ArgsLin *)arg;

    LOG_DEBUG("Thread de linearização iniciada.\n");

    struct timespec next_activation;
    clock_gettime(CLOCK_MONOTONIC, &next_activation);  // Define o tempo inicial

    while (1) {
        // Verifica se o sistema deve ser encerrado
        pthread_mutex_lock(&args->t->mutex);
        if (args->t->encerrar) {
            pthread_mutex_unlock(&args->t->mutex);
            pthread_exit(NULL);  // Encerra a thread se a flag 'encerrar' for setada
        }
        pthread_mutex_unlock(&args->t->mutex);

        // Leitura do ângulo de orientação θ
        double theta;
        pthread_mutex_lock(&args->e->mutex);
        theta = args->e->x3;
        pthread_mutex_unlock(&args->e->mutex);

        // Leitura das velocidades de controle (v1 e v2)
        double v1, v2;
        pthread_mutex_lock(&args->c->mutex);
        v1 = args->c->v1;
        v2 = args->c->v2;
        pthread_mutex_unlock(&args->c->mutex);

        // Equações de linearização inversa para calcular u1 e u2
        double u1 = cos(theta) * v1 + sin(theta) * v2;   // Cálculo da velocidade linear
        double u2 = (-sin(theta) * v1 + cos(theta) * v2) / R;  // Cálculo da velocidade angular

        // Saturação para evitar valores excessivos
        if (u1 > U1_MAX) u1 = U1_MAX;
        if (u1 < -U1_MAX) u1 = -U1_MAX;
        if (u2 > U2_MAX) u2 = U2_MAX;
        if (u2 < -U2_MAX) u2 = -U2_MAX;

        // Atualiza os comandos de controle (u1, u2) no monitor compartilhado
        pthread_mutex_lock(&args->l->mutex);
        args->l->u1 = u1;
        args->l->u2 = u2;
        pthread_mutex_unlock(&args->l->mutex);

        // Registra no log os valores calculados de linearização
        LOG_DEBUG("Linearização: theta=%.2f, v=(%.2f, %.2f) → u=(%.2f, %.2f)\n",
                  theta, v1, v2, u1, u2);

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
