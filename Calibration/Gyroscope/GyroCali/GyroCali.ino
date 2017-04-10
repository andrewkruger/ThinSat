const int chipSelect = 10;

#include <SdFat.h>
SdFat sd;
SdFile myFile;
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_L3GD20_U.h>

/* Assign a unique ID to this sensor at the same time */
Adafruit_L3GD20_Unified gyro = Adafruit_L3GD20_Unified(20);



void setup() {

  Serial.begin(9600);
  Serial.println("Gyroscope Test"); Serial.println("");

  gyro.enableAutoRange(true);
  if(!gyro.begin(GYRO_RANGE_2000DPS))
  {
    /* There was a problem detecting the L3GD20 ... check your connections */
    Serial.println("Ooops, no L3GD20 detected ... Check your wiring!");
    while(1);
  }

  // put your setup code here, to run once:
  if (!sd.begin(chipSelect, SPI_FULL_SPEED)) sd.initErrorHalt();

  // open the file for write at end like the Native SD library
  if (!myFile.open("test.txt", O_RDWR | O_CREAT | O_AT_END)) {
    sd.errorHalt("opening test.txt for write failed");
    } else {

  Serial.println("SD Card Initialized");
  // print header
   }

  // close the file:
  myFile.close();

}

void loop() {
  // put your main code here, to run repeatedly:
  sensors_event_t event; 
  gyro.getEvent(&event);

  Serial.println(event.gyro.x,4);
  
  if (!myFile.open("test.txt", O_RDWR | O_CREAT | O_AT_END)) {
    sd.errorHalt("opening test.txt for write failed");
    } else {
    
    myFile.print(event.gyro.x,4);
    myFile.print('\t');
    myFile.print(event.gyro.y,4);
    myFile.print('\t');
    myFile.print(event.gyro.z,4);
    myFile.print('\n');
    
   }

  myFile.close();
  delay(100);



}
