/*
   A simple example to illustrate the use of a MAX31855 and an ADS1115 module
   to take temperature measurements.

   Since the ADS module measures voltage and not temperature a very simple (and
   inaccurate) cold junction compensation method is also illustrated.
   This is simply to illustrate the basic principle

   The MAX31855 library and tutorial can be found at the following respective
   links:
        https://github.com/adafruit/Adafruit-MAX31855-library
        https://learn.adafruit.com/thermocouple/using-a-thermocouple

   The ADS1X15 library and tutorial can be found at the following respective
   links:
        https://github.com/adafruit/Adafruit_ADS1X15
        https://learn.adafruit.com/adafruit-4-channel-adc-breakouts/programming

    Created by SM Roux
*/

// Import ADC libraries
#include <Wire.h>
#include <Adafruit_ADS1015.h>
// Import thermocouple libraries
#include <SPI.h>
#include <Adafruit_MAX31855.h>

// ADC variables
Adafruit_ADS1115 ads; // ADS1115 object
int vals[4]; // array of bit values measured from the 4 ADS1115 channels
float mVolts[4]; // array of millivolt values converted from vals
float temps[4]; // array of temperature values converted from mVolts
float gain_val; // amplification factor for the ADC

// MAX31855 variables
int clkPin = 5; // digital pin location for the clock line
int csPin = 4; // digital pin location for chip selection
int doPin = 3; // digital pin location for the data line
Adafruit_MAX31855 tc(clkPin, csPin, doPin); // MAX31855 object
float tcTemp; // Thermocouple temperature
float cjTemp; // Cold-junction temperature
float cjVolts; // Equivalent thermocouple voltage of cold-junction

void setup() {
    // Open serial port
    Serial.begin(9600);
    Serial.println("Starting measurements");

    // Open ADS1115 (I2C device)
    /*
       A specific address may be specified with `begin' if using multiple
       ADS boards, see the documentation concerning this
    */
    ads.begin();
    // ADS1115 Default Range: +/- 6.144V (1 bit = 0.1875mV)
    // (Default: 2/3x gain)
    /*
    Data resolution may be improved by specifying the gain.  Be careful when
    setting the gain that you do now exceed the new voltage range or it might
    blow the ADC.
    Default:
        * Gain: 2/3x
        * Range: +/- 6.144 V
        * Resolution: 0.1875 mV/bit
    */
    ads.setGain(GAIN_SIXTEEN);
    gain_val = 0.0078123;
    /*
    For sixteen gain:
        * Gain: 16x
        * Range: +/- 0.256 V
        * Resolution: 0.0078125 mV
    */
    delay(1000);
}

void loop() {
    // Get MAX31855 values
    Serial.println("*** MAX31855 ***");
    Serial.print("Cold Junction Temp: ");
    cjTemp = tc.readInternal(); // Cold-junction temperature on board
    Serial.println(cjTemp);
    Serial.print("TC Temp: ");
    Serial.println(tc.readCelsius());

    // Get ADC values
    Serial.println("*** ADC Values ***");
    Serial.println(ads.getGain());
    for(int i=0;i<4;i++)
    {
        // Read signal
        vals[i] = ads.readADC_SingleEnded(i);
        // Convert to volts
        mVolts[i] = vals[i] * gain_val;
        // Cold junction correction; simplified
        cjVolts = cjTemp * 0.042;
        // Convert to temperature with cold-junction compensation; simplified
        temps[i] = (mVolts[i] + cjVolts) / 0.042;
        // Output
        Serial.print(i);
        Serial.print(": ");
        Serial.print(temps[i]);
        Serial.println(" degC");
    }
    Serial.println("*****");
    delay(2000);
}
