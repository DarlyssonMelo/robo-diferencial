/*
    FILE: main.c
    DESCRIPTION:
        Função principal que inicializa todas as threads e recursos do sistema.
    AUTHOR: Darlysson Lima
    LAST UPDATE: Julho, 2025
    LICENSE: CC BY-SA
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "monitors.h"
#include "logger_thread.h"

// Protótipos das threads
void *sim_thread(void *arg);
void *linearization_thread(void *arg);
void *control_thread(void *arg);
void *ref_generator_thread(void *arg);
void *model_ref_x_thread(void *arg);
void *model_ref_y_thread(void *arg);
void *interface_thread(void *arg);
void *logger_thread(void *arg);
void *timer_thread(void *arg);

int main() {
    // Monitores
    MonitorEstado estado;
    MonitorComando comando;
    MonitorLinearizacao linearizacao;
    MonitorReferencia referencia;
    MonitorModeloRef modeloX, modeloY;
    MonitorParametros parametros;
    MonitorTempo tempo;

    // Inicializa mutexes
    pthread_mutex_init(&estado.mutex, NULL);
    pthread_mutex_init(&comando.mutex, NULL);
    pthread_mutex_init(&linearizacao.mutex, NULL);
    pthread_mutex_init(&referencia.mutex, NULL);
    pthread_mutex_init(&modeloX.mutex, NULL);
    pthread_mutex_init(&modeloY.mutex, NULL);
    pthread_mutex_init(&parametros.mutex, NULL);
    pthread_mutex_init(&tempo.mutex, NULL);

    // Inicializa variáveis
    estado.x1 = estado.x2 = estado.x3 = estado.y1 = estado.y2 = 0;
    comando.v1 = comando.v2 = 0;
    linearizacao.u1 = linearizacao.u2 = 0;
    referencia.xref = referencia.yref = 0;
    modeloX.y_m = modeloX.dy_m = 0;
    modeloY.y_m = modeloY.dy_m = 0;
    parametros.alpha1 = parametros.alpha2 = 3;
    tempo.tempo_atual = 0;
    tempo.encerrar = 0;

    // Structs de argumentos
    ArgsSim sim_args         = { &estado, &linearizacao, &tempo };
    ArgsLin lin_args         = { &estado, &comando, &linearizacao, &tempo };
    ArgsCtrl ctrl_args       = { &estado, &modeloX, &modeloY, &parametros, &comando, &tempo };
    ArgsModel modelx_args    = { &referencia, &modeloX, &parametros, &tempo };
    ArgsModel modely_args    = { &referencia, &modeloY, &parametros, &tempo };
    ArgsModel ref_args       = { &referencia, NULL, NULL, &tempo };
    ArgsInterface intf_args  = { &parametros, &estado, &referencia, &tempo };
    ArgsLogger logger_args   = { &estado, &referencia, &comando, &linearizacao, &tempo };

    // Criação das threads
    pthread_t th_sim, th_lin, th_ctrl, th_ref, th_mx, th_my, th_intf, th_log, th_timer;

    pthread_create(&th_sim,   NULL, sim_thread,         &sim_args);
    pthread_create(&th_lin,   NULL, linearization_thread, &lin_args);
    pthread_create(&th_ctrl,  NULL, control_thread,     &ctrl_args);
    pthread_create(&th_ref,   NULL, ref_generator_thread, &ref_args);
    pthread_create(&th_mx,    NULL, model_ref_x_thread, &modelx_args);
    pthread_create(&th_my,    NULL, model_ref_y_thread, &modely_args);
    pthread_create(&th_intf,  NULL, interface_thread,   &intf_args);
    pthread_create(&th_log,   NULL, logger_thread,      &logger_args);
    pthread_create(&th_timer, NULL, timer_thread,       &tempo);

    // Aguarda todas as threads
    pthread_join(th_sim,   NULL);
    pthread_join(th_lin,   NULL);
    pthread_join(th_ctrl,  NULL);
    pthread_join(th_ref,   NULL);
    pthread_join(th_mx,    NULL);
    pthread_join(th_my,    NULL);
    pthread_join(th_intf,  NULL);
    pthread_join(th_log,   NULL);
    pthread_join(th_timer, NULL);

    printf("Simulação concluída com sucesso.\n");
    return 0;
}
