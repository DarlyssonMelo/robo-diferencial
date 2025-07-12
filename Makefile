# Variáveis
CC = gcc
LOG_ENABLED ?= 1
CFLAGS = -std=c17 -Wall -Wextra -Iinclude -DLOG_ENABLED=$(LOG_ENABLED)
LDFLAGS = -lm -lpthread
BUILD_DIR = build
SRC_DIR = src
INCLUDE_DIR = include
DATA_DIR = data
LOG_DIR = logs

LOG_TIMESTAMP := $(shell date +%Y-%m-%d_%H-%M-%S)
LOG_FILE := $(LOG_DIR)/log_$(LOG_TIMESTAMP).log

# Gatilho para recompilar ao mudar LOG_ENABLED
REBUILD_FLAG := .rebuild_$(LOG_ENABLED)

# Detecta arquivos
SRCS := $(wildcard $(SRC_DIR)/*.c)
HEADERS := $(wildcard $(INCLUDE_DIR)/*.h)
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

EXEC = main

# Regras principais
all: build

build: $(REBUILD_FLAG) $(BUILD_DIR) $(EXEC)

run: build
	@./$(EXEC) 2> $(LOG_FILE)
	@echo "[INFO] Logs salvos em: $(LOG_FILE)"

plot: run
	@echo "[INFO] Gerando gráfico..."
	@python3 $(SRC_DIR)/plot.py
	@echo "[INFO] Gráfico salvo em data/trajetoria.png"

# Força rebuild se mudar LOG_ENABLED
$(REBUILD_FLAG):
	@echo "[INFO] Alterando modo de log (LOG_ENABLED=$(LOG_ENABLED))"
	rm -f .rebuild_*
	touch $@
	$(MAKE) clean

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
	@echo "[INFO] Binário de execução gerado em: $(EXEC)"
	@echo "[INFO] Para rodar manualmente: ./$(EXEC)"
	@echo "[INFO] Para gerar o gráfico: make plot"

clean:
	rm -rf $(BUILD_DIR)/* .rebuild_* $(DATA_DIR)/* $(EXEC)

.PHONY: all build clean run plot
