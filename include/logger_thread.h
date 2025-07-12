#ifndef LOGGER_THREAD_H
#define LOGGER_THREAD_H

/*
    FILE: logger_thread.h
    DESCRIPTION:
        Cabeçalho que declara a thread responsável pelo logging dos dados de simulação.
        Inclui interfaces para inicialização e controle do log em tempo de execução.
    AUTHOR: Darlysson Lima
    LAST UPDATE: Julho, 2025
    LICENSE: CC BY-SA
*/

#include "monitors.h"  // Para uso de monitores e sincronização entre threads

/* Declara a função da thread de logging */
void *logger_thread(void *arg);

#endif // LOGGER_THREAD_H
