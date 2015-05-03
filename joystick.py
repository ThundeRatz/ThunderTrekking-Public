#!/usr/bin/python
import sys
import select

JS_EVENT_BUTTON =	0x01
JS_EVENT_AXIS =		0x02
JS_EVENT_INIT =		0x08

axes = []
buttons = []

def init():
	global axes, buttons
	packet = sys.stdin.readline().split()
	axes = [0.] * int(packet[0])
	buttons = [False] * int(packet[1])

def get():	# ignora JS_EVENT_INIT
	global axes, buttons
	modificados = [[], []]
	while select.select([sys.stdin], [], [], 0.)[0]:
		packet = sys.stdin.readline().split()
		packet[0], packet[1] = int(packet[0]), int(packet[1])
		if bool(packet[0] & JS_EVENT_BUTTON):
			buttons[packet[1]] = True if packet[2] == '1' else False
			modificados[1].append(packet[1])
		else:
			axes[packet[1]] = float(packet[2])
			modificados[0].append(packet[1])
	return modificados

if __name__ == "__main__":
	init()
	while True:
		if select.select([sys.stdin], [], [], 0.)[0]:
			print(get())
			print(axes, buttons)
