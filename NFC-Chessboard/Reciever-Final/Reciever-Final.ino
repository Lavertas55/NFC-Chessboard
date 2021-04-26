#include <RH_ASK.h>
#include <SPI.h>
#include <Servo.h>

Servo myservo;

RH_ASK driver;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while(!Serial);

  myservo.attach(5);
  myservo.write(0);

  if (!driver.init()) Serial.println("init failed");
}

void loop() {
  // put your main code here, to run repeatedly:
  uint8_t buf[4];
  uint8_t buflen = sizeof(buf);

  const char* msg = "open";

  if (driver.recv(buf, &buflen)) {
    Serial.println((char*)buf);

    if (strcmp((char*)buf, msg) != 0) {
      Serial.println("Opening...");
      myservo.write(90);
      delay(5000);
      myservo.write(0);
    }
  }
}
