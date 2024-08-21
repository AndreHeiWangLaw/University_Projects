// Assignment 3, Question 2, Week 7
// Hei Wang Andre Law, 4017 5600
// Program: Reverse order of inputed number

#include<iostream>
using namespace std;

void reverse(); // function prototype of the reverse function

int main() {
	reverse(); // call the reverse function  
	cout << endl;
	reverse(); // call the reverse function again
	return 0;
}

// reverse function
void reverse() {
	int num; // user input of integer
	int reverse = 0; // set initial value of the reversed number to zero
	int rest; // the remain of a division

	// prompt user to enter a number
	cout << "Type an integer number: "; 
	cin >> num; 

	// while loop as long as it isn't zero (meaning still divisable by 10 to get a 'rest')
	while (num != 0) {
		rest = num % 10; // the remain corresponding the right-most digit
		reverse = reverse * 10 + rest; // set the second left-most digit as the rest
		num = num / 10; // division original number by 10 for the while loop check 
	}
	cout << "Output: " << reverse << endl; // output the final reversed number
}