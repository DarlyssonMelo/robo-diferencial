/*
    FILE: timer_thread.c
    DESCRIPTION:
        Implementa uma thread auxiliar de temporização e sincronização, 
        atualizando o tempo de simulação e sinalizando o encerramento da simulação.
    AUTHOR: Darlysson Lima
    LAST UPDATE: Julho, 2025
    LICENSE: CC BY-SA
*/

#include <unistd.h>   // Para usleep (pausa em milissegundos)
#include <stdio.h>    // Para exibição de mensagens
#include "monitors.h" // Para acessar dados compartilhados entre threads

#define SIM_TIME_SECONDS 20  // Tempo total de simulação em segundos
#define INTERVAL_MS 100      // Intervalo de atualização do tempo (100 ms)

/* Função da thread de temporização e sincronização */
void *timer_thread(void *arg) {
    MonitorTempo *tempo = (MonitorTempo *)arg;  // Acesso à estrutura de tempo compartilhada

    double t = 0.0;  // Inicializa o tempo de simulação
    while (t <= SIM_TIME_SECONDS) {
        // Atualiza o tempo atual da simulação
        pthread_mutex_lock(&tempo->mutex);
        tempo->tempo_atual = t;
        pthread_mutex_unlock(&tempo->mutex);

        // Pausa a thread por 100 ms antes de atualizar o tempo
        usleep(INTERVAL_MS * 1000);  // Intervalo de 100 ms
        t += INTERVAL_MS / 1000.0;  // Incrementa o tempo
    }

    // Quando o tempo de simulação atingir o limite, sinaliza o encerramento
    pthread_mutex_lock(&tempo->mutex);
    tempo->encerrar = 1;  // Define a flag de encerramento
    pthread_mutex_unlock(&tempo->mutex);

    printf("[INFO] Simulação encerrada após %.2fs\n", t);  // Exibe a mensagem de encerramento
    pthread_exit(NULL);  // Finaliza a thread
}
