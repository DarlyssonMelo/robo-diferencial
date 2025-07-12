#ifndef MONITORS_H
#define MONITORS_H

/*
    FILE: monitors.h
    DESCRIPTION:
        Cabeçalho que declara monitores (mutexes) e estruturas de sincronização
        para acesso seguro às variáveis compartilhadas entre threads.
    AUTHOR: Darlysson Lima
    LAST UPDATE: Julho, 2025
    LICENSE: CC BY-SA
*/

#include <pthread.h>  // Para uso de mutexes e sincronização entre threads

// ==========================
// Estruturas de Monitoramento
// ==========================

// Monitor para o estado do robô
typedef struct {
    double x1, x2, x3;  // Posições e orientações do robô
    double y1, y2;
    pthread_mutex_t mutex;  // Mutex para sincronização
} MonitorEstado;

// Monitor para os comandos v(t) (velocidades)
typedef struct {
    double v1, v2;  // Velocidades em duas direções
    pthread_mutex_t mutex;  // Mutex para sincronização
} MonitorComando;

// Monitor para os comandos u(t) (entrada de controle)
typedef struct {
    double u1, u2;  // Comandos de controle
    pthread_mutex_t mutex;  // Mutex para sincronização
} MonitorLinearizacao;

// Monitor para as referências (xref, yref)
typedef struct {
    double xref, yref;  // Referências para as posições
    pthread_mutex_t mutex;  // Mutex para sincronização
} MonitorReferencia;

// Monitor para o modelo de referência (direções X e Y)
typedef struct {
    double y_m, dy_m;  // Saída e derivada do modelo de referência
    pthread_mutex_t mutex;  // Mutex para sincronização
} MonitorModeloRef;

// Monitor para os parâmetros α1 e α2
typedef struct {
    double alpha1, alpha2;  // Parâmetros de controle
    pthread_mutex_t mutex;  // Mutex para sincronização
} MonitorParametros;

// Monitor para o tempo de simulação
typedef struct {
    double tempo_atual;  // Tempo atual da simulação
    int encerrar;  // Flag para indicar se o sistema deve ser encerrado
    pthread_mutex_t mutex;  // Mutex para sincronização
} MonitorTempo;

// ==========================
// Estruturas de Argumentos para Threads
// ==========================

// Argumentos para a thread de simulação
typedef struct {
    MonitorEstado *e;  // Estado do robô
    MonitorLinearizacao *l;  // Comandos de linearização
    MonitorTempo *t;  // Tempo de simulação
} ArgsSim;

// Argumentos para a thread de linearização
typedef struct {
    MonitorEstado *e;  // Estado do robô
    MonitorComando *c;  // Comandos de controle
    MonitorLinearizacao *l;  // Comandos de linearização
    MonitorTempo *t;  // Tempo de simulação
} ArgsLin;

// Argumentos para a thread de controle
typedef struct {
    MonitorEstado *e;  // Estado do robô
    MonitorModeloRef *mx;  // Modelo de referência na direção X
    MonitorModeloRef *my;  // Modelo de referência na direção Y
    MonitorParametros *p;  // Parâmetros de controle
    MonitorComando *c;  // Comandos de controle
    MonitorTempo *t;  // Tempo de simulação
} ArgsCtrl;

// Argumentos para a thread de modelo de referência
typedef struct {
    MonitorReferencia *r;  // Referências
    MonitorModeloRef *m;  // Modelo de referência
    MonitorParametros *p;  // Parâmetros de controle
    MonitorTempo *t;  // Tempo de simulação
} ArgsModel;

// Argumentos para a interface com o usuário
typedef struct {
    MonitorParametros *p;  // Parâmetros de controle
    MonitorEstado *e;  // Estado do robô
    MonitorReferencia *r;  // Referências
    MonitorTempo *t;  // Tempo de simulação
} ArgsInterface;

// Argumentos para a thread de logging
typedef struct {
    MonitorEstado *e;  // Estado do robô
    MonitorReferencia *r;  // Referências
    MonitorComando *c;  // Comandos de controle
    MonitorLinearizacao *l;  // Comandos de linearização
    MonitorTempo *t;  // Tempo de simulação
} ArgsLogger;

#endif // MONITORS_H
