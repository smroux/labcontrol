/*
   A simple example to illustrate serial communication between the Arduino
   and a Python application using digital and analog pins.

   Created by SM Roux
*/

int pin1 = A0;
int pin2 = 5;
int pin3 = 10;
float data;
String output;
unsigned long time;

void setup() {
	pinMode(pin2, OUTPUT);
	pinMode(pin3, OUTPUT);
	Serial.begin(9600);
}

void loop() {
if(Serial.available()) {
		time = millis();
		output = time;
		output += ";";
		data = analogRead(pin1);
		output += data;
		output += ";";
		data = digitalRead(pin2);
		output += data;
		output += ";";
		data = digitalRead(pin3);
		output += data;
		Serial.println(output);
	}
	delay(1000);
}





