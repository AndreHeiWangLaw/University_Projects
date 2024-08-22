// Engr 290 HW 1 

#include <stdlib.h> 
#include <avr/io.h>
#include <util/delay.h>

long duration, distance;

void setup(){
	
	Serial.begin(9600);			//Cheese arduino code, to be replaced = baud rate
	
	// Set I/O for P6
	DDRB|=(1<<PB3);				// Set PB3 as output (Trig) (Shared w/ D3 LED)
	DDRD&=!(1<<PD2);			// Set PD2 as input (Echo)
}

void loop(){
	
	// Pulse Wave
	PORTB&=!(1<<PB3);			// Set PB3 to low 
	_delay_us(2);				// Delay 2 microseconds
	PORTB|=(1<<PB3);			// Set PB3 to high
	_delay_us(10);				// Delay 10 Microseconds
	PORTB&=!(1<<PB3);			// Set PB3 to low
	
	// Cheese arduino code, to be replaced
	duration = pulseIn(PD2, HIGH); 		
	distance = duration * 0.034 / 2;
	Serial.print("Distance: ");
	Serial.print(distance);
	Serial.print(" cm");
	Serial.println();
	
	// LED "L" Control on Arduino Board
	
	// LED D3 Brightness Control
	
}