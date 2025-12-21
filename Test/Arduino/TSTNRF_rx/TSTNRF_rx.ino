#include <SPI.h>
#include <RF24.h>

RF24 radio(9, 10);

void setup() {
  pinMode(53, OUTPUT);  // Mega: SS pin must be OUTPUT for SPI master
  Serial.begin(9600);
  while (!Serial);

  radio.begin();

  if (radio.isChipConnected()) {
    Serial.println("NRF24L01 CONNECTED");
  } else {
    Serial.println("NRF24L01 NOT DETECTED");
  }
}

void loop() {}