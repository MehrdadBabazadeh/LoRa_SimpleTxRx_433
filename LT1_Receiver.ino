// LT1 Receiver example to be used in ISM band 433 MHz with initial settings
// User is responsible for respecting the regulations in the region

#include <SPI.h>
#include <LoRa.h>

HardwareSerial RPI_1(PB7, PB6);
HardwareSerial Serial1(USART1);

const int CS_LoRa = PA8;     // Pin 38 - LoRa radio chip select
const int irqPin = PA9;      // change for your board; must be a hardware interrupt pin
const int resetPin = PA10;   // Pin 61 - LoRa radio reset; This is 9 in Library .h file

void setup() {
  RPI_1.begin(115200);
  Serial1.begin(115200);
  while (!Serial);
  
  SPI.begin();

  pinMode(PB5, OUTPUT);  // Green LED
  pinMode(PB8, OUTPUT);  // Red LED
  
  SPI.setSCLK(PA_5);     // Not certain it is necessary given the pins were defined above
  SPI.setMISO(PA_6);     // I tried both syntax PB_3 / PB_5 and  PB3 / PB5
  SPI.setMOSI(PA_7);     // I tried both syntax PB_3 / PB_5 and  PB3 / PB5

  Serial1.println("LoRa Receiver");
  LoRa.setPins(CS_LoRa, resetPin, irqPin);

  if (!LoRa.begin(433E6)) {
    Serial1.println("Starting LoRa failed!");
    while (1);
  }

  // ranges from 6-12,default 7 see API docs. Higher SF, longer range and slower
  LoRa.setSpreadingFactor(7);           
  //  TX Power: 14-20; Default is 17
  LoRa.setTxPower(18); 
  //  10.4E3, 15.6E3, 20.8E3, 31.25E3, 41.7E3, 62.5E3, 125E3, 250E3; ; Default is 125kHz
  LoRa.setSignalBandwidth(125E3);  
}

void loop() {

  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial1.print("LT1(Receiver) gets: '");

    // read packet
    while (LoRa.available()) {
      Serial1.print((char)LoRa.read());
      digitalWrite(PB5, HIGH);
      digitalWrite(PB8, LOW);      
    }

    // print RSSI of packet
    Serial1.print("' with RSSI ");
    Serial1.println(LoRa.packetRssi());
    delay(1000);   
   
    digitalWrite(PB5, LOW);
    digitalWrite(PB8, HIGH); 
  }
}
