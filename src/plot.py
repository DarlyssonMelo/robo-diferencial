"""
FILE: plot.py
DESCRIPTION:
    Script Python que gera os gráficos das trajetórias simuladas,
    incluindo y(t), xref(t) e yref(t).
AUTHOR: Darlysson Lima
LAST UPDATE: Julho, 2025
LICENSE: CC BY-SA
"""


import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

def main():
    # Carrega CSV
    df = pd.read_csv('data/saida.csv')

    # Extrai colunas
    tempo = df['t']
    x_real = df['y1']
    y_real = df['y2']
    x_ref = df['xref']
    y_ref = df['yref']
    theta = df['x3']

    # Calcula erro
    erro_x = x_ref - x_real
    erro_y = y_ref - y_real

    # ============= Gráfico 1: Posição X(t) =============
    plt.figure(figsize=(8,5))
    plt.plot(tempo, x_ref, 'r--', label='X de Referência')
    plt.plot(tempo, x_real, 'b-', label='X Real')
    plt.xlabel('Tempo (s)')
    plt.ylabel('X (m)')
    plt.title('Componente X ao longo do tempo')
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    plt.savefig('data/posicao_x.png')
    plt.close()

    # ============= Gráfico 2: Posição Y(t) =============
    plt.figure(figsize=(8,5))
    plt.plot(tempo, y_ref, 'r--', label='Y de Referência')
    plt.plot(tempo, y_real, 'b-', label='Y Real')
    plt.xlabel('Tempo (s)')
    plt.ylabel('Y (m)')
    plt.title('Componente Y ao longo do tempo')
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    plt.savefig('data/posicao_y.png')
    plt.close()

    # ============= Gráfico 3: Trajetória XY =============
    plt.figure(figsize=(8,6))
    plt.plot(x_ref, y_ref, 'r--', label='Trajetória de Referência')
    plt.plot(x_real, y_real, 'b-', label='Trajetória Real')
    plt.xlabel('X (m)')
    plt.ylabel('Y (m)')
    plt.title('Trajetória no Plano XY')
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    plt.savefig('data/trajetoria_xy.png')
    plt.close()

    # ============= Gráfico 4: Erro de rastreamento =============
    plt.figure(figsize=(8,5))
    plt.plot(tempo, erro_x, 'g-', label='Erro em X')
    plt.plot(tempo, erro_y, 'm-', label='Erro em Y')
    plt.xlabel('Tempo (s)')
    plt.ylabel('Erro (m)')
    plt.title('Erro de Rastreamento ao longo do tempo')
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    plt.savefig('data/erro_rastreamento.png')
    plt.close()

    # ============= Gráfico 5: Ângulo da frente (theta) =============
    plt.figure(figsize=(8,5))
    plt.plot(tempo, theta, 'c-', label='Ângulo θ(t)')
    plt.xlabel('Tempo (s)')
    plt.ylabel('Ângulo (rad)')
    plt.title('Ângulo da Frente do Robô ao longo do tempo')
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    plt.savefig('data/angulo_theta.png')
    plt.close()

    print("[INFO] Todos os gráficos foram gerados em data/")

if __name__ == "__main__":
    main()
