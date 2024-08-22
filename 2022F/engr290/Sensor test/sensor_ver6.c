// Code referenced from Dr. Rastko Selmic's "Sample Code (.c and .h files)"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
// #include <util/twi.h>
#include <string.h>
#include <util/delay.h>


// copy pasted from https://arduino.stackexchange.com/questions/40213/arduino-uno-uart-coding-in-pure-c
#define F_CPU 16000000UL
#define BAUD 9600
#define BAUD_PRESCALE (((F_CPU / (BAUD * 16UL))) - 1)
//#include<util/setbaud.h>

// Comment/uncomment ONE of the TWO to define/undefine IR or US sensor
//#define IR_SENSOR
#define US_SENSOR

// Error message when BOTH or NEITHER of US and IR sensor are defined above
#if !defined(IR_SENSOR) && !defined(US_SENSOR) || defined(IR_SENSOR) && defined(US_SENSOR)
#error Comment/uncomment ONE of the TWO to define/undefine IR or US sensor
#endif

/*
Speed of Sound is 340 m/s 
340 m/s = 29 microseconds/cm

*/
#if defined(US_SENSOR)
#define ADC_VALUE_AT_15CM 11  //  255
#define ADC_VALUE_AT_40CM 29  //  0
#endif

// Declare Functions
void UART_Init();				// inisialise USART
void pointerString(char*);		// iterate over a string suing char pointer
void yellowLed(int);				// PB5 yellow led out of range detector
int adcConvertion();        
double calculateDistance(int adcValue);

// main function
int main(){
	
	// initiate data type avariables
	char pulseDistStr[5];					// pulse char array used in 'pointerString', buffer of 100
	long duration, distance;							
	int adcValue;
	
	// Initialise I/O
	DDRB|=(1<<PB3);				// Set PB3 as output (Trig) (Shared w/ D3 LED)
	DDRD&=!(1<<PD2);			// Set PD2 as input (Echo)
	DDRB|=(1<<PB5);				// Set PB5 as output (Shared w/ Yellow LED)
	
	UART_Init();
	
	while (1){
		// Pulse Wave
		PORTB&=!(1<<PB3);			// Set PB3 to low 
		_delay_us(2);				// Delay 2 microseconds
		PORTB|=(1<<PB3);			// Set PB3 to high
		_delay_us(10);				// Delay 10 Microseconds
		PORTB&=!(1<<PB3);			// Set PB3 to low
		
		adcValue=adcConvertion();
		distance= calculateDistance(adcValue);
		itoa(distance, pulseDistStr, 10);    		// convert int to string (base of 10)
		
		pointerString("Distance: "); 				// print  
		pointerString(pulseDistStr);				// print value of the distance in cm
		pointerString(" cm \n");					// print 
		
		yellowLed(distance);				// PB5 yellow led out of range detector
		
		//pointerString("Distance in cm : ");
		Serial.print(distance);
		_delay_us(1000);
	}
	return 0;
}

// copy pasted from https://arduino.stackexchange.com/questions/40213/arduino-uno-uart-coding-in-pure-c
void UART_Init(){									// inisialise USART
	UBRR0H = (uint8_t)(BAUD_PRESCALE>>8);					// sets high baud rate, shifted right side by 8
	UBRR0L = (uint8_t)(BAUD_PRESCALE); 						// sets low baud rate
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);				// turn on the transmission and reception
	UCSR0C = (3<<UCSZ00);
}

// referenced in https://www.avrfreaks.net/forum/tut-soft-using-usart-serial-communications?name=PNphpBB2&file=viewtopic&t=45341
void pointerString(char* stringPtr){
	while (*stringPtr != 0) {					// Loop as long as data of pointer isn't null 
		while (!(UCSR0A & (1<<UDRE0)));			// Do nothing until data is received
		UDR0 = *stringPtr;						// Echo back the byte value to 'stringPtr'
		stringPtr++;							// stringPtr = stringPtr + 1
	}
}

void yellowLed(int distDetect){
	if (distDetect < 15 || distDetect > 40){
		PORTB|=(1<<PB5);			// Set PB5 to high = LED ON
	} else
		PORTB&=!(1<<PB5);			// Set PB5 to high = LED OFF
}

int adcConvertion(){

  ADMUX |= B00000100;    // read from A4
  ADMUX |= B11000000;    // REFS1 and REFS0 equal 1 

  ADCSRA |= B11000000;   // ADEN and ADSC equal 1
  while(bit_is_set(ADCSRA,ADSC)); // To know when the conversion is done
  int value = ADCL | (ADCH << 8);
  return value;
}

double calculateDistance(int adcValue){

double a = (40.0 - 15.0) / (ADC_VALUE_AT_40CM - ADC_VALUE_AT_15CM);
double b = 40.0 - (a * ADC_VALUE_AT_40CM);
double distance = (a * ((double) adcValue)) + b;
return distance;
}