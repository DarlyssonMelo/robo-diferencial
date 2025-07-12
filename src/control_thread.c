/*
    FILE: control_thread.c
    DESCRIPTION:
        Implementa a thread responsável pelo cálculo do sinal de controle v(t),
        com base no controlador por modelo de referência (controle em malha fechada).
    AUTHOR: Darlysson Lima
    LAST UPDATE: Julho, 2025
    LICENSE: CC BY-SA
*/

#define _POSIX_C_SOURCE 199309L
#include <time.h>
#include <unistd.h>
#include <math.h>
#include "monitors.h"  // Para acesso às variáveis compartilhadas (mutexes)
#include "logs.h"      // Para registro de logs de depuração

#define PERIOD_MS 50   // Período de execução da thread em ms
#define V_MAX 1.0      // Limite máximo de velocidade linear (v1)
#define W_MAX 1.0      // Limite máximo de velocidade angular (v2)

void *control_thread(void *arg) {
    ArgsCtrl *args = (ArgsCtrl *)arg;

    LOG_DEBUG("Thread de controle iniciada.\n");

    struct timespec next_activation;
    clock_gettime(CLOCK_MONOTONIC, &next_activation);  // Tempo inicial da thread

    while (1) {
        // Verifica o tempo de simulação e se a thread deve ser encerrada
        pthread_mutex_lock(&args->t->mutex);
        if (args->t->encerrar) {
            pthread_mutex_unlock(&args->t->mutex);
            pthread_exit(NULL);  // Encerra a thread quando indicado
        }
        pthread_mutex_unlock(&args->t->mutex);

        // Captura o estado atual do robô (y1, y2)
        double y1, y2;
        pthread_mutex_lock(&args->e->mutex);
        y1 = args->e->y1;
        y2 = args->e->y2;
        pthread_mutex_unlock(&args->e->mutex);

        // Captura o modelo de referência nas direções X e Y
        double ymx, dymx, ymy, dymy;
        pthread_mutex_lock(&args->mx->mutex);
        ymx = args->mx->y_m;
        dymx = args->mx->dy_m;
        pthread_mutex_unlock(&args->mx->mutex);

        pthread_mutex_lock(&args->my->mutex);
        ymy = args->my->y_m;
        dymy = args->my->dy_m;
        pthread_mutex_unlock(&args->my->mutex);

        // Captura os parâmetros α1 e α2
        double alpha1, alpha2;
        pthread_mutex_lock(&args->p->mutex);
        alpha1 = args->p->alpha1;
        alpha2 = args->p->alpha2;
        pthread_mutex_unlock(&args->p->mutex);

        // Calcula o sinal de controle v(t) para as duas direções
        double v1 = dymx + alpha1 * (ymx - y1);  // Velocidade linear para a direção X
        double v2 = dymy + alpha2 * (ymy - y2);  // Velocidade angular para a direção Y

        // Aplicação de saturação para evitar valores de controle excessivos
        if (v1 > V_MAX) v1 = V_MAX;
        if (v1 < -V_MAX) v1 = -V_MAX;
        if (v2 > W_MAX) v2 = W_MAX;
        if (v2 < -W_MAX) v2 = -W_MAX;

        // Atualiza os comandos de controle nas estruturas compartilhadas
        pthread_mutex_lock(&args->c->mutex);
        args->c->v1 = v1;
        args->c->v2 = v2;
        pthread_mutex_unlock(&args->c->mutex);

        // Registra as variáveis de controle e de referência no log
        LOG_DEBUG("Controle atualizado: v=(%.2f, %.2f), ym=(%.2f, %.2f), y=(%.2f, %.2f)\n",
                  v1, v2, ymx, ymy, y1, y2);

        // Espera até o próximo período de ativação
        next_activation.tv_nsec += PERIOD_MS * 1e6;
        while (next_activation.tv_nsec >= 1e9) {
            next_activation.tv_sec++;
            next_activation.tv_nsec -= 1e9;
        }
        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &next_activation, NULL);
    }

    pthread_exit(NULL);
}
