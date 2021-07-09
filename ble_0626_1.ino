#include <Arduino_LSM9DS1.h> // 9 축 IMU 용 라이브러리 포함
#include <ArduinoBLE.h>


void setup()
{
  Serial.begin(9600); //Serial monitor to display all sensor values
  if (!IMU.begin()) //Initialize IMU sensor
  { 
    Serial.println("Failed to initialize IMU!"); 
    while (1);
  }

}

float accel_x, accel_y, accel_z;
float gyro_x, gyro_y, gyro_z;
void loop()
{
  if (IMU.accelerationAvailable()) 
    {
      IMU.readAcceleration(accel_x, accel_y, accel_z);
      Serial.print("Accelerometer = ");
      Serial.print(accel_x); 
      Serial.print(", ");
      Serial.print(accel_y);
      Serial.print(", ");
      Serial.println(accel_z);
    }
      delay (200);

  //Gyroscope values 
  if (IMU.gyroscopeAvailable()) 
    {
      IMU.readGyroscope(gyro_x, gyro_y, gyro_z);
      Serial.print("Gyroscope = ");
      Serial.print(gyro_x); 
      Serial.print(", ");
      Serial.print(gyro_y);
      Serial.print(", ");
      Serial.println(gyro_z);
    }
      delay (200);
}
