
# Robô Diferencial - Simulação e Controle

## Descrição

Este projeto implementa uma simulação de um **robô diferencial** com controle por **modelo de referência**. O sistema inclui threads que realizam a simulação do movimento do robô, o cálculo das referências, e a aplicação de controle para gerar o movimento em tempo real. A simulação é baseada em modelos matemáticos simples de movimento e controle de um robô móvel com acionamento diferencial.

## Estrutura do Projeto

O projeto é organizado da seguinte forma:

- **src/**: Contém os arquivos de código-fonte (.c) do projeto.
- **include/**: Contém os arquivos de cabeçalho (.h) utilizados no projeto.
- **data/**: Diretório onde os dados gerados durante a execução serão armazenados.
- **logs/**: Diretório onde os arquivos de log serão salvos.
- **Makefile**: Arquivo utilizado para compilar, rodar e gerar gráficos da simulação.

## Requisitos

Antes de rodar o projeto, você precisará dos seguintes requisitos instalados:

- **GCC**: O compilador C utilizado.
- **Python 3**: Para gerar gráficos a partir dos dados de simulação.
- **Biblioteca pthread**: Para suporte a multithreading.
- **Biblioteca matemática (math)**: Para funções matemáticas, como seno, cosseno, etc.

## Como Configurar e Executar

### Passo 1: Clone o repositório

Clone este repositório para a sua máquina local:

```bash
git clone https://github.com/seu-usuario/robo-diferencial.git
cd robo-diferencial
```

### Passo 2: Compilação

Para compilar o projeto, basta executar o comando:

```bash
make
```

Este comando compilará o código e gerará um executável chamado **`main`**. O executável será armazenado no diretório **`build/`**.

### Passo 3: Executando a Simulação

Após compilar, você pode rodar a simulação com o comando:

```bash
make run
```

Este comando irá executar o programa, gerar logs e salvar os resultados em **`logs/log_<timestamp>.log`**.

### Passo 4: Gerando o Gráfico

Depois de rodar a simulação, você pode gerar um gráfico com os dados da simulação com o comando:

```bash
make plot
```

Isso irá gerar um gráfico baseado nos dados da simulação e salvar em **`data/trajetoria.png`**.

### Passo 5: Alterando a Configuração de Logs

O **Makefile** permite ativar ou desativar os logs alterando a variável **`LOG_ENABLED`**. Para isso, modifique o **Makefile** ou execute o comando:

```bash
make LOG_ENABLED=0
```

### Passo 6: Limpando os Arquivos Gerados

Para limpar todos os arquivos de compilação e dados gerados, execute:

```bash
make clean
```

Este comando irá remover os arquivos de objeto, logs e o executável gerado.

## Funções Principais

- **Simulação do Robô**: A simulação do robô é realizada por uma thread que integra as equações diferenciais do modelo do robô utilizando o método de Euler.
- **Controle por Modelo de Referência**: O controle é realizado por uma thread que utiliza o modelo de referência para calcular os sinais de controle **`v(t)`** e **`w(t)`**.
- **Geração de Referências**: As referências de movimento **`xref(t)`** e **`yref(t)`** são geradas por uma thread que calcula essas variáveis baseadas em funções senoidais com dependência do tempo.
- **Linearização**: A linearização do sistema é realizada por uma thread que utiliza feedback para gerar o sinal de controle **`u(t)`** a partir do estado do robô e das referências.

## Estrutura do Código

- **src/**: Contém os arquivos `.c` responsáveis pela lógica do controle, simulação, e geração de referências.
- **include/**: Contém os arquivos de cabeçalho `.h`, com definições de funções e estruturas utilizadas no projeto.
- **logs/**: Contém os logs gerados durante a execução, que podem ser úteis para **depuração** e análise dos resultados da simulação.
- **Makefile**: Arquivo utilizado para automatizar o processo de compilação e execução.

## Contribuições

Se você deseja contribuir para este projeto, fique à vontade para **forkar** o repositório e enviar **pull requests**. Sua contribuição será muito bem-vinda!

