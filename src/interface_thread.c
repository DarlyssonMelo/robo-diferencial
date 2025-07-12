/*
    FILE: interface_thread.c
    DESCRIPTION:
        Implementa a thread da interface com o usuário, exibindo informações 
        sobre o estado atual da simulação e permitindo alterar parâmetros α1 e α2.
    AUTHOR: Darlysson Lima
    LAST UPDATE: Julho, 2025
    LICENSE: CC BY-SA
*/

#include <unistd.h>  // Para usleep (pausa de 1 segundo)
#include <stdio.h>   // Para exibição de informações na tela
#include "monitors.h" // Para acesso aos dados compartilhados entre threads

/* Função da thread da interface com o usuário */
void *interface_thread(void *arg) {
    ArgsInterface *args = (ArgsInterface *)arg;

    while (1) {
        // Verifica se o sistema deve ser encerrado
        pthread_mutex_lock(&args->t->mutex);
        if (args->t->encerrar) {
            pthread_mutex_unlock(&args->t->mutex);
            pthread_exit(NULL);  // Encerra a thread se a flag 'encerrar' for setada
        }
        double t = args->t->tempo_atual;  // Captura o tempo atual
        pthread_mutex_unlock(&args->t->mutex);

        // Leitura do estado atual do robô e das referências
        double x1, x2, x3, y1, y2, xref, yref, a1, a2;

        // Leitura do estado do robô (posição e orientação)
        pthread_mutex_lock(&args->e->mutex);
        x1 = args->e->x1; x2 = args->e->x2; x3 = args->e->x3;
        y1 = args->e->y1; y2 = args->e->y2;
        pthread_mutex_unlock(&args->e->mutex);

        // Leitura das referências de posição
        pthread_mutex_lock(&args->r->mutex);
        xref = args->r->xref; yref = args->r->yref;
        pthread_mutex_unlock(&args->r->mutex);

        // Leitura dos parâmetros de controle (α1, α2)
        pthread_mutex_lock(&args->p->mutex);
        a1 = args->p->alpha1; a2 = args->p->alpha2;
        pthread_mutex_unlock(&args->p->mutex);

        // Exibe as informações da simulação no formato:
        // [tempo] estado_do_robô | referência | parâmetros de controle
        printf("[%.2fs] x=(%.2f, %.2f, %.2f) | y=(%.2f, %.2f) | ref=(%.2f, %.2f) | α=(%.2f, %.2f)\n",
               t, x1, x2, x3, y1, y2, xref, yref, a1, a2);

        // Pausa a thread por 1 segundo
        usleep(1000000); // 1 segundo
    }
}
