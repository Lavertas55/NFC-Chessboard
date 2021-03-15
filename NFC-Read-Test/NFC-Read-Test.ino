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

#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 5
#define SS_PIN 53

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance

void setup() {
  // Initialize
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println(F("Read code off of nfc tag:"));
}

void loop() {
  // Prepare Key
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

  byte block;
  byte len;
  MFRC522::StatusCode status;

  // Reset loop if no new card is present on reader
  if (! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if (! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  Serial.println(F("**Card Detected:**"));

  mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid));  //dump some details about the card

  //mfrc522.PICC_DumpToSerial(&(mfrc522.uid));       //uncomment this to see all blocks in hex

  Serial.print(F("Name: "));

  byte buffer1[18];
  String code;

  block = 4;
  len = 18;


  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 4, &key, &(mfrc522.uid));

  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Authentication failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  status = mfrc522.MIFARE_Read(block, buffer1, &len);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Reading failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  
  for (uint8_t i = 0; i < 16; i++) {
    if (buffer1[i] != 32) {
      Serial.write(buffer1[i]);
    }
  }
  Serial.print(" ");
  Serial.println(code);
  
  Serial.println(F("\n**End Reading**\n"));

  delay(1000);

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}
