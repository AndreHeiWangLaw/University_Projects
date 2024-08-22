#include<Wire.h>
#include <stdlib.h>
#include <avr/io.h>
#include <cstdlib>
const int MPU=0x68; 
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

void setup(){
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B); 
  Wire.write(0);    
  Wire.endTransmission(true);
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  float y;

}
void loop(){
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);  
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,12,true);  
  AcX=Wire.read()<<8|Wire.read();    
  AcY=Wire.read()<<8|Wire.read();  
  AcZ=Wire.read()<<8|Wire.read();  
  GyX=Wire.read()<<8|Wire.read();  
  GyY=Wire.read()<<8|Wire.read();  
  GyZ=Wire.read()<<8|Wire.read();  
  
  Serial.print("Accelerometer: ");
  Serial.print("X = "); Serial.print(AcX);
  Serial.print(" | Y = "); Serial.print(AcY);
  Serial.print(" | Z = "); Serial.println(AcZ); 
  
  Serial.print("Gyroscope: ");
  Serial.print("X = "); Serial.print(GyX);
  Serial.print(" | Y = "); Serial.print(GyY);
  Serial.print(" | Z = "); Serial.println(GyZ);
  Serial.println(" ");
  delay(333);
  
  
  if (GyZ >= 90 || GyZ <= -90){		// degree
	  digitalWrite(LED_BUILTIN, HIGH);
  }
  
  if (GyX < abs(0.01)){
	  // off LED
	  y = 0;
  } else if (GyX > abs(1)){
	  // max brightness
	  y = 255;
  } else
	  // y= ax + b
	  if (GyX > 0){
		y = 255*GyX; // ex: Gyx = 0.5, y = 127.5
	  } else {
		y = -255*GyX; // ex: Gyx = -0.5, y = 127.5
	  }
  }
  analogWrite(11,y);

}