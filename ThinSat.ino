
// Libraries
#include <Wire.h>


// Setup SD Card
#include <SdFat.h>
SdFat sd;
SdFile myFile;
const int chipSelect = 10;


// Temperature Sensor BMP180
#include <SFE_BMP180.h>
SFE_BMP180 pressure;


// Adafruit Sensors
#include <Adafruit_Sensor.h>
#include <Adafruit_9DOF.h>
Adafruit_9DOF dof = Adafruit_9DOF();


// Gyroscope L3GD20
#include <Adafruit_L3GD20_U.h>
Adafruit_L3GD20_Unified gyro = Adafruit_L3GD20_Unified(20);
float Gx_scale = 1.;
float Gy_scale = 0.9973;
float Gz_scale = 1.;
float Gx_offset = 0.;
float Gy_offset = 0.1028;
float Gz_offset = 0.;
float Gx, Gy, Gz;


// Accelerometer LSM303 
#include <Adafruit_LSM303_U.h>
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(30301);
float Ax_offset = -0.37898635; 
float Ay_offset = 0.14820653;
float Az_offset = 0.15208379;
float Ax_scale = 10.03228632;
float Ay_scale = 10.30999622;
float Az_scale = 10.28190202;
float Ax, Ay, Az;
float convert_accel = 9.7995; // Gravity at Chicago altitude 


// Magnetometer LSM303
Adafruit_LSM303_Mag_Unified   mag   = Adafruit_LSM303_Mag_Unified(30302);
float Mx_offset = 0.;
float My_offset = 0.;
float Mz_offset = 0.;
float Mx_scale = 1.;
float My_scale = 1.;
float Mz_scale = 1.;
float Mxy_scale = 0.;
float Mxz_scale = 0.;
float Myz_scale = 0.;
float Bx, By, Bz, Mx, My, Mz;
float convert_mag = 1.; //Needed to convert to known unit

void setup() {
  Serial.begin(9600);

  // SD Card
/*  if (!sd.begin(chipSelect, SPI_FULL_SPEED)) sd.initErrorHalt();

  // open the file for write at end like the Native SD library
  if (!myFile.open("test.txt", O_RDWR | O_CREAT | O_AT_END)) {
    sd.errorHalt("opening test.txt for write failed");
    } else {
  Serial.println("SD Card Initialized");
  }
  // close the file:
  myFile.close();
*/

  // Temperature sensor
  if (pressure.begin())
    Serial.println("BMP180 init success");
  else
  {
    // Oops, something went wrong, this is usually a connection problem,
    // see the comments at the top of this sketch for the proper connections.

    Serial.println("BMP180 init fail (disconnected?)\n\n");
    while(1); // Pause forever.
  }


  // Gyroscope
  gyro.enableAutoRange(true);
  if(!gyro.begin(GYRO_RANGE_2000DPS))
  {
    /* There was a problem detecting the L3GD20 ... check your connections */
    Serial.println("Ooops, no L3GD20 detected ... Check your wiring!");
    while(1);
  }


  // Accelerometer
    if(!accel.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println(F("Ooops, no LSM303 detected ... Check your wiring!"));
    while(1);
  }


  // Magnetometer
  if(!mag.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }

}


void loop() {

  // Temperature
  double T;
  char status = pressure.startTemperature();
  if (status != 0)
  {
    delay(status);
    status = pressure.getTemperature(T);
  }


  // Gyroscope
  sensors_event_t gyro_event; 
  gyro.getEvent(&gyro_event);
  Gx = (gyro_event.gyro.x - Gx_offset) / Gx_scale; // rad/s
  Gy = (gyro_event.gyro.y - Gy_offset) / Gy_scale;
  Gz = (gyro_event.gyro.z - Gz_offset) / Gz_scale;

  
  // Accelerometer
  sensors_event_t accel_event; 
  accel.getEvent(&accel_event);
  Ax = (accel_event.acceleration.x - Ax_offset) / Ax_scale * convert_accel; // m/s^2
  Ay = (accel_event.acceleration.y - Ay_offset) / Ay_scale * convert_accel;
  Az = (accel_event.acceleration.z - Az_offset) / Az_scale * convert_accel;


  // Magnetometer
  sensors_event_t mag_event;
  mag.getEvent(&mag_event);
  Bx = mag_event.magnetic.x - Mx_offset;
  By = mag_event.magnetic.y - My_offset;
  Bz = mag_event.magnetic.z - Mz_offset;
  Mx = ( Bx * Mx_scale + By * Mxy_scale + Bz * Mxz_scale ) * convert_mag;
  My = ( Bx * Mxy_scale + By * My_scale + Bz * Myz_scale ) * convert_mag;
  Mz = ( Bx * Mxz_scale + By * Myz_scale + Bz * Mz_scale ) * convert_mag;
  
  

  // Print data
  Serial.print(T);
  Serial.print('\t');
  Serial.print(Gx,4);
  Serial.print('\t');
  Serial.print(Gy,4);
  Serial.print('\t');  
  Serial.print(Gz,4);
  Serial.print('\t');
  Serial.print(Ax,4);
  Serial.print('\t');
  Serial.print(Ay,4);
  Serial.print('\t');  
  Serial.print(Az,4);
  Serial.print('\t');    
  Serial.print(Mx,4);
  Serial.print('\t');  
  Serial.print(My,4);
  Serial.print('\t');  
  Serial.print(Mz,4);
  Serial.print('\n');  
  delay(1000);
}

