#include <RH_ASK.h>
#include <SPI.h>

RH_ASK driver;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while(!Serial);

  if (!driver.init()) Serial.println("init failed");
}

void loop() {
  // put your main code here, to run repeatedly:
  uint8_t buf[4];
  uint8_t buflen = sizeof(buf);

  if (driver.recv(buf, &buflen)) {
    Serial.println((char*)buf);

    if ((char*)buf == "open\0") {
      Serial.println("Opening...");
    }
  }
}
