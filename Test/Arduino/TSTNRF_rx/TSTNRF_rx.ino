#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
 
//Declaremos los pines CE y el CSN
#define CE_PIN 9
#define CSN_PIN 10
 
//Variable con la dirección del canal que se va a leer
byte direccion[5] ={'c','a','n','a','l'};

//creamos el objeto radio (NRF24L01)
RF24 radio(CE_PIN, CSN_PIN);

//vector para los datos recibidos
byte datos[2];

void setup()
{
 //inicializamos el NRF24L01 
  radio.begin();
  //inicializamos el puerto serie
  Serial.begin(9600); 

   radio.setCRCLength(RF24_CRC_8);
  //Abrimos el canal de Lectura
  radio.setChannel(100);
  radio.setDataRate(RF24_1MBPS);
  radio.openReadingPipe(1, direccion);

    //empezamos a escuchar por el canal
  radio.startListening();
 
}
 
void loop() {
 uint8_t numero_canal;
 //if ( radio.available(&numero_canal) )
 if ( radio.available() )
 {    
     //Leemos los datos y los guardamos en la variable datos[]
     radio.read(datos,sizeof(datos));
     
     //reportamos por el puerto serial los datos recibidos
     Serial.print(datos[0]); 
     Serial.print(datos[1]); 
 }
 else
 {
     Serial.println("No hay datos de radio disponibles");
 }
 delay(1000);
}