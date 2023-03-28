#include<SoftwareSerial.h>

#define TxD 1
#define RxD 0

SoftwareSerial bluetoothSerial(TxD, RxD);

char c;

void setup() {
  bluetoothSerial.begin(9600);
  Serial.begin(9600);

}

void loop() {
  if(bluetoothSerial.available()){
    c = bluetoothSerial.read();
    Serial.println(c);
}
}
