#include <SPI.h>

const int CSN = 8;

void setup() {
  Serial.begin(9600);
  pinMode(53, OUTPUT);
  pinMode(CSN, OUTPUT);
  digitalWrite(CSN, HIGH);
  
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  
  delay(100);
  
  // Read CONFIG register (should be 0x08 or 0x09)
  digitalWrite(CSN, LOW);
  byte status = SPI.transfer(0x00);  // Read CONFIG
  byte config = SPI.transfer(0x00);
  digitalWrite(CSN, HIGH);
  
  Serial.print("Status: ");
  Serial.println(status, HEX);
  Serial.print("CONFIG: ");
  Serial.println(config, HEX);
}

void loop() {}