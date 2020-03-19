#include <MFRC522.h>
#include <SPI.h>
#include <EEPROM.h>

#define pinRST 9 // Defines pins for RST, SS connections respectively.
#define pinSS 10

byte readCard[4]; // Array that will hold UID of the RFID card.
int successRead;

MFRC522 mfrc522(pinSS, pinRST); // Creates MFRC522 instance.
MFRC522::MIFARE_Key key; // Creates MIFARE key instance.


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); // Starts the serial connection at 9600 baud rate.
  SPI.begin(); // Initiates SPI connection between RFID module and Arduino.
  mfrc522.PCD_Init(); // Initiates MFRC522 RFID module.

  Serial.println("RFID reading process initiated.");
  Serial.println("Please scan your RFID card to the reader.");

  do {
    successRead = getID();
  }

  while (!successRead);
  for (int i = 0; i < mfrc522.uid.size; i++) {
    EEPROM.write(i, readCard[i] );
  }

  Serial.println("RFID card information is saved to memory.");
  
}

void loop() {
  // put your main code here, to run repeatedly:
  getID();
}

int getID() // Function that will read and print the RFID cards UID.
{
  if (!mfcr522.PICC_IsNewCardPresent()) {
    return;
  }

  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  Serial.print("UID: ");
  for (int i = 0; i < mfrc522.uid.size; i++) {
    readCard[i] = mfrc522.uid.uidByte[i]; // Reads RFID cards UID.
    Serial.print(readCard[i], HEX); // Prints RFID cards UID to the serial monitor.
  }

  mfrc522.PICC_HaltA(); // Stops the reading process.
}
