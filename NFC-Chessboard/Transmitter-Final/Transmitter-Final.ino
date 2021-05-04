/*
 * PINOUT
 * 3.3V   3.3V
 * RST    5
 * GND    GND
 * MISO   50
 * MOSI   51
 * SCK    52
 * SDA(SS) - 1    23
 * SDA(SS) - 2    25
 */

#include <SPI.h>
#include <MFRC522.h>
#include <RH_ASK.h>

#define RST_PIN     5
#define SS_1_PIN    23
#define SS_2_PIN    25

#define NR_OF_READERS   2

byte ssPins[] = {SS_1_PIN, SS_2_PIN};

RH_ASK driver;
MFRC522 mfrc522[NR_OF_READERS];

bool bp1 = false;
bool wq = false; 
bool bp1v = false;
bool wqv = false;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  while(!Serial);

  if(!driver.init()) Serial.println("init failed");

  SPI.begin();

  for (uint8_t reader = 0; reader < NR_OF_READERS; reader++) {
    mfrc522[reader].PCD_Init(ssPins[reader], RST_PIN);
    Serial.print(F("Reader "));
    Serial.print(reader);
    Serial.print(F(": "));
    mfrc522[reader].PCD_DumpVersionToSerial();
  }
}

void loop() {
  // put your main code here, to run repeatedly:

  for (uint8_t reader = 0; reader < NR_OF_READERS; reader++) {

    if (mfrc522[reader].PICC_IsNewCardPresent() && mfrc522[reader].PICC_ReadCardSerial()) {
      Serial.print(F("Reader "));
      Serial.print(reader);

      Serial.print(F(": Card UID: "));
      String content = "";

      for (byte i = 0; i < mfrc522[reader].uid.size; i++) {
        Serial.print(mfrc522[reader].uid.uidByte[i] < 0x10 ? " 0" : " ");
        Serial.print(mfrc522[reader].uid.uidByte[i], HEX);
        content.concat(String(mfrc522[reader].uid.uidByte[i] < 0x10 ? " 0" : " "));
        content.concat(String(mfrc522[reader].uid.uidByte[i], HEX));
      }

      Serial.println();
      Serial.print("Message: ");
      content.toUpperCase();

      if (content.substring(1) == "DB B5 B7 1C" && reader == 0) {
        Serial.println("Black Pawn 1 - Present");
        bp1 = true;
      }

      if (content.substring(1) == "9A 3D 67 15" && reader == 1) {
        Serial.println("White Queen - Present");
        wq = true;
      }

      mfrc522[reader].PICC_HaltA();
      mfrc522[reader].PCD_StopCrypto1();
    }
  }

  if (wq && bp1) {
    delay(2000);
   
    for (uint8_t reader = 0; reader < NR_OF_READERS; reader++) {
      
      String content = "";
      
      for (byte i = 0; i < mfrc522[reader].uid.size; i++) {
        Serial.println("Reader: " + reader);
        Serial.print(mfrc522[reader].uid.uidByte[i] < 0x10 ? " 0" : " ");
        Serial.print(mfrc522[reader].uid.uidByte[i], HEX);
        content.concat(String(mfrc522[reader].uid.uidByte[i] < 0x10 ? " 0" : " "));
        content.concat(String(mfrc522[reader].uid.uidByte[i], HEX));
      }

      content.toUpperCase();

      Serial.println();

      if (content.substring(1) == "DB B5 B7 1C" && reader == 0) {
        Serial.println("Black Pawn 1 - Verified");
        bp1v = true;
      }

      if (content.substring(1) == "9A 3D 67 15" && reader == 1) {
        Serial.println("White Queen - Verified");
        wqv = true;
      }

      if (bp1v && wqv) {
        Serial.println("Verified...");

        const char* msg = "open";
        driver.send((uint8_t *)msg, strlen(msg));
        driver.waitPacketSent();
        
        wqv = false;
        bp1v = false;
        bp1 = false;
        wq = false;
      } 
      else if (!(bp1v && wqv) && reader == (NR_OF_READERS - 1)) {
        wqv = false;
        bp1v = false;
        bp1 = false;
        wq = false;

        Serial.println("Verification Failed");
      }

      mfrc522[reader].PICC_HaltA();
      mfrc522[reader].PCD_StopCrypto1();
    }
  }
}
