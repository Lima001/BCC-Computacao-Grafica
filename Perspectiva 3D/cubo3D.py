import pygame
from math import sin, cos, pi

# Declaração de Constantes

# Dimensões Tela
LARGURA = 800
ALTURA = 600

# Cores para renderização
BRANCO = (255, 255, 255)
VERDE = (0, 255, 0)
VERMELHO = (255, 0, 0)
AZUL = (30, 120, 255)
PRETO = (0, 0, 0)
LARANJA = (255, 165, 0)
ROXO = (177, 156, 217)

# Clock
VELOCIDADE = 60

# Definição do Cubo
P1 = [[1], [1], [1], [1]]
P2 = [[-1], [1], [1], [1]]
P3 = [[1], [-1], [1], [1]]
P4 = [[-1], [-1], [1], [1]]

P5 = [[1], [1], [-1], [1]]
P6 = [[-1], [1], [-1], [1]]
P7 = [[1], [-1], [-1], [1]]
P8 = [[-1], [-1], [-1], [1]]

LISTA_PONTOS = [P1, P2, P3, P4, P5, P6, P7, P8]

# Definição pin-hole
FOCAL = 100

# Matrizes
ANG = 0.5*pi/180

R = [
    [cos(ANG), 0, -sin(ANG), 0],
    [0, 1, 0, 0],
    [sin(ANG), 0, cos(ANG), 0],
    [0, 0, 0, 1]
]

DX = 0
DY = 0
DZ = 7

SX = 2
SY = 2
SZ = 2

T = [
    [SX, 0, 0, DX],
    [0, SY, 0, DY],
    [0, 0, SZ, DZ],
    [0, 0, 0, 1]
]


# Multiplicação de matrizes
def mul(M, N):

    resultado = []

    for i in range(len(M)):
        resultado.append([])
        for j in range(len(N[0])):
            resultado[i].append(0)

    for i in range(len(M)):
        for j in range(len(N[0])):
            for k in range(len(N)):
                resultado[i][j] += M[i][k] * N[k][j]

    return resultado


def applyCameraTransformation(p):
    return [FOCAL*p[0][0]/p[2][0], FOCAL*p[1][0]/p[2][0]]


def desenharPonto(p, cor=BRANCO):
    p[0], p[1] = p[0]+LARGURA/2, ALTURA/2-p[1]
    pygame.draw.circle(tela, cor, p, 2)


def desenharLinha(pi, pf, cor=BRANCO):
    pi[0], pi[1] = pi[0]+LARGURA/2, ALTURA/2-pi[1]
    pf[0], pf[1] = pf[0]+LARGURA/2, ALTURA/2-pf[1]

    pygame.draw.line(tela, cor, pi, pf)


pygame.init()
tela = pygame.display.set_mode((LARGURA, ALTURA), vsync=1)
relogio = pygame.time.Clock()

executar = True
while executar:

    tela.fill(PRETO)

    for evento in pygame.event.get():
        if evento.type == pygame.QUIT:
            executar = False

    if not executar:
        break

    for i in range(len(LISTA_PONTOS)):
        LISTA_PONTOS[i] = mul(R, LISTA_PONTOS[i])
        desenharPonto(applyCameraTransformation(mul(T, LISTA_PONTOS[i])))

    desenharLinha(applyCameraTransformation(
        mul(T,LISTA_PONTOS[0])), applyCameraTransformation(mul(T,LISTA_PONTOS[1])), AZUL)
    desenharLinha(applyCameraTransformation(
        mul(T,LISTA_PONTOS[1])), applyCameraTransformation(mul(T,LISTA_PONTOS[3])), AZUL)
    desenharLinha(applyCameraTransformation(
        mul(T,LISTA_PONTOS[3])), applyCameraTransformation(mul(T,LISTA_PONTOS[2])), AZUL)
    desenharLinha(applyCameraTransformation(
        mul(T,LISTA_PONTOS[2])), applyCameraTransformation(mul(T,LISTA_PONTOS[0])), AZUL)

    desenharLinha(applyCameraTransformation(
        mul(T,LISTA_PONTOS[4])), applyCameraTransformation(mul(T,LISTA_PONTOS[5])), AZUL)
    desenharLinha(applyCameraTransformation(
        mul(T,LISTA_PONTOS[5])), applyCameraTransformation(mul(T,LISTA_PONTOS[7])), AZUL)
    desenharLinha(applyCameraTransformation(
        mul(T,LISTA_PONTOS[7])), applyCameraTransformation(mul(T,LISTA_PONTOS[6])), AZUL)
    desenharLinha(applyCameraTransformation(
        mul(T,LISTA_PONTOS[6])), applyCameraTransformation(mul(T,LISTA_PONTOS[4])), AZUL)

    desenharLinha(applyCameraTransformation(
        mul(T,LISTA_PONTOS[0])), applyCameraTransformation(mul(T,LISTA_PONTOS[4])), AZUL)
    desenharLinha(applyCameraTransformation(
        mul(T,LISTA_PONTOS[1])), applyCameraTransformation(mul(T,LISTA_PONTOS[5])), AZUL)
    desenharLinha(applyCameraTransformation(
        mul(T,LISTA_PONTOS[3])), applyCameraTransformation(mul(T,LISTA_PONTOS[7])), AZUL)
    desenharLinha(applyCameraTransformation(
        mul(T,LISTA_PONTOS[2])), applyCameraTransformation(mul(T,LISTA_PONTOS[6])), AZUL)

    relogio.tick(VELOCIDADE)
    pygame.display.update()

pygame.quit()
