#include <SPI.h>
#include "CN0391.h"
#include "Communication.h"

void setup() {
  Serial.begin(9600);
  SPI.begin();
  SPI.setDataMode(SPI_MODE3);
  delay(1000);
  pinMode(10, OUTPUT);
  CN0391_init();
  CN0391_calibration(RTD_CHANNEL);
  CN0391_calibration(TH_CHANNEL);

}

void loop() {
  CN0391_set_data();
  float temp = CN0391_get_temperature(2);
  Serial.println(temp);
  Serial.println("ping");
  delay(2000);
}
