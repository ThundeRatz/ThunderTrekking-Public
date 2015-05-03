alvo = [latitude, longitude] #Coordenadas do alvo, devem ser colocadas manualmente
import time
import math
modoGPS=True #variavel que definira se o robo esta em modo GPS
def set_angle(lla, venu, alvo):
	"""Dada uma lista contendo latitude, longitude e altitude, outra 
	contendo as componentes da velocidade em ENU, e uma última contendo
	as coordenadas do alvo (latitude, longitude), define um angulo pelo 
	qual o robo ira seguir. E definido anteriormente uma tolerancia, 
	que cria uma regiao na qual o robo nao mudara seu trajeto.Isso e 
	necessario para contrabalancear o problema de controle paravajustes 
	pequenos"""
	tolerancia = 5*(math.pi)/180 #em radianos
	angulo_atual = math.atan2(venu[1],venu[0])
	angulo_ideal = math.atan2(lla[1] - alvo[1], lla[0] - alvo[0])
	if not -tolerancia<(angulo_atual-angulo_ideal)<tolerancia:
		#A diferenca e maior que a faixa de tolerancia
		return [angulo_ideal, angulo_ideal-angulo_atual]
	else:
		return "mantenha trajetoria"
def girar(angulo):
	"""Dado um angulo pelo qual o robo tera que girar, em radianos
	(entre -pi e pi), coordena os motores para que o robo gire esse angulo.
	Para isso cria um fator de correcao, que devera ser definido experimentalmente.
	A possibilidade de angulos muito pequenos, em que o controle e 
	problematico, foi eliminada."""
	fator_de_correcao = 1/60 #toma o valor 1/x, sendo x o angulo que o robo varre em 1s
	if angulo>=0:
		#motor 1 varia de 0 a "algum valor" em metade do tempo total,
		#e de volta a 0 no restante do tempo
		motor1.output(0,100,(angulo*fator_de_correcao/2))
		motor1.output(100,0,(angulo*fator_de_correcao/2))
	else:
		#caso o angulo seja negativo, a situacao sera analoga com o motor2
		motor2.output(0,100,(angulo*fator_de_correcao/2))
		motor2.output(100,0,(angulo*fator_de_correcao/2))
while modoGPS:
	"""Loop principal do GPS. Apenas funcionara quando o booleano 
	'modoGPS' for True - assim permite override da camera quando for
	necessario"""
	recebeu pacote:#instancia de pseudocodigo, o que seria melhor usar?
		if packet[0]=='56'
			venu = packet[1]
		if packet[0]=='4a'
			lla = packet[1]
			try:
				angulo_novo, delta = set_angle(lla, venu, alvo)
				"""Tenta dar update no angulo e na diferenca entre o angulo
				ideal e o atual. Caso nos encontremos na faixa de tolerancia,
				o programa entrara no except abaixo"""
				#motor1, motor2 = -255, -255 (freio, necessario testes)
				#time.sleep(n) (tempo ate frenagem)
				motor1, motor2 = 0, 0 #para o robo
				girar(delta)#direciona o robo
				motor1, motor2 = 255, 255#em marcha novamente
			except IOError #Angulo nao foi atualizado
