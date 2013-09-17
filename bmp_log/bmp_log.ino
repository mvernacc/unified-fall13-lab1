/* Datalogger for BMP085 sensor and SD card
 * Matthew Vernacchia
 * 2013 Aug 27
 *
 * Based on BMP085_output by Filipe Vieira.
 */
#include <Wire.h> // Two-wire interface for pressure sensor.
#include "BMP085.h" // Pressure sensor driver.
#include <SdFat.h> // SD card driver.

/**** SETTABLE PARAMETERS*****************************/
// These are parameters you may want to change for the lab.
/* Define SERIAL_PRINT to have the program print sensor data over serial
so that you can view it on your computer in real time. */
#define SERIAL_PRINT
// Time between samples (milliseconds).
int t_samp = 2000;
// Launch altitude in cm above sea level.
int32_t launch_alt = 6;
/****************************************************/

// The Digital Pressure Sensor object.
BMP085 dps = BMP085();
long Temperature = 0, Pressure = 0, Altitude = 0;

// SD chip select pin.
const uint8_t chipSelect = 10;
// SD file system object.
SdFat sd;
// Create serial stream.
ArduinoOutStream cout(Serial);
// store error strings in flash to save RAM.
#define error(s) sd.errorHalt_P(PSTR(s))
// Data storage file name.
char name[] = "DATA.CSV";

void setup(void) {
  Serial.begin(9600);
  Wire.begin();
  
  // Initialize the digital pressure sensor.
  Serial.print("Initializing digital pressure sensor..."); 
  dps.init(MODE_ULTRA_HIGHRES, launch_alt, (bool)true);
  Serial.print(" done\n");
  
  // Initialize the SD card.
  Serial.print("Initializing the SD card...");
  if (!sd.begin(chipSelect, SPI_HALF_SPEED)) sd.initErrorHalt();
  Serial.print(" done\n");
    
  // Write the first line of the data file.
  ofstream sdout(name, ios::out | ios::app);
  if (!sdout) error("open failed");
  sdout << "Time(ms), Temp(C), Alt(cm), Pressure(Pa)" << endl;
  // Close the data file.
  sdout.close();
  
  delay(5000);
}
 

void loop(void) {
  // Record iteration start time (milliseconds).
  long t_loop_start = millis();
  // Blink the TX LED on when we take a sample.
  TXLED1;
  
  // Read data from the digital pressure sensor.
  dps.getTemperature(&Temperature); 
  dps.getPressure(&Pressure);
  dps.getAltitude(&Altitude);
  
#ifdef SERIAL_PRINT
  // Print the sensor readings to the serial line.
  Serial.print("Temp(deciC):");
  Serial.print(Temperature);
  Serial.print("  Alt(cm):");
  Serial.print(Altitude);
  Serial.print("  Pressure(Pa):");
  Serial.println(Pressure);
#endif //SERIAL_PRINT
  
  // Open the data file
  ofstream sdout(name, ios::out | ios::app);
  if (!sdout) error("open failed");
  // Print the sensor readings to the data file.
  sdout << millis() << "," << Temperature << "," << Altitude << "," << Pressure << endl;
  // Close the data file.
  sdout.close();
  
  // Turn the TX LED off.
  TXLED0;
  //Wait until it is time to take the next sample.
  while( millis() - t_loop_start < t_samp )
  {
    delay(5);
  }
}


