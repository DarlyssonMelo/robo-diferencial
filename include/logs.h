#ifndef LOGS_H
#define LOGS_H

/*
    FILE: logs.h
    DESCRIPTION:
        Cabeçalho com utilitários de log para diferentes níveis (debug, erro).
        As funções de log podem ser usadas por múltiplos módulos para registro de mensagens.
    AUTHOR: Darlysson Lima
    LAST UPDATE: Julho, 2025
    LICENSE: CC BY-SA
*/

#include <stdio.h>
#include <stdarg.h>

// Controle de ativação do log
#ifndef LOG_ENABLED
#define LOG_ENABLED 1
#endif

#if LOG_ENABLED

// Função interna de log com argumentos variádicos
static inline void log_internal(const char* level,
                                const char* file, const char* function, int line,
                                const char* format, ...) {
    va_list args;
    va_start(args, format);

    // Exibe a mensagem de log no formato: [LEVEL] file:function():line: mensagem
    fprintf(stderr, "[%s] %s:%s():%d: ", level, file, function, line);
    vfprintf(stderr, format, args);  // Formata e exibe a mensagem

    va_end(args);
}

// Macros para log de debug e erro
#define LOG_DEBUG(...) \
    log_internal("DEBUG", __FILE__, __func__, __LINE__, __VA_ARGS__)

#define LOG_ERROR(...) \
    log_internal("ERROR", __FILE__, __func__, __LINE__, __VA_ARGS__)

// Macro de erro fatal, registra a mensagem e finaliza o programa
#define LOG_ERROR_AND_EXIT(...) do { \
    LOG_ERROR(__VA_ARGS__); \
    exit(EXIT_FAILURE); \
} while (0)

#else
// Se log estiver desativado, as macros não fazem nada
#define LOG_DEBUG(...) do {} while (0)
#define LOG_ERROR(...) do {} while (0)
#define LOG_ERROR_AND_EXIT(...) do {} while (0)
#endif

#endif // LOGS_H
