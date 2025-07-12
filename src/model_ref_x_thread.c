/*
    FILE: model_ref_x_thread.c
    DESCRIPTION:
        Implementa a thread de simulação do modelo de referência na direção X,
        que calcula o modelo de referência com base na entrada xref(t).
    AUTHOR: Darlysson Lima
    LAST UPDATE: Julho, 2025
    LICENSE: CC BY-SA
*/

#define _POSIX_C_SOURCE 199309L
#include <time.h>
#include <unistd.h>
#include "monitors.h"  // Para acessar dados compartilhados entre threads
#include "logs.h"      // Para log de eventos

#define PERIOD_MS 50  // Período de execução da thread em ms

/* Função da thread de modelo de referência na direção X */
void *model_ref_x_thread(void *arg) {
    ArgsModel *args = (ArgsModel *)arg;

    LOG_DEBUG("Thread modelo de referência X iniciada.\n");

    struct timespec next_activation;
    clock_gettime(CLOCK_MONOTONIC, &next_activation);  // Define o tempo inicial

    double dt = PERIOD_MS / 1000.0;  // Intervalo de tempo em segundos

    while (1) {
        // Verifica se o sistema deve ser encerrado
        pthread_mutex_lock(&args->t->mutex);
        if (args->t->encerrar) {
            pthread_mutex_unlock(&args->t->mutex);
            pthread_exit(NULL);  // Encerra a thread se a flag 'encerrar' for setada
        }
        pthread_mutex_unlock(&args->t->mutex);

        // Leitura da referência de posição xref
        double xref;
        pthread_mutex_lock(&args->r->mutex);
        xref = args->r->xref;
        pthread_mutex_unlock(&args->r->mutex);

        // Leitura do modelo de referência (y_m)
        double ymx;
        pthread_mutex_lock(&args->m->mutex);
        ymx = args->m->y_m;
        pthread_mutex_unlock(&args->m->mutex);

        // Leitura do parâmetro α1
        double alpha1;
        pthread_mutex_lock(&args->p->mutex);
        alpha1 = args->p->alpha1;
        pthread_mutex_unlock(&args->p->mutex);

        // Cálculo da variação do modelo de referência (dymx) e integração
        double dymx = alpha1 * (xref - ymx);  // Derivada do modelo
        ymx += dymx * dt;  // Integração para atualizar o valor de ymx

        // Atualiza o modelo de referência
        pthread_mutex_lock(&args->m->mutex);
        args->m->y_m = ymx;
        args->m->dy_m = dymx;
        pthread_mutex_unlock(&args->m->mutex);

        // Log de depuração com os valores calculados
        LOG_DEBUG("Modelo X: xref=%.2f, ymx=%.2f, dymx=%.2f\n", xref, ymx, dymx);

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
