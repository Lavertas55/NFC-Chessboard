/*
 * PINOUT:
 * 
 * MFRC     MEGA
 * SDA      D45
 * SCK      D44
 * MOSI     D51
 * MISO     D50
 * IRQ      N/A
 * GND      GND
 * RST      D5
 * 3.3V     3.3V
*/

#include <MFRC522.h>
#include <SPI.h>

#define RST_PIN 5
#define SS_PIN 45

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance

MFRC522::MIFARE_Key key;

void setup(){
 Serial.begin(9600);
 while(!Serial);
 SPI.begin();
}
