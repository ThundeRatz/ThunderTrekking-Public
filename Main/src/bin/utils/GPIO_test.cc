#include <iostream>
#include <string>

#include "GPIO.hh"

using namespace std;
using namespace Trekking;

int main() {
	int number, direction;
	//GPIO::Direction direction;
	string operation;
	cout << "CUIDADO COM CONFIGURAÇÕES DA GPIO" << endl
		<< "Faça testes na GPIO com nada conectado" << endl
		<< "Número: ";
	cin >> number;

	GPIO gpio(number);
	cout << "Direção (GPIO_IN, GPIO_OUT_LOW, GPIO_OUT_HIGH): ";
	cin >> direction;

	gpio.direction((GPIO::Direction) direction);

	cout << "Operação (poll, edge, set, get): ";
	cin >> operation;

	if (operation == "poll") {
		gpio.poll();
	} else if (operation == "edge") {
		int edge;
		cout << "GPIO_EDGE_NONE, GPIO_EDGE_RISE, GPIO_EDGE_FALL, GPIO_EDGE_BOTH: ";
		cin >> edge;
		gpio.edge((GPIO::Edge) edge);
	} else if (operation == "set") {
		int value;
		cout << "0/1: ";
		cin >> value;
		gpio = value;
	} else if (operation == "get") {
		cout << gpio.get();
	}
}
