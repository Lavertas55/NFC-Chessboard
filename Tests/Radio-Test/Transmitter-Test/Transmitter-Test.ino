#include <RH_ASK.h>
#include <SPI.h>

RH_ASK driver;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Serial init");
  
  if (!driver.init()) Serial.println("init failed");
}

void loop() {
  // put your main code here, to run repeatedly:
  const char *msg = "open";
  driver.send((uint8_t *)msg, strlen(msg));
  driver.waitPacketSent();
  delay(1000);
}
