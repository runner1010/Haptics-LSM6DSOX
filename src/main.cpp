#include <Arduino.h>
#include "LSM6DSOXSensor.h"

// Declare LSM6DSOX sensor. Sensor address can have 2 values LSM6DSOX_I2C_ADD_L (corresponds to 0x6A I2C address) or LSM6DSOX_I2C_ADD_H (corresponds to 0x6B I2C address)
// On Adafruit lsm6dsox sensor, LSM6DSOX_I2C_ADD_L is the default address
LSM6DSOXSensor lsm6dsoxSensor = LSM6DSOXSensor(&Wire, LSM6DSOX_I2C_ADD_L);

int scaling[2] = {3000, -3000};

void setup() {
  Serial.begin(115200);
  //Wire.begin();
  Wire.begin(6, 7); // Initialize I2C with custom SDA and SCL pins (6 and 7 in this case)
  delay(5000); // Wait for Serial to be ready

  // Default clock is 100kHz. LSM6DSOX also supports 400kHz, let's use it
  Wire.setClock(400000);

  // Init the sensor
  lsm6dsoxSensor.begin();

  // Enable accelerometer and gyroscope, and check success
  if (lsm6dsoxSensor.Enable_X() == LSM6DSOX_OK && lsm6dsoxSensor.Enable_G() == LSM6DSOX_OK) {
    Serial.println("Success enabling accelero and gyro");
  } else {
    Serial.println("Error enabling accelero and gyro");
  }

  // Read ID of device and check that it is correct
  uint8_t id;
  lsm6dsoxSensor.ReadID(&id);
  if (id != LSM6DSOX_ID) {
    Serial.println("Wrong ID for LSM6DSOX sensor. Check that device is plugged");
  } else {
    Serial.println("Receviced correct ID for LSM6DSOX sensor");
  }

  // Set accelerometer scale at +- 4G. Available values are +- 2, 4, 8, 16 G
  lsm6dsoxSensor.Set_X_FS(4);

  // Set gyroscope scale at +- 125 degres per second. Available values are +- 125, 250, 500, 1000, 2000 dps
  lsm6dsoxSensor.Set_G_FS(125);


  // Set Accelerometer sample rate to 6667 Hz. Available values are +- 12.0, 26.0, 52.0, 104.0, 208.0, 416.0, 833.0, 1667.0, 3333.0, 6667.0 Hz
  lsm6dsoxSensor.Set_X_ODR(6667.0f);


  // Set Gyroscope sample rate to 208 Hz. Available values are +- 12.0, 26.0, 52.0, 104.0, 208.0, 416.0, 833.0, 1667.0, 3333.0, 6667.0 Hz
  lsm6dsoxSensor.Set_G_ODR(208.0f);


}

void loop() {

// Read accelerometer
uint8_t acceleroStatus;
lsm6dsoxSensor.Get_X_DRDY_Status(&acceleroStatus);
if (acceleroStatus == 1) { // Status == 1 means a new data is available
int32_t acceleration[3];
lsm6dsoxSensor.Get_X_Axes(acceleration);

// Plot data for each axis in g
Serial.print(">");
Serial.print("AX:");
Serial.print(acceleration[0]/1000.0f); // Convert to g
Serial.print(", AY:");
Serial.print(acceleration[1]/1000.0f); // Convert to g
Serial.print(", AZ:");
Serial.print((acceleration[2]/1000.0f)+1); // Convert to g and add 1g offset to Z axis
Serial.print(", Upper:");
Serial.print(scaling[0]/1000.0f); // Convert to g
Serial.print(", Lower:");
Serial.print(scaling[1]/1000.0f); // Convert to g
//Serial.println("");
}

// Read gyroscope
uint8_t gyroStatus;
lsm6dsoxSensor.Get_G_DRDY_Status(&gyroStatus);
if (gyroStatus == 1) { // Status == 1 means a new data is available
int32_t rotation[3];
lsm6dsoxSensor.Get_G_Axes(rotation);

// Plot data for each axis in milli degrees per second
//Serial.print(">");
Serial.print(", GX=");
Serial.print(rotation[0]);
Serial.print(", GY=");
Serial.print(rotation[1]);
Serial.print(", GZ=");
Serial.print(rotation[2]);
Serial.println("");
}

delay(10);

}