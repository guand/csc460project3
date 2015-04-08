#ifndef bluetooth_h
#define bluetooth_h

#include "Arduino.h"


class Bluetooth {
	private:
	static int rxPin;
	static int txPin;
	char msg[1024];
	void read();
	char Name[256];
	
	public:
	static HardwareSerial* btSerial;
	int getrxPin();
	void setrxPin(int rx);
	int gettxPin();
	void settxPin(int tx);
	HardwareSerial* getBluetoothSerial();
	void setupBluetooth();
	Bluetooth(char name[]);
	Bluetooth(char name[], int r, int t);
	String Read();
	void Send(char c[]);
	char *getName();
	void setName(char c[]);
};

#endif