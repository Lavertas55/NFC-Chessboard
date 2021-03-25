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

#define RST_PIN     5
#define SS_1_PIN    23
#define SS_2_PIN    25

#define NR_OF_READERS   2

byte ssPins[] = {SS_1_PIN, SS_2_PIN};

MFRC522 mfrc522[NR_OF_READERS];

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  while(!Serial);

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
      
    }
  }
}
