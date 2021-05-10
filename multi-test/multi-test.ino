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
#define RETRY_TOLERANCE 5

byte ssPins[] = {SS_1_PIN, SS_2_PIN};
uint8_t control = 0x00;

MFRC522 mfrc522[NR_OF_READERS];

int CardDetected[NR_OF_READERS];


void setup() {
  Serial.begin(9600);
  while(!Serial);

  SPI.begin();

  for (uint8_t reader = 0; reader < NR_OF_READERS; reader++) {
    mfrc522[reader].PCD_Init(ssPins[reader], RST_PIN);
    Serial.print(F("Reader "));
    Serial.print(reader);
    Serial.print(F(": "));
    mfrc522[reader].PCD_DumpVersionToSerial();

    CardDetected[reader] = 0;
  }
}


void checkReader(int reader) {
  bool RetryDetection=false;

  for (int RetryIndex = 0; RetryIndex < RETRY_TOLERANCE; RetryIndex++){
    control = 0;
    for (int i = 0; i < 3; i++) {
      if (!mfrc522[reader].PICC_IsNewCardPresent()) {
      if (mfrc522[reader].PICC_ReadCardSerial()) {
      control |= 0x16;
      }
        if (mfrc522[reader].PICC_ReadCardSerial()) {
          control |= 0x16;    
        }
        control += 0x1;
      }
      control += 0x4;
    }

    if (control == 13 || control == 14) {
      RetryDetection = true;
    }

    delay(50);
  }

  if(RetryDetection) {
    String TestText = "Reader" + String(reader) + ": Card is still there";
    Serial.println(TestText);
  }

  else {
    String TestText = "Reader" + String(reader) + ": Card was removed!";
    Serial.println(TestText);
    CardDetected[reader] = 0;
  }
}

void loop() {

  for (int reader = 0; reader < NR_OF_READERS; reader++) {
    bool newCard = mfrc522[reader].PICC_IsNewCardPresent() || mfrc522[reader].PICC_ReadCardSerial();

    String TestText = "Reader" + String(reader) + " is waiting for tags...";
    //Serial.println(TestText);

    if (newCard && CardDetected[reader] == 0) {
      CardDetected[reader] = 1;
      String TestText = "Reader" + String(reader) + ": new Card detected!";
      Serial.println(TestText);
    }
    else if (CardDetected[reader] == 1) {
      checkReader(reader);
    }

    mfrc522[reader].PICC_HaltA();
    mfrc522[reader].PCD_StopCrypto1();
  }
}
