import math
def angulo_desejado(ll_alvo, ll_atual):
	"""Dados latitude e longitude do alvo e atuais, calcula o angulo em que o robo deve estar para 
	atingir o alvo"""
	dy = ll_alvo[0]-ll_atual[0]
	dx = ll_alvo[1]-ll_atual[1]
	angulo = math.atan2(dx, dy) / math.pi * 180
	return angulo
