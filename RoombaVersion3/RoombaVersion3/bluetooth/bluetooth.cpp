#include "bluetooth.h"
#include "../shared/rtos_n/os.h"

int Bluetooth::rxPin = 17;
int Bluetooth::txPin = 16;
HardwareSerial* Bluetooth::btSerial;

int Bluetooth::getrxPin(){
	return rxPin;
}

void Bluetooth::setrxPin(int rx){
	rxPin = rx;
}

int Bluetooth::gettxPin(){
	return txPin;
}

void Bluetooth::settxPin(int tx){
	txPin = tx;
}

void Bluetooth::setupBluetooth(){
	char msg[20] = "";
	int i = 0;
	
	btSerial = &Serial2;

	btSerial->begin(9600);

	btSerial->write("AT+BAUD4");
	delay(1100);
	while (btSerial->available()) {
		msg[i] = btSerial->read();
		i++;
	}
	
	char ok9600[] = "OK9600";
	if (strcmp(msg, ok9600) != 0) {
		OS_Abort();
	}
	
	memset(msg, 0, strlen(msg));
	i = 0;
	
	btSerial->write(Bluetooth::Name);
	delay(1100);
	while (btSerial->available()) {
		msg[i] = btSerial->read();
		i++;
	}
	
	char oksetname[] = "OKsetname";
	if (strcmp(msg, oksetname) != 0) {
		OS_Abort();
	}
	
	memset(msg, 0, strlen(msg));
	i = 0;

	btSerial->write("AT+PIN1111");
	delay(1100);
	while (btSerial->available()) {
		msg[i] = btSerial->read();
		i++;
	}
	
	char oksetpin[] = "OKsetPIN";
	if (strcmp(msg, oksetpin) != 0) {
		OS_Abort();
	}
}

Bluetooth::Bluetooth(char name[]){
	setName(name);
	setrxPin(2);
	settxPin(3);
}

Bluetooth::Bluetooth(char name[], int r, int t){
	setName(name);
	setrxPin(r);
	settxPin(t);
	setupBluetooth();
}

void Bluetooth::setName(char c[]){
	strcpy(Name, "AT+NAME");
	strcat(Name, c);
}

char * Bluetooth::getName(){
	return &Name[0];
}

String Bluetooth::Read(){
	char c;
	String retorno = "";
	while(1){
		if (btSerial->available()) {
			c = btSerial->read();
			retorno += c;
			if(c == '#')
			break;
			} else {
			break;
		}
	}
	return retorno;
}

void Bluetooth::Send(char c[]){
	btSerial->print(c);
}