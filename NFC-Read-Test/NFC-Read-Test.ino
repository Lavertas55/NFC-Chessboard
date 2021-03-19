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

/*
 * CARD UID REFERENCE
 * BP1 = DB B5 B7 1C
 * WQ = 9A 3D 67 15
 */

#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 5
#define SS_PIN 53

#define G_LED_PIN 24
#define R_LED_PIN 22

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance

void setup() {
  // Initialize
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();

  pinMode(G_LED_PIN, OUTPUT);
  pinMode(R_LED_PIN, OUTPUT);
  
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

  Serial.print("UID tag :");
  String content = "";

  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }

  Serial.println();
  Serial.print("Message: ");
  content.toUpperCase();

  if (content.substring(1) == "DB B5 B7 1C") {
    Serial.println("Black Pawn 1 - Present");
    digitalWrite(G_LED_PIN, HIGH);
    Serial.println();
    delay(3000);
    digitalWrite(G_LED_PIN, LOW);
  }

  else if (content.substring(1) == "9A 3D 67 15") {
    Serial.println("White Queen - Present");
    digitalWrite(G_LED_PIN, HIGH);
    Serial.println();
    delay(3000);
    digitalWrite(R_LED_PIN, LOW);
  }

  else {
    Serial.println("Piece Not Recognized");
    digitalWrite(R_LED_PIN, HIGH);
    Serial.println();
    delay (3000); 
    digitalWrite(R_LED_PIN, LOW);
  }

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}
