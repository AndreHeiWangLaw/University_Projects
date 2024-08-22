// ctrl+f '~' in order to find comments of code to be coded

// trig = when pin set to HIGH, US pulse of 10us
// echo = pin set to high when US burst is transmitted until sensor detects echo 
			// the time it remained high/2 = distance (most likely not gonna be in cm)

int main() {
	
	// Initialise GPIOs
	cli ();			// Teacher had it in his code... smth related to interrupt
	DDRB=(1<<PB3);	// Output for PWM (Echo),not sure but D3 Led is connected to PB3
					// Plug HC-SR04 into P6 of the board???
	PORTB=(1<<PB3);	// PB3-HI (D3 OFF)
	
	PORTD=(1<<PD2);	// Enable pull-ups, Input (Trig), checkout connector P6
	sei();			// // Teacher had it in his code... smth related to interrupt
	
	
	// ~ digitalwrite of trig low, (PORTD&(1<<PD2))
	// ~ delay before start of new cycle
	// ~ digitalwrite of trig high (!(PORTD&(1<<PD2)))
	// ~ delay of 10us = duration where trig is HIGH
	// ~ digitalwrite of trig low, (PORTD&(1<<PD2))
	
	while (1){
		keypress = getchar();
		if (keypress=='k'){ // get out of infinite loop w/ keypress 'k'
			break;
		}
	
		// ~ figure out the duration of echo
		// pinB3_status=(PINB&(1<<PB3)); //check status of pin3
		
		
		if 	(PINB&(1<<PB3)) { // rising edge IRQ - start of echo
			// ~ start timer 't'
		}
		if (!(PINB&(1<<PB3)) { //falling edge IRQ - end of echo
			// ~ end timer 't' where distance = (0.034cm/us * t us)/2
			// 340m/s = speed of sound
		}
		// ~ print distance in cm
	}
	return 0;
}



