/*
    FILE: ref_generator_thread.c
    DESCRIPTION:
        Implementa a thread de geração das referências xref(t) e yref(t).
        As referências são baseadas em funções senoidais com dependência do tempo.
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

#define PERIOD_MS 120  // Período de execução da thread em ms

#ifndef M_PI
#define M_PI 3.14159265358979323846  // Definir M_PI se não estiver definido
#endif

/* Função da thread de geração de referências (xref, yref) */
void *ref_generator_thread(void *arg) {
    ArgsModel *args = (ArgsModel *)arg;
    MonitorReferencia *r = args->r;
    MonitorTempo *t = args->t;

    LOG_DEBUG("Thread de referência iniciada.\n");

    struct timespec next_activation;
    clock_gettime(CLOCK_MONOTONIC, &next_activation);  // Define o tempo inicial

    while (1) {
        // Verifica o tempo atual de simulação
        pthread_mutex_lock(&t->mutex);
        if (t->encerrar) {
            pthread_mutex_unlock(&t->mutex);
            pthread_exit(NULL);  // Encerra a thread se a flag 'encerrar' for setada
        }
        double tempo = t->tempo_atual;
        pthread_mutex_unlock(&t->mutex);

        // Cálculo da referência xref e yref com base no tempo
        double xref = (5.0 / M_PI) * cos(0.2 * M_PI * tempo);
        double yref = (tempo < 10.0) ?
            (5.0 / M_PI) * sin(0.2 * M_PI * tempo) : 
            -(5.0 / M_PI) * sin(0.2 * M_PI * tempo);

        // Atualiza o monitor de referência
        pthread_mutex_lock(&r->mutex);
        r->xref = xref;
        r->yref = yref;
        pthread_mutex_unlock(&r->mutex);

        // Registra no log a atualização das referências
        LOG_DEBUG("Referência atualizada: t=%.2f → xref=%.2f, yref=%.2f\n", tempo, xref, yref);

        // Espera até o próximo instante
        next_activation.tv_nsec += PERIOD_MS * 1e6;
        while (next_activation.tv_nsec >= 1e9) {
            next_activation.tv_sec++;
            next_activation.tv_nsec -= 1e9;
        }
        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &next_activation, NULL);
    }

    pthread_exit(NULL);  // Encerra a thread
}
