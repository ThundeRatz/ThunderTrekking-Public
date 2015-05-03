import numpy as np
#class Config:

# Constantes:

TRANS_DILATION = 0
TRANS_OPEN = 1

TRANS_AND = 0
TRANS_OR = 1

# Configuracoes:

IMAGEM_ESCALA = 2

ORANGE_LOWER = np.array([0, 20, 10],np.uint8)	#claro
#ORANGE_LOWER = np.array([4, 100, 10],np.uint8)
#ORANGE_LOWER = np.array([5, 30, 160],np.uint8)
ORANGE_UPPER = np.array([8, 255, 255],np.uint8)
WHITE_LOWER = np.array([0, 10, 200],np.uint8)	# claro
WHITE_UPPER = np.array([255, 255, 255],np.uint8)
#WHITE_LOWER = np.array([0, 100, 10],np.uint8)
#WHITE_UPPER = np.array([255, 180, 255],np.uint8)

FILTROS_COR = [ [ORANGE_LOWER, ORANGE_UPPER] ,  [WHITE_LOWER, WHITE_UPPER] ]

TRANSFORMACOES = [
[TRANS_OPEN, TRANS_DILATION],
[TRANS_OPEN, TRANS_DILATION]
]

VALORES_TRANSFORMACOES = [
[np.ones((10, 4), "uint8"), np.ones((15, 15), "uint8")],
[np.ones((6, 10), "uint8"), np.ones((8, 8), "uint8")]
]

MOMENTO_TAMANHO = [ [0, TRANS_AND, 1] ]
MOMENTO_TAMANHO_MIN = 200 # 200 se claro e preciso
#MOMENTO_TAMANHO_MIN = 0

MOMENTO_PARADA = [ [0] ]

