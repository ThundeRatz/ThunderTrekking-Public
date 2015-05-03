#!/usr/bin/env python

#sudo apt-get install libopencv-dev python-opencv

import cv2, math
import numpy as np
import time
from ColorConfig import *

# cria a classe de procura por objetos a partir da faixa de cores
class ColourTracker:

	# inicializa
	def __init__(self):
		# comeca a captura de imagens
		self.capture = cv2.VideoCapture(0)

		# diminui a qualidade da imagem para realizar operacoes mais rapidamente
		self.scale_down = IMAGEM_ESCALA
	
	#comeca a deteccao
	def run(self):
		#offset inicial
		camera = []
	
		f, orig_img = self.capture.read()

		# espelha a imagem
		#orig_img = cv2.flip(orig_img, 1)

		# 'borra' a imagem
		#img = cv2.GaussianBlur(orig_img, (5,5), 0)

		# troca o valor dos pixels (em termos de cores) de RGB para HSV
		img = cv2.resize(orig_img, (len(orig_img[0]) / self.scale_down, len(orig_img) / self.scale_down))
		resized = img
		img = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)

		#determina os limites da faixa de cores (H = {0-30} = Amarelo-Vermelho)
		#orange_lower = np.array([1, 150, 0],np.uint8)
		#orange_upper = np.array([6, 255, 255],np.uint8)
		#orange_lower = np.array([3, 140, 100],np.uint8)
		#orange_upper = np.array([6, 255, 255],np.uint8)
		
		# cria uma imagem binaria (intensidade de pixel maxima para o que esta dentro da faixa de cores)
		filtrados = []
		for filtro in FILTROS_COR:
			filtrados.append(cv2.inRange(img, filtro[0], filtro[1]))
		for trans in range(len(filtrados)):
			cv2.imshow("Filtro Cor %d" % trans, filtrados[trans])
		# transformacoes da imagem
		for lista_transformacao in range(len(TRANSFORMACOES)):
			for transformacao in range(len(TRANSFORMACOES[lista_transformacao])):
				trans = TRANSFORMACOES[lista_transformacao][transformacao]
				if trans == TRANS_DILATION:
					filtrados[lista_transformacao] = cv2.dilate(filtrados[lista_transformacao], VALORES_TRANSFORMACOES[lista_transformacao][transformacao])
					cv2.imshow("Transformacao %d %d (dilate)" % (lista_transformacao, transformacao), filtrados[lista_transformacao])
				elif trans == TRANS_OPEN:
					filtrados[lista_transformacao] = cv2.morphologyEx(filtrados[lista_transformacao], cv2.MORPH_OPEN, VALORES_TRANSFORMACOES[lista_transformacao][transformacao])
					cv2.imshow("Transformacao %d %d (open)" % (lista_transformacao, transformacao), filtrados[lista_transformacao])
				else:
					raise(Exception)
		
		momento_tamanho = filtrados[MOMENTO_TAMANHO[0][0]]
		for trans_tamanho in range(len(MOMENTO_TAMANHO)):
			if MOMENTO_TAMANHO[trans_tamanho][-2] == TRANS_AND:
				momento_tamanho = cv2.bitwise_and(momento_tamanho, filtrados[MOMENTO_TAMANHO[trans_tamanho][-1]])
				cv2.imshow("Momento tamanho", cv2.bitwise_or(momento_tamanho, MOMENTO_TAMANHO[trans_tamanho][-1]))
			elif MOMENTO_TAMANHO[trans_tamanho][-2] == TRANS_OR:
				momento_tamanho = cv2.bitwise_or(momento_tamanho, filtrados[MOMENTO_TAMANHO[trans_tamanho][-1]])
			else:
				raise(Exception)
		# encontra os pontos que formam o contorno dos objetos detectados
		contours, hierarchy = cv2.findContours(momento_tamanho, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)

		# serie de comparacoes para encontrar o maior objeto detectado
		max_area = 0
		largest_contour = None
		for idx, contour in enumerate(contours):
			area = cv2.contourArea(contour)
			if area > max_area:
				max_area = area
				largest_contour = contour
		if not largest_contour == None:
			# calcula os momentos da imagem
			moment = cv2.moments(largest_contour)
			camera.append(moment["m00"])
			#if moment["m00"] > MOMENTO_TAMANHO_MIN / self.scale_down:
			if len(MOMENTO_TAMANHO) == 1:
				momento_parada = filtrados[MOMENTO_PARADA[0][0]]
			else:
				for trans_parada in range(len(MOMENTO_PARADA)):
					if len(MOMENTO_PARADA[trans_parada]) == 3:
						momento_parada = filtrados[MOMENTO_PARADA[trans_parada][0]]
					if MOMENTO_PARADA[trans_parada][-2] == TRANS_AND:
						momento_parada = cv2.bitwise_and(momento_parada, filtrados[MOMENTO_PARADA[trans_parada][-1]])
					elif MOMENTO_PARADA[trans_parada][-2] == TRANS_OR:
						momento_parada = cv2.bitwise_or(momento_parada, filtrados[MOMENTO_PARADA[trans_parada][-1]])
					cv2.imshow("Momento parada %d" % trans_parada, momento_parada)
			# menor retangulo para o maior objeto encontrado
			cx=moment["m10"]/moment["m00"];
			camera.append(((int)(cx)*self.scale_down)-320)
			camera.append(cv2.moments(momento_parada)['m00'])
		else:
			camera.append(0)
		if largest_contour != None and moment["m00"] > MOMENTO_TAMANHO_MIN / self.scale_down:
			cy=moment["m01"]/moment["m00"];
			radius = 4
			center = (int(cx),int(cy))
			cv2.circle(resized,center,radius,(0,255,0),2)
		cv2.imshow("ColourTrackerWindow", resized)
		if cv2.waitKey(20) == 27:
			self.capture.release()
		camera.append(1000)
		return camera
	
	def leitura_branco(self):
		f, img = self.sensor2.read()
		cv2.imshow("Branco", img)
		img = cv2.resize(img, (1, 1))
		print img
		img = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
		print img
		cv2.imshow("HSV", img)
		
		#white_lower = np.array([240, 250, 240],np.uint8)
		white_lower = np.array([50, 100, 50],np.uint8)
		white_upper = np.array([255, 255, 255],np.uint8)
		#green_lower = np.array([0, 0, 0],np.uint8)
		#green_upper = np.array([100, 255, 150],np.uint8)
		
		
		img = cv2.inRange(img, white_lower, white_upper)
		cv2.imshow("Branco filtrado", img)
		
		img = cv2.morphologyEx(img, cv2.MORPH_OPEN, np.ones((3, 3), "uint8"))
		cv2.imshow("Branco open", img)
		
		area = cv2.moments(img)['m00']
		print("Sensor branco: ", )
		if cv2.waitKey(20) == 27:
			self.capture.release()
		return area == 0
		
			
if __name__ == "__main__":
	colour_tracker = ColourTracker()
	while True:
		#print colour_tracker.run()
		print colour_tracker.leitura_branco()
		time.sleep(0.1)

