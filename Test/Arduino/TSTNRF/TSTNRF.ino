#include <SPI.h>
#include <RF24.h>

RF24 radio(9, 10);  // CE, CSN

const byte direccion[5] = {0xEA, 0xDD, 0xCC, 0xBB, 0xAA};

void setup() {
  Serial.begin(9600);
  
  Serial.println("\n=== NRF24L01 Diagnostic ===\n");
  
  if (!radio.begin()) {
    Serial.println("ERROR: NRF24L01 not responding!");
    Serial.println("Check wiring: CE->9, CSN->10, MOSI->11, MISO->12, SCK->13");
    Serial.println("Check power: Must be 3.3V (NOT 5V!)");
    radio.printDetails();
    while (1);
  }
  
  Serial.println("Module detected OK!");
  
  // Print all configuration - this tells you if the chip is alive
  
  // If you see all 0x00 or all 0xFF = module is dead/not communicating
  // If you see real values = module is alive
  
  radio.setPALevel(RF24_PA_HIGH);
  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(100);
  radio.setPayloadSize(1);
  radio.openReadingPipe(0, direccion);
  radio.startListening();
  
  Serial.println("\n=== After Configuration ===\n");
  radio.printDetails();
  
  Serial.println("\nWaiting for data...");
}

void loop() {
  if (radio.available()) {
    uint8_t dato;
    radio.read(&dato, 1);
    Serial.print("Received: ");
    Serial.println(dato);
  }
}
