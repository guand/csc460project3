#include "bluetooth.h"

    Bluetooth *blue = new Bluetooth("RoombaBluetooth");

    void setup(){
        pinMode(13, OUTPUT);
        Serial.begin(9600);
        blue->setupBluetooth(); 
    }
    
    void loop(){
        delay(500);
        digitalWrite(13, HIGH);
        Serial.println("looping");
        delay(500);
        digitalWrite(13, LOW);
        String msg = blue->Read();
        if(msg.length() > 0){
            Serial.print("Received: ");
            Serial.println(msg);
        }
        if(Serial.available()){

            blue->Send("Example message#");
        }
    }
