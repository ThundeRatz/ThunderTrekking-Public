#!/usr/bin/env python

#sudo apt-get install libopencv-dev python-opencv

import cv2, math
import numpy as np
import time

display = 1

# cria a classe de procura por objetos a partir da faixa de cores
class ColourTracker:

	# inicializa
	def __init__(self):
		# comeca a captura de imagens
		self.capture = cv2.VideoCapture(0)

		# diminui a qualidade da imagem para realizar operacoes mais rapidamente
		self.scale_down = 1

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
		img = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)

		#determina os limites da faixa de cores (H = {0-30} = Amarelo-Vermelho)
		orange_lower = np.array([1, 150, 0],np.uint8)
		orange_upper = np.array([6, 255, 255],np.uint8)
		
		# cria uma imagem binaria (intensidade de pixel maxima para o que esta dentro da faixa de cores)
		orange_binary = cv2.inRange(img, orange_lower, orange_upper)

		# operacao de dilatacao da imagem
		dilation = np.ones((15, 15), "uint8")
		orange_binary = cv2.dilate(orange_binary, dilation)

		# encontra os pontos que formam o contorno dos objetos detectados
		contours, hierarchy = cv2.findContours(orange_binary, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)

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
			if moment["m00"] > 600 / self.scale_down:

				# menor retangulo para o maior objeto encontrado
				cx=moment["m10"]/moment["m00"];
				camera.append(((int)(cx)*self.scale_down)-320)

				# abre janela para mostrar o video com o contorno desenhado
				if(display==1):
					cy=moment["m01"]/moment["m00"];
					radius = 4
					center = (int(cx),int(cy))
					cv2.circle(orig_img,center,radius,(0,255,0),2)
					cv2.circle(orange_binary,center,radius,(0,255,0),2)
					cv2.imshow("ColourTrackerWindow", orange_binary)
					cv2.imshow("ColourTrackerWindow2", orig_img)
					if cv2.waitKey(20) == 27:
						cv2.destroyWindow("ColourTrackerWindow")
						cv2.destroyWindow("ColourTrackerWindow2")
						self.capture.release()
			camera.append(1000)
		else:
			camera.append(0)
			camera.append(1000)
		return camera

if __name__ == "__main__":
	colour_tracker = ColourTracker()
	while True:
		print colour_tracker.run()
		#time.sleep(0.1)
