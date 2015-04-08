#include "bluetooth.h"
#include <string.h>

    int Bluetooth::rxPin = 19;
    int Bluetooth::txPin = 18;
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
    //btSerial = new SoftwareSerial(rxPin, txPin);
    //pinMode(txPin, OUTPUT);
    //pinMode(rxPin, INPUT);
	
	btSerial = &Serial2;
    //pinMode(txPin, OUTPUT);
    //pinMode(rxPin, INPUT);

    btSerial->begin(9600);

    Serial.println("Bluetooth with 9600");
    btSerial->write("AT+BAUD4");
    delay(2100);
    while (btSerial->available()) {
		Serial.write(btSerial->read());
	}

	Serial.println("Setting name");
	Serial.println(Bluetooth::Name);
    btSerial->write(Bluetooth::Name);
    delay(2100);
    while (btSerial->available()) {
		Serial.write(btSerial->read());
	}

    Serial.println("\nAjusting PIN - 6666");
    btSerial->write("AT+PIN7777");
    delay(2100);
    while (btSerial->available()) {
		Serial.write(btSerial->read());
	}

    Serial.println("\nBluetooth version");
    btSerial->write("AT+VERSION");
    delay(2100);
    while (btSerial->available()) 
    Serial.write(btSerial->read());
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