/* 
   This program illustrates the use of photoresistors to control a servo motor.
   
   Create by SM Roux
 */

#include <Servo.h>

int photoOnePin = 0;
int photoTwoPin = 1;
int servoPin = 2;

int photoOneVal, photoTwoVal, photoDiff;
Servo servoMotor;



void setup() {
  Serial.begin(9600);
  servoMotor.attach(servoPin);
  servoMotor.write(90);  
}

void loop() {
  // Get photoresistor values
  photoOneVal = analogRead(photoOnePin);
  photoTwoVal = analogRead(photoTwoPin);
  photoDiff = photoOneVal - photoTwoVal;
  Serial.print(photoOneVal); Serial.print(", "); Serial.print(photoTwoVal); Serial.print(", "); Serial.println(photoDiff);

  // Move servo motor
  if (photoDiff > 200) {
    servoMotor.write(0);   
  }
  else if (photoDiff < -200) {
    servoMotor.write(180);
  }
  else {
    servoMotor.write(90);
  }
  delay(2000);
}
