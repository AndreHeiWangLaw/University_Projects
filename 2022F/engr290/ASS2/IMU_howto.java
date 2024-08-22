/*
   Arduino and MPU6050 Accelerometer and Gyroscope Sensor Tutorial
   by Dejan, https://howtomechatronics.com
*/
#include <Wire.h>
#include <stdlib.h>
#include <avr/io.h>
#include <cstdlib>
const int MPU = 0x68; // MPU6050 I2C address
float AccX, AccY, AccZ;
float GyroX, GyroY, GyroZ;
float accAngleX, accAngleY, gyroAngleX, gyroAngleY, gyroAngleZ;
float roll, pitch, yaw;
float AccErrorX, AccErrorY, GyroErrorX, GyroErrorY, GyroErrorZ;
float elapsedTime, currentTime, previousTime;
float sensitivityGyro;
float sensitivityAcc;
float y;
int c = 0;

void setup() {
  Serial.begin(19200);
  Wire.begin();                      // Initialize comunication
  pinMode(LED_BUILTIN, OUTPUT);
  
	// Set the accelerometer sensitivity scale range
	// 1 = ±2g, 2 = ±4g, 3 = ±8g and 4 = ±16g
	switch(1){
		case 1:
			Wire.beginTransmission(MPU);       // Start communication with MPU6050 // MPU=0x68
			Wire.write(0x6B);                  // Talk to the register 
			Wire.write(0x00);                  // Set the register bits 0000 0000 (B)
			Wire.endTransmission(true);        // End the transmission
			sensitivityAcc = 16384;		   // LSB Sensitivity ±2g
			break;
		case 2:
			Wire.beginTransmission(MPU);       // Start communication with MPU6050 // MPU=0x68
			Wire.write(0x6B);                  // Talk to the register 
			Wire.write(0x08);                  // Set the register bits 0000 1000 (B)
			Wire.endTransmission(true);        // End the transmission
			sensitivityAcc = 8192; 		   // LSB Sensitivity ±2g
			break;
		case 3:
			Wire.beginTransmission(MPU);       // Start communication with MPU6050 // MPU=0x68
			Wire.write(0x6B);                  // Talk to the register 
			Wire.write(0x10);                  // Set the register bits 0001 0000 (B)
			Wire.endTransmission(true);        // End the transmission
			sensitivityAcc = 4096; 		   // LSB Sensitivity ±8g
			break;
		case 4:
			Wire.beginTransmission(MPU);       // Start communication with MPU6050 // MPU=0x68
			Wire.write(0x6B);                  // Talk to the register 
			Wire.write(0x18);                  // Set the register bits, 0001 1000 (B)
			Wire.endTransmission(true);        // End the transmission
			sensitivityAcc = 2048; 		   // LSB Sensitivity ±16g
			break;
	  
	}
	
	// Set the Gyroscope sensitivity scale range
	// 1 = ±250°/s, 2 = ±500°/s, 3 = ±1000°/s and 4 = ±2000°/s
	switch(1){
		case 1:
			Wire.beginTransmission(MPU);     	// Start communication with MPU6050 // MPU=0x68
			Wire.write(0x1B);                	// Talk to the register 
			Wire.write(0x00);                	// Set the register bits 0000 0000 (B)
			Wire.endTransmission(true);     	// End the transmission
			sensitivityGyro = 131;				// LSB Sensitivity ±250°/s
			break;
		case 2:
			Wire.beginTransmission(MPU);     	// Start communication with MPU6050 // MPU=0x68
			Wire.write(0x1B);                	// Talk to the register 
			Wire.write(0x08);                	// Set the register bits 0000 1000 (B)
			Wire.endTransmission(true);    		// End the transmission
			sensitivityGyro = 65.5; 			// LSB Sensitivity ±500°/s
			break;
		case 3:
			Wire.beginTransmission(MPU);    	// Start communication with MPU6050 // MPU=0x68
			Wire.write(0x1B);               	// Talk to the register 
			Wire.write(0x10);               	// Set the register bits 0001 0000 (B)
			Wire.endTransmission(true);    	    // End the transmission
			sensitivityGyro = 32.8; 			// LSB Sensitivity ±1000°/s
			break;
		case 4:
			Wire.beginTransmission(MPU);    	// Start communication with MPU6050 // MPU=0x68
			Wire.write(0x1B);               	// Talk to the register 
			Wire.write(0x18);               	// Set the register bits, 0001 1000 (B)
			Wire.endTransmission(true);     	// End the transmission
			sensitivityGyro = 16.4; 			// LSB Sensitivity ±2000°/s
			break;
	  
	}
  /*
  Wire.beginTransmission(MPU);       // Start communication with MPU6050 // MPU=0x68
  Wire.write(0x6B);                  // Talk to the register 6B
  Wire.write(0x00);                  // Make reset - place a 0 into the 6B register
  Wire.endTransmission(true);        //end the transmission
  */
  
  /*
  // Configure Accelerometer Sensitivity - Full Scale Range (default +/- 2g)
  Wire.beginTransmission(MPU);
  Wire.write(0x1C);                  //Talk to the ACCEL_CONFIG register (1C hex)
  Wire.write(0x10);                  //Set the register bits as 00010000 (+/- 8g full scale range)
  Wire.endTransmission(true);
  
  // Configure Gyro Sensitivity - Full Scale Range (default +/- 250deg/s)
  Wire.beginTransmission(MPU);
  Wire.write(0x1B);                   // Talk to the GYRO_CONFIG register (1B hex)
  Wire.write(0x10);                   // Set the register bits as 00010000 (1000deg/s full scale)
  Wire.endTransmission(true);
  delay(20);
  */
  
  // Call this function if you need to get the IMU error values for your module
  //calculate_IMU_error();
  delay(20);
}

void loop() {
  // === Read acceleromter data === //
  Wire.beginTransmission(MPU);
  Wire.write(0x3B); // Start with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true); // Read 6 registers total, each axis value is stored in 2 registers
  
  //For a any range, we need to divide the raw values by sensitivityAcc, according to the datasheet
  AccX = (Wire.read() << 8 | Wire.read()) / sensitivityAcc; // X-axis value
  AccY = (Wire.read() << 8 | Wire.read()) / sensitivityAcc; // Y-axis value
  AccZ = (Wire.read() << 8 | Wire.read()) / sensitivityAcc; // Z-axis value
  
  // Calculating Roll and Pitch from the accelerometer data
  accAngleX = (atan(AccY / sqrt(pow(AccX, 2) + pow(AccZ, 2))) * 180 / PI) - 0.58; // AccErrorX ~(0.58) See the calculate_IMU_error()custom function for more details
  accAngleY = (atan(-1 * AccX / sqrt(pow(AccY, 2) + pow(AccZ, 2))) * 180 / PI) + 1.58; // AccErrorY ~(-1.58)
 
  // === Read gyroscope data === //
  previousTime = currentTime;        // Previous time is stored before the actual time read
  currentTime = millis();            // Current time actual time read
  elapsedTime = (currentTime - previousTime) / 1000; // Divide by 1000 to get seconds
  Wire.beginTransmission(MPU);
  Wire.write(0x43); // Gyro data first register address 0x43
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true); // Read 4 registers total, each axis value is stored in 2 registers
  
  GyroX = (Wire.read() << 8 | Wire.read()) / sensitivityGyro; // For any range we have to divide first the raw value by sensitivityGyro, according to the datasheet
  GyroY = (Wire.read() << 8 | Wire.read()) / sensitivityGyro;
  GyroZ = (Wire.read() << 8 | Wire.read()) / sensitivityGyro;
  
  // Correct the outputs with the calculated error values
  GyroX = GyroX + 0.56; // GyroErrorX ~(-0.56)
  GyroY = GyroY - 2; // GyroErrorY ~(2)
  GyroZ = GyroZ + 0.79; // GyroErrorZ ~ (-0.8)
  
  // Currently the raw values are in degrees per seconds, deg/s, so we need to multiply by sendonds (s) to get the angle in degrees
  gyroAngleX = gyroAngleX + GyroX * elapsedTime; // deg/s * s = deg
  gyroAngleY = gyroAngleY + GyroY * elapsedTime;
  yaw =  yaw + GyroZ * elapsedTime;
  
  // Complementary filter - combine acceleromter and gyro angle values
  roll = 0.96 * gyroAngleX + 0.04 * accAngleX;
  pitch = 0.96 * gyroAngleY + 0.04 * accAngleY;
  
  // Print the values on the serial monitor
  Serial.print("Roll: ");
  Serial.print(roll);
  Serial.print(" / ");
  Serial.print("Pitch: ");
  Serial.print(pitch);
  Serial.print(" / ");
  Serial.print("Yaw: ");
  Serial.println(yaw);
  
  // LED "L" control from yaw readings
  if (yaw >= 90 || yaw <= -90){		// degree
	  digitalWrite(LED_BUILTIN, HIGH);
  }
  
  // D3 brightness control on x-axis
  if (abs(AccX) < 0.01){
      // off LED
      y = 0;
  } else if (abs(AccX) > 1){
      // max brightness
      y = 255;
  } else
      // y= ax + b
      if (AccX > 0){     // roll is positive
          y = 255*AccX;     // ex: if roll = 0.5, y = 255*0.5 = 127.5
      } else {            // roll is negative
          y = -255*AccX;     // ex: if roll = -0.5, y = -255*-0.5 = 127.5
      }
  
  
  Serial.print("\nD3 Brightness: ");
  Serial.print(y); 		// value sould range from  0 (dim) to 255 (bright)
  analogWrite(11,y);
  
  
}

void calculate_IMU_error() {
  // We can call this funtion in the setup section to calculate the accelerometer and gyro data error. From here we will get the error values used in the above equations printed on the Serial Monitor.
  // Note that we should place the IMU flat in order to get the proper values, so that we then can the correct values
  // Read accelerometer values 200 times
  while (c < 200) {
    Wire.beginTransmission(MPU);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU, 6, true);
    AccX = (Wire.read() << 8 | Wire.read()) / sensitivityAcc ;
    AccY = (Wire.read() << 8 | Wire.read()) / sensitivityAcc ;
    AccZ = (Wire.read() << 8 | Wire.read()) / sensitivityAcc ;
    // Sum all readings
    AccErrorX = AccErrorX + ((atan((AccY) / sqrt(pow((AccX), 2) + pow((AccZ), 2))) * 180 / PI));
    AccErrorY = AccErrorY + ((atan(-1 * (AccX) / sqrt(pow((AccY), 2) + pow((AccZ), 2))) * 180 / PI));
    c++;
  }
  
  //Divide the sum by 200 to get the error value
  AccErrorX = AccErrorX / 200;
  AccErrorY = AccErrorY / 200;
  c = 0;
  
  // Read gyro values 200 times
  while (c < 200) {
    Wire.beginTransmission(MPU);
    Wire.write(0x43);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU, 6, true);
    GyroX = Wire.read() << 8 | Wire.read();
    GyroY = Wire.read() << 8 | Wire.read();
    GyroZ = Wire.read() << 8 | Wire.read();
	
    // Sum all readings
    GyroErrorX = GyroErrorX + (GyroX / sensitivityGyro);
    GyroErrorY = GyroErrorY + (GyroY / sensitivityGyro);
    GyroErrorZ = GyroErrorZ + (GyroZ / sensitivityGyro);
    c++;
  }
  
  //Divide the sum by 200 to get the error value
  GyroErrorX = GyroErrorX / 200;
  GyroErrorY = GyroErrorY / 200;
  GyroErrorZ = GyroErrorZ / 200;
  
  // Print the error values on the Serial Monitor
  Serial.print("AccErrorX: ");
  Serial.println(AccErrorX);
  Serial.print("AccErrorY: ");
  Serial.println(AccErrorY);
  Serial.print("GyroErrorX: ");
  Serial.println(GyroErrorX);
  Serial.print("GyroErrorY: ");
  Serial.println(GyroErrorY);
  Serial.print("GyroErrorZ: ");
  Serial.println(GyroErrorZ);
}