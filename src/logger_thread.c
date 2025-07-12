/*
    FILE: logger_thread.c
    DESCRIPTION:
        Implementa a thread que registra os dados da simulação em arquivos CSV.
    AUTHOR: Darlysson Lima
    LAST UPDATE: Julho, 2025
    LICENSE: CC BY-SA
*/

#define _POSIX_C_SOURCE 199309L  // Necessário para a função clock_gettime
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include "monitors.h"  // Para uso de monitores e mutexes
#include "logs.h"      // Para uso do sistema de logs

#define PERIOD_MS 50   // Período da thread de logging em milissegundos

/* Função da thread de logging */
void *logger_thread(void *arg) {
    ArgsLogger *args = (ArgsLogger *)arg;  // Dados passados para a thread

    LOG_DEBUG("Thread de registro iniciada.\n");

    // Abre o arquivo de saída para registro
    FILE *file = fopen("data/saida.csv", "w");
    if (!file) {
        perror("Erro ao abrir data/saida.csv");
        pthread_exit(NULL);  // Finaliza a thread em caso de erro
    }

    // Escreve o cabeçalho do CSV
    fprintf(file, "t,xref,yref,x1,x2,x3,y1,y2,v1,v2,u1,u2\n");

    struct timespec next_activation;
    clock_gettime(CLOCK_MONOTONIC, &next_activation);  // Inicializa o tempo de ativação

    double t = 0.0;          // Tempo inicial
    double dt = PERIOD_MS / 1000.0;  // Intervalo de tempo para a próxima leitura (em segundos)

    while (1) {
        // Verifica se a thread deve ser encerrada
        pthread_mutex_lock(&args->t->mutex);
        if (args->t->encerrar) {
            pthread_mutex_unlock(&args->t->mutex);
            break;  // Encerra a thread se a flag de encerramento estiver ativada
        }
        pthread_mutex_unlock(&args->t->mutex);

        // Leitura dos dados de várias fontes, protegidas por mutexes
        double xref, yref, x1, x2, x3, y1, y2, v1, v2, u1, u2;

        // Leitura dos valores de referência
        pthread_mutex_lock(&args->r->mutex);
        xref = args->r->xref;
        yref = args->r->yref;
        pthread_mutex_unlock(&args->r->mutex);

        // Leitura do estado do robô
        pthread_mutex_lock(&args->e->mutex);
        x1 = args->e->x1;
        x2 = args->e->x2;
        x3 = args->e->x3;
        y1 = args->e->y1;
        y2 = args->e->y2;
        pthread_mutex_unlock(&args->e->mutex);

        // Leitura das velocidades
        pthread_mutex_lock(&args->c->mutex);
        v1 = args->c->v1;
        v2 = args->c->v2;
        pthread_mutex_unlock(&args->c->mutex);

        // Leitura dos comandos de controle
        pthread_mutex_lock(&args->l->mutex);
        u1 = args->l->u1;
        u2 = args->l->u2;
        pthread_mutex_unlock(&args->l->mutex);

        // Grava os dados no arquivo
        fprintf(file, "%.2f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f\n",
                t, xref, yref, x1, x2, x3, y1, y2, v1, v2, u1, u2);

        fflush(file);  // Força a gravação no arquivo
        t += dt;       // Incrementa o tempo

        // Atualiza o tempo da próxima ativação
        next_activation.tv_nsec += PERIOD_MS * 1e6;
        while (next_activation.tv_nsec >= 1e9) {
            next_activation.tv_sec++;
            next_activation.tv_nsec -= 1e9;
        }

        // Espera até o próximo período de ativação
        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &next_activation, NULL);
    }

    fclose(file);  // Fecha o arquivo de saída
    LOG_DEBUG("Logger finalizado.\n");
    pthread_exit(NULL);  // Finaliza a thread
}
