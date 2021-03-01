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
    mfrc522.PCD_Init();

    for (byte 1 = 0; i < 6; i++) {
        key.keyByte[i] = 0xFF;
    }

    Serial.println(F("Initialized"));
    Serial.print(F("Using key (for A and B): "));
    dump_byte_array(key.keyByte, MFRC522::MF_KEY_SIZE);
    Serial.println(F("Waiting for tag..."));
}

void loop() {
    // Reset loop if no new card is present
    if (!mfrc522.PICC_IsNewCardPresent())
        return;
    
    // select one of the cards
    if (!mfrc522.PICC_ReadCardSerial())
        return;

    Serial.print(F("Card UID:"));
    dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
    Serial.println();


    //check for compatibility
    if (    piccType != MFRC522::PICC_TYPE_MIFARE_MINI
        &&  piccType != MFRC522::PICC_TYPE_MIFARE_1K
        &&  piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
        Serial.println(F("ERROR: Not a MIFARE Classic Card."));
        return;
    }

    byte sector = 1;
    byte blockAddr = 4;
    byte dataBlock[] = {
        0x01, 0x02, 0x03, 0x04,
        0x05, 0x06, 0x07, 0x08,
        0x09, 0x0a, 0xff, 0x0b,
        0x0c, 0x0d, 0x0e, 0x0f
    };
    byte trailerBlock = 7;
    MFRC522::StatusCode status;
    byte buffer[18];
    byte size = sizeof(buffer);

    //authenticate using key A
    Serial.println(F("Authenticating using key A..."));
    status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("PCD_Authenticate() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return;
    }

    //read data from block
    Serial.print(F("Reading code from block ")); Serial.print(blockAddr);
    Serial.println(F(" ..."));
    status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(blockAddr, buffer, &size);
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("MIFARE_Read() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
    }    
    Serial.print(F("Code in block ")); Serial.print(blockAddr); Serial.println(F(":"));
    dump_byte_array(buffer, 16); Serial.println();
    Serial.println();

    // Authenticate using key B
    Serial.println(F("Authenticating again using"))
}