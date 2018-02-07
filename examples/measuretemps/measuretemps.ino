/*
   This program illustrates the use of various temperature measurement devices.
   The devices tested include:

   * 18B20 temperature sensor module (prefix: tsm)
   * MAX31855 thermocouple (prefix:max)
   * ADS1115 4 channel analog to digital converter (prefix: ads)

   Created by SM Roux
*/

// Required libraries
// Generic
#include <SPI.h>
// For 18B20
#include <OneWire.h>
#include <DallasTemperature.h>
// For MAX31855
#include <Adafruit_MAX31855.h>
// For ADS1115
#include <Adafruit_ADS1015.h>

// 18B20 variables
int tsmPin = 2;
OneWire oneWire(tsmPin);
DallasTemperature tsmBoard(&oneWire); //todo: check if necessary to send address
float tsmTemp;
// MAX31855 variables
int maxDO = 3;
int maxCS = 4;
int maxCLK = 5;
Adafruit_MAX31855 maxBoard(maxCLK, maxCS, maxDO);
double maxTemp;
// ADS1115 variables
Adafruit_ADS1115 adsBoard;
float adsTemps[4];
float adsGainVal;

void setup()
{
	Serial.begin(9600);
  // 18B20 init
	tsmBoard.begin();
  // ADS1115 init
  adsBoard.begin();
  adsBoard.setGain(GAIN_SIXTEEN); // Watch out when setting gain that you do not exceed the voltage range
  adsGainVal = 0.0078123;
  
  delay(1000);
}

void loop()
{
	getTSMTemp();
  getMaxTemp();
  getADSTemp(tsmTemp);

	delay(1000);
}

float getTSMTemp()
{
	tsmBoard.requestTemperatures();
	tsmTemp = tsmBoard.getTempCByIndex(0);
  Serial.print("TSM Temp: "); Serial.println(tsmTemp);
	return tsmTemp;
}

double getMaxTemp()
{
  maxTemp = maxBoard.readCelsius();
  Serial.print("MAX Temp: "); Serial.println(maxTemp);
  Serial.println(maxBoard.readInternal());
  return maxTemp;
}

float * getADSTemp(float refTemp)
{
  int sigs[4];
  float volts[4];
  Serial.print("ADS Temps: ");
  for(int i=0;i<4;i++)
  {
    // Read signal
    sigs[i] = adsBoard.readADC_SingleEnded(i);
    // Convert to volts
    volts[i] = sigs[i] * adsGainVal;
    // Convert to temperature with cold-junction correction; simplified
    adsTemps[i] = (volts[i] + refTemp * 0.042) / 0.042;
    Serial.print(adsTemps[i]); Serial.print(", ");
  }
  Serial.println("");
  return adsTemps;
}
